#ifndef AI_H
#define AI_H

#include <cstdlib>
#include <ctime>

#include "board.h"
#include "nnet/nnet.h"

namespace reversi {

class AI {
 public:
  AI();
  AI(const AI& ai);
  AI& operator= (const AI& ai);
  Move decidemove(const Board current);

  static const int INPUT_NODES;
  static const int HIDDEN_NODES;
  static const int OUTPUT_NODES;
  static void toinput(const Board b, const Move m, const int turn, int data[66]);

 private:
  Board current;
};

}

#endif /* end of include guard: AI_H */
