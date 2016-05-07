#include "../src/game.h"
#include <iostream>

using reversi::Board;
using reversi::Game;
using std::cout;
using std::endl;

int main(void) {
  int c1[8][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {-1, 1, 1, 1, 1, 1, 1, 1}
  };
  Board b1(c1);
  Game g1(b1);
  cout << g1.isgameover() << endl;
  return 0;
}
