#include "ai.h"

#ifdef DEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif

using std::vector;

namespace reversi {

AI::AI() {
  srand(time(NULL));
}

Move AI::decidemove(const Board current) {
  this->current = current;
  vector<Move> moves = this->current.getvalidmoves();

  if (moves.size() == 0) {
    return Move(-1, -1);
  }

  const int index = rand() % moves.size();
  Move move = moves[index];

#ifdef DEBUG
  cout << "ai choose: " << move.x << ", " << move.y << endl;
#endif

  return move;
}

}
