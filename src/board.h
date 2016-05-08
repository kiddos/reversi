#ifndef BOARD_H
#define BOARD_H

#include <vector>

namespace reversi {

#define BOARDSIZE 8
#define NDIRECT 8

struct Move {
  int x, y;
  Move() : x(0), y(0) {}
  Move(int x, int y) : x(x), y(y) {}
  Move operator+(const int delta[2]) const {return Move(x+delta[0], y+delta[1]);}
};

class Board {
 public:
  Board();
  Board(int config[BOARDSIZE][BOARDSIZE]);
  Board(const Board& board);
  Board& operator= (const Board& b);

  bool isvalidmove(const Move m);
  bool isvalidmove(const Move m, const int delta[2]);
  std::vector<Move> getvalidmoves();
  bool perform(const Move m);
  bool changeturn();

  void copyboard(int board[BOARDSIZE][BOARDSIZE]) const;
  void quickdisplay() const;
  int getblackcount() const;
  int getwhitecount() const;
  int getturn() const;
  bool isfull() const;

  static const int WHITE, BLACK, EMPTY;
  static const int DELTA[NDIRECT][2];

 private:
  bool inrange(const Move c) const;

  int board[BOARDSIZE][BOARDSIZE];
  int turn;
};

} /* end of reversi namespace */

#endif /* end of include guard: BOARD_H */
