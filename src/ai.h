#ifndef AI_H
#define AI_H

#include <cstdlib>
#include <ctime>

#include "board.h"

namespace reversi {

class AI {
 public:
  AI();
  Move decidemove(const Board current);

 private:
  Board current;
};

}

#endif /* end of include guard: AI_H */
