#include "board.h"

#ifdef DEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif

#define OPPOSITE(turn) -(turn)
#define BOARD(move) board[move.y][move.x]

using std::vector;

namespace reversi {

const int Board::WHITE = 1;
const int Board::BLACK = -1;
const int Board::EMPTY = 0;
const int Board::DELTA[NDIRECT][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0},
                                      {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

Board::Board() {
  turn = WHITE;
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for(int j = 0 ; j < BOARDSIZE ; ++j) {
      board[i][j] = EMPTY;
    }
  }

  board[3][3] = WHITE;
  board[4][4] = WHITE;
  board[3][4] = BLACK;
  board[4][3] = BLACK;
}

Board::Board(int config[BOARDSIZE][BOARDSIZE]) : Board() {
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      board[i][j] = config[i][j];
    }
  }
}

Board::Board(const Board& b) : Board() {
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      board[i][j] = b.board[i][j];
    }
  }
  turn = b.turn;
}

Board& Board::operator= (const Board& b) {
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      board[i][j] = b.board[i][j];
    }
  }
  turn = b.turn;
  return *this;
}

bool Board::inrange(const Move m) const {
  if (m.x >= BOARDSIZE || m.y >= BOARDSIZE || m.x < 0 || m.y < 0)
    return false;
  return true;
}

bool Board::isvalidmove(const Move m) const {
  for (int i = 0 ; i < NDIRECT ; ++i) {
    if (isvalidmove(m, DELTA[i])) return true;
  }
  return false;
}

bool Board::isvalidmove(const Move m, const int delta[2]) const {
  if (!inrange(m) || BOARD(m) != EMPTY)
    return false;

  Move newm = m + delta;
  if (inrange(newm) && BOARD(newm) == OPPOSITE(turn)) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      newm = newm + delta;
      if (inrange(newm) && BOARD(newm) == EMPTY)
        return false;
      else if (inrange(newm) && BOARD(newm) == turn)
        return true;
    }
  }
  return false;
}

vector<Move> Board::getvalidmoves() const {
  vector<Move> moves;
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for (int j = 0 ; j < BOARDSIZE; ++j) {
      Move m(i, j);
      if (isvalidmove(m)) {
        moves.push_back(m);
      }
    }
  }
  return moves;
}

bool Board::perform(const Move m) {
  bool performed = false;
  for (int i = 0 ; i < NDIRECT ; ++i) {
    if (isvalidmove(m, DELTA[i])) {
      if (!performed) performed = true;

      Move newm = m + DELTA[i];
      for (int j = 0 ; j < BOARDSIZE ; ++j) {
        if (inrange(newm) && BOARD(newm) == OPPOSITE(turn))
          BOARD(newm) = turn;
        else if (BOARD(newm) == turn)
          continue;

        newm = newm + DELTA[i];
      }
    }
  }

  if (performed) {
    BOARD(m) = turn;
    turn = OPPOSITE(turn);
  }

  return performed;
}

bool Board::changeturn() {
  for (int i = 0 ; i < BOARDSIZE; ++i) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      Move m(i, j);
      if (isvalidmove(m)) {
#ifdef DEBUG
        cout << "there are valid moves to perform" << endl;
#endif
        return false;
      }
    }
  }

  turn = OPPOSITE(turn);
  return true;
}

void Board::copyboard(int b[BOARDSIZE][BOARDSIZE]) const {
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      b[i][j] = board[i][j];
    }
  }
}

void Board::quickdisplay() const {
#ifdef DEBUG
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      if (board[i][j] == WHITE) {
        cout << "0 ";
      } else if (board[i][j] == BLACK) {
        cout << "* ";
      } else {
        cout << "  ";
      }
    }
    cout << endl;
  }
#endif
}

int Board::getwhitecount() const {
  int whitecount = 0;
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      if (board[i][j] == Board::WHITE) {
        whitecount ++;
      }
    }
  }
  return whitecount;
}

int Board::getblackcount() const {
  int blackcount = 0;
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      if (board[i][j] == Board::BLACK) {
        blackcount ++;
      }
    }
  }
  return blackcount;
}

int Board::getturn() const {
  return turn;
}

bool Board::isfull() const {
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      if (board[i][j] == Board::EMPTY)
        return false;
    }
  }
  return true;
}

}

#undef BOARD
#undef OPPOSITE
