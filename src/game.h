#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <fstream>
#include "board.h"
#include "ai.h"

namespace reversi {

struct Record {
  Board state;
  Move move;
  int turn;
  Record(Board state, Move move, int turn) : state(state), move(move), turn(turn) {}
  Record(const Record& r) : state(r.state), move(r.move), turn(r.turn) {}
  Record& operator= (const Record& r) {
    state=r.state;move=r.move;turn=r.turn;
    return *this;
  }
};

class Game {
 public:
  Game();
  explicit Game(const Board& board);
  Game(const Game& game);
  Game& operator= (const Game& game);

  bool playerinput(const Move m);
  bool airespond();
  bool isgameover();
  void quicksimulate();
  void saverecords(bool readable=true) const;
  int getwinner() const;
  Board getboard() const;
  std::vector<Record> getrecords() const;

 private:
  bool isrecordexists(const std::string path) const;
  std::string getrecordpath(const std::string ext) const;
  AI ai;
  Board board;
  bool over;
  std::vector<Record> records;
};

}

#endif /* end of include guard: GAME_H */
