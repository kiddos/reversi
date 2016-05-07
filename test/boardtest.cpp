#include <iostream>
#include <vector>

#include "../src/board.h"

using namespace reversi;
using namespace std;

void printmoves(vector<Move> moves) {
  for (vector<Move>::iterator m = moves.begin() ; m != moves.end() ; ++m) {
    cout << (*m).x << ", " << (*m).y << endl;
  }
}

void copyboard(Board& b1, Board& b2) {
  b2 = b1;
}

int main(void) {
  Board b1;
  printmoves(b1.getvalidmoves());
  b1.quickdisplay();

  int config1[8][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {-1, 1, 1, 1, 1, 1, 1, 0}
  };
  Board b2(config1);
  printmoves(b2.getvalidmoves());
  b2.quickdisplay();

  int config2[8][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {-1, 1, 1, 1, 1, 1, 1, 0}
  };
  Board b3(config2);
  b3.changeturn();

  Board b4;
  copyboard(b3, b4);
  b4.changeturn();
  b4.quickdisplay();

  int config3[8][8];
  b4.copyboard(config3);

  Board b5(config3);
  b5.quickdisplay();

  return 0;
}
