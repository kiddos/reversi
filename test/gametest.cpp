#include "../src/game.h"
#include <iostream>
#include <vector>

using reversi::Board;
using reversi::Game;
using reversi::Move;
using std::cout;
using std::endl;
using std::vector;

int main(void) {
  int c1[8][8] = {
    {0, 1, 1, 1, 1, 1, 1, 1},
    {0, -1, 1, 1, 1, 1, 1, 1},
    {0, -1, 1, 1, 1, 1, 1, 1},
    {0, -1, 1, -1, 1, 1, 1, 1},
    {0, -1, -1, 1, 1, 1, 1, 1},
    {-1, 0, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 1, 1, 1, 1}
  };
  Board b1(c1);
  Game g1(b1);
  cout << "game over: " << g1.isgameover() << endl;
  g1.playerinput(Move(1, 5));
  vector<Move> moves = g1.getboard().getvalidmoves();
  for (uint32_t i = 0 ; i < moves.size() ; ++i) {
    cout << "move: " << moves[i].x << ", " << moves[i].y << endl;
  }
  g1.getboard().quickdisplay();
  return 0;
}
