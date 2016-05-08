#include "ai.h"

#ifdef DEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif

using std::vector;

namespace reversi {

const int AI::INPUT_NODES = 66;
const int AI::HIDDEN_NODES = 360;
const int AI::OUTPUT_NODES = 1;

AI::AI() {
  srand(time(NULL));
}

AI::AI(const AI& ai) : current(ai.current) {}

AI& AI::operator= (const AI& ai) {
  current = ai.current;
  return *this;
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

void AI::toinput(const Board b, const Move m, const int turn, int data[66]) {
  // first 64 nodes are board state
  int board[BOARDSIZE][BOARDSIZE] = {{0}};
  b.copyboard(board);
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      if (board[i][j] == turn) {
        data[i*BOARDSIZE+j] = 1;
      } else if (board[i][j] == Board::EMPTY) {
        data[i*BOARDSIZE+j] = 0;
      } else {
        data[i*BOARDSIZE+j] = -1;
      }
    }
  }

  // last 2 nodes is next move
  data[INPUT_NODES-2] = m.x;
  data[INPUT_NODES-1] = m.y;
}

}
