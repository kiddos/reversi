#include "game.h"

using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;
using std::endl;

namespace reversi {

Game::Game() : over(false) {}

Game::Game(const Board& board) : board(board), over(false) {}

Game::Game(const Game& game) : ai(game.ai), board(game.board), over(game.over),
                               records(game.records) {}

Game& Game::operator= (const Game& game) {
  ai = game.ai;
  board = game.board;
  over = game.over;
  records = game.records;
  return *this;
}

bool Game::playerinput(const Move m) {
  if (board.isvalidmove(m)) {
    // add record
    Record r(board, m, board.getturn());
    records.push_back(r);
    // perform move
    board.perform(m);
    return true;
  } else {
    board.changeturn();
    if (board.getvalidmoves().size() == 0)
      over = true;
    return false;
  }
}

bool Game::airespond() {
  Move m = ai.decidemove(board);
  if (board.isvalidmove(m)) {
    // add record
    Record r(board, m, board.getturn());
    records.push_back(r);
    // perform move
    board.perform(m);
    return true;
  } else {
    board.changeturn();
    if (board.getvalidmoves().size() == 0)
      over = true;
    return false;
  }
}

bool Game::isgameover() {
  if (!over)
    over = board.isfull();
  return over;
}

bool Game::isrecordexists(const string path) const {
  ifstream input(path, std::ios::in);
  if (input.is_open()) {
    input.close();
    return true;
  }
  return false;
}

string Game::getrecordpath(const string ext) const {
  int index = 0;
  const string base = "game";
  string path = base + std::to_string(index) + ext;
  while (isrecordexists(path)) {
    index ++;
    path = base + std::to_string(index) + ext;
  }
  return path;
}

void Game::saverecords(bool readable) const {
  if (readable) {
    ofstream output(getrecordpath(".record"), std::ios::out);
    for (size_t i = 0 ; i < records.size() ; ++i) {
      int b[BOARDSIZE][BOARDSIZE] = {{0}};
      output << (records[i].turn == Board::WHITE ? "white" : "black");
      output << ": " << records[i].move.x << ", " << records[i].move.y << endl;
      records[i].state.copyboard(b);
      for (int i = 0 ; i < BOARDSIZE ; ++i) {
        for (int j = 0 ; j < BOARDSIZE ; ++j) {
          if (b[i][j] == Board::WHITE)
            output << "0 ";
          else if (b[i][j] == Board::BLACK)
            output << "* ";
          else if (b[i][j] == Board::EMPTY)
            output << "  ";
        }
        output << endl;
      }
    }
    const Record last = records[records.size()-1];
    output << "white: " << last.state.getwhitecount();
    output << ", black: " << last.state.getblackcount();
    output.close();
  } else {
    ofstream output(getrecordpath(".input"), std::ios::out);
    int winner = getwinner();
    for (size_t i = 0 ; i < records.size() ; ++i) {
      int* input = new int[AI::INPUT_NODES];
      AI::toinput(records[i].state, records[i].move, records[i].turn, input);
      for (int i = 0 ; i < AI::INPUT_NODES ; ++i) {
        output << input[i] << " ";
      }
      if (records[i].turn == winner) output << "1";
      else output << "0";
      delete input;
      output << endl;
    }
    output.close();
  }
}

void Game::quicksimulate() {
  while (!isgameover()) {
    airespond();
  }
}

int Game::getwinner() const {
  if (over) {
    return board.getblackcount() > board.getwhitecount() ?
        Board::BLACK : Board::WHITE;
  }
  return Board::EMPTY;
}

Board Game::getboard() const {
  return board;
}

vector<Record> Game::getrecords() const {
  return records;
}

}
