#ifndef AI_H
#define AI_H

#include <fstream>
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
  Move decidemove(const Board& current, bool smart=false, bool output=false);

  static const int INPUT_NODES;
  static const int HIDDEN_LAYERS;
  static const int OUTPUT_NODES;
  static const std::string AI_PARAM_PATH;
  static nn::NeuralNet getmodel();

  static void toinput(const Board& b, int turn, int data[64]);
  static bool loadparam(nn::NeuralNet& nnet);
  static double eval(const Board& b);

 private:
  double minimax(const Board& current, int& whichmove, int level);
  Board current;
  nn::NeuralNet nnet;
  bool loaded;

};

}

#endif /* end of include guard: AI_H */
