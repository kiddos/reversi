#ifndef AITRAINER_H
#define AITRAINER_H

#include <fstream>
#include <string>
#include <omp.h>
#include "game.h"
#include "nnet/nnet.h"

namespace reversi {

class AITrainer {
 public:
  AITrainer();
  void train(const int ninstances, const int iterations);
  void saveparam() const;

 private:
  double testaccuracy(const nn::mat result, const nn::mat answer);
  nn::NeuralNet nnet;
  Game* games;
  int nprocess;
};

} /* end of reversi namespace */

#endif /* end of include guard: AITRAINER_H */
