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

double Board::eval() const {
  const double EXTRA_SMALL = 0.01;
  const double SMALL = 0.2;
  const double MEDIUM = 4;
  const double LARGE = 50;
  const double EXTRA_LARGE = 200;

  double score = 0;

  if (board[0][0] == turn) score += EXTRA_LARGE;
  else if (board[0][0] == OPPOSITE(turn)) score -= EXTRA_LARGE;

  if (board[BOARDSIZE-1][0] == turn) score += EXTRA_LARGE;
  else if (board[BOARDSIZE-1][0] == OPPOSITE(turn)) score -= EXTRA_LARGE;

  if (board[0][BOARDSIZE-1] == turn) score += EXTRA_LARGE;
  else if (board[BOARDSIZE-1][0] == OPPOSITE(turn)) score -= EXTRA_LARGE;

  if (board[BOARDSIZE-1][BOARDSIZE-1] == turn) score += EXTRA_LARGE;
  else if (board[BOARDSIZE-1][BOARDSIZE-1] == OPPOSITE(turn)) score -= EXTRA_LARGE;

  if (board[1][1] == turn) score -= LARGE;
  else if (board[1][1] == OPPOSITE(turn)) score += LARGE;

  if (board[BOARDSIZE-2][1] == turn) score -= LARGE;
  else if (board[BOARDSIZE-2][1] == OPPOSITE(turn)) score += LARGE;

  if (board[1][BOARDSIZE-2] == turn) score -= LARGE;
  else if (board[BOARDSIZE-2][1] == OPPOSITE(turn)) score += LARGE;

  if (board[BOARDSIZE-2][BOARDSIZE-1] == turn) score -= LARGE;
  else if (board[BOARDSIZE-2][BOARDSIZE-1] == OPPOSITE(turn)) score += LARGE;

  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    if (board[0][i] == turn) score += MEDIUM;
    else if (board[0][i] == OPPOSITE(turn)) score -= MEDIUM;
    if (board[i][0] == turn) score += MEDIUM;
    else if (board[i][0] == OPPOSITE(turn)) score -= MEDIUM;
    if (board[BOARDSIZE-1][i] == turn) score += MEDIUM;
    else if (board[BOARDSIZE-1][i] == OPPOSITE(turn)) score -= MEDIUM;
    if (board[i][BOARDSIZE-1] == turn) score += MEDIUM;
    else if (board[i][BOARDSIZE-1] == OPPOSITE(turn)) score -= MEDIUM;
  }

  for (int i = 1 ; i < BOARDSIZE-2 ; ++i) {
    if (board[1][i] == turn) score -= SMALL;
    else if (board[1][i] == OPPOSITE(turn)) score += SMALL;
    if (board[i][1] == turn) score -= SMALL;
    else if (board[i][1] == OPPOSITE(turn)) score += SMALL;
    if (board[BOARDSIZE-2][i] == turn) score -= SMALL;
    else if (board[BOARDSIZE-2][i] == OPPOSITE(turn)) score += SMALL;
    if (board[i][BOARDSIZE-2] == turn) score -= SMALL;
    else if (board[i][BOARDSIZE-2] == OPPOSITE(turn)) score += SMALL;
  }

  for (int i = 2 ; i < BOARDSIZE-2 ; ++i) {
    for (int j = 2 ; j < BOARDSIZE-2 ; ++j) {
      if (board[i][j] == turn) score += EXTRA_SMALL;
      else if (board[i][j] == OPPOSITE(turn)) score -= EXTRA_SMALL;
    }
  }
  return score;
}

double Board::eval(int t) const {
  const double EXTRA_SMALL = 0.01;
  const double SMALL = 0.2;
  const double MEDIUM = 4;
  const double LARGE = 50;
  const double EXTRA_LARGE = 200;

  double score = 0;

  if (board[0][0] == t) score += EXTRA_LARGE;
  else if (board[0][0] == OPPOSITE(t)) score -= EXTRA_LARGE;

  if (board[BOARDSIZE-1][0] == t) score += EXTRA_LARGE;
  else if (board[BOARDSIZE-1][0] == OPPOSITE(t)) score -= EXTRA_LARGE;

  if (board[0][BOARDSIZE-1] == t) score += EXTRA_LARGE;
  else if (board[BOARDSIZE-1][0] == OPPOSITE(t)) score -= EXTRA_LARGE;

  if (board[BOARDSIZE-1][BOARDSIZE-1] == t) score += EXTRA_LARGE;
  else if (board[BOARDSIZE-1][BOARDSIZE-1] == OPPOSITE(t)) score -= EXTRA_LARGE;

  if (board[1][1] == t) score += LARGE;
  else if (board[1][1] == OPPOSITE(t)) score -= LARGE;

  if (board[BOARDSIZE-2][1] == t) score += LARGE;
  else if (board[BOARDSIZE-2][1] == OPPOSITE(t)) score -= LARGE;

  if (board[1][BOARDSIZE-2] == t) score += LARGE;
  else if (board[BOARDSIZE-2][1] == OPPOSITE(t)) score -= LARGE;

  if (board[BOARDSIZE-2][BOARDSIZE-1] == t) score += LARGE;
  else if (board[BOARDSIZE-2][BOARDSIZE-1] == OPPOSITE(t)) score -= LARGE;

  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    if (board[0][i] == t) score += MEDIUM;
    else if (board[0][i] == OPPOSITE(t)) score -= MEDIUM;
    if (board[i][0] == t) score += MEDIUM;
    else if (board[i][0] == OPPOSITE(t)) score -= MEDIUM;
    if (board[BOARDSIZE-1][i] == t) score += MEDIUM;
    else if (board[BOARDSIZE-1][i] == OPPOSITE(t)) score -= MEDIUM;
    if (board[i][BOARDSIZE-1] == t) score += MEDIUM;
    else if (board[i][BOARDSIZE-1] == OPPOSITE(t)) score -= MEDIUM;
  }

  for (int i = 1 ; i < BOARDSIZE-2 ; ++i) {
    if (board[1][i] == t) score -= SMALL;
    else if (board[1][i] == OPPOSITE(t)) score += SMALL;
    if (board[i][1] == t) score -= SMALL;
    else if (board[i][1] == OPPOSITE(t)) score += SMALL;
    if (board[BOARDSIZE-2][i] == t) score -= SMALL;
    else if (board[BOARDSIZE-2][i] == OPPOSITE(t)) score += SMALL;
    if (board[i][BOARDSIZE-2] == t) score -= SMALL;
    else if (board[i][BOARDSIZE-2] == OPPOSITE(t)) score += SMALL;
  }

  for (int i = 2 ; i < BOARDSIZE-2 ; ++i) {
    for (int j = 2 ; j < BOARDSIZE-2 ; ++j) {
      if (board[i][j] == t) score += EXTRA_SMALL;
      else if (board[i][j] == OPPOSITE(t)) score -= EXTRA_SMALL;
    }
  }
  return score;
}


void Board::copyto(int b[BOARDSIZE][BOARDSIZE]) const {
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

int Board::gettotalcount() const {
  int total = 0;
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      if (board[i][j] != Board::EMPTY) {
        total ++;
      }
    }
  }
  return total;
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
