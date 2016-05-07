#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "ai.h"

namespace reversi {

class Game {
 public:
  Game();
  explicit Game(const Board& board);
  Game(const Game& game);
  Game& operator= (const Game& game);

  bool playerinput(const Move m);
  bool airespond();
  bool isgameover();

  Board getboard() const;

 private:
  AI ai;
  Board board;
  bool over;
};

}

#endif /* end of include guard: GAME_H */
