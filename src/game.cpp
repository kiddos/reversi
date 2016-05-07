#include "game.h"

namespace reversi {

Game::Game() : over(false) {}

Game::Game(const Board& board) : board(board) {}

Game::Game(const Game& game) : ai(game.ai), board(game.board), over(game.over) {}

Game& Game::operator= (const Game& game) {
  this->ai = game.ai;
  this->board = game.board;
  this->over = game.over;
  return *this;
}

bool Game::playerinput(const Move m) {
  if (board.isvalidmove(m)) {
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
  if (board.getvalidmoves().size() == 0)
    over = true;
  return over;
}

Board Game::getboard() const {
  return board;
}

}
