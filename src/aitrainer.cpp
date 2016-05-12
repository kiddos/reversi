#include "aitrainer.h"

#ifdef DEBUG
#include <iostream>
using std::cout;
#endif

using std::string;
using std::ofstream;
using std::endl;
using std::vector;
using nn::NeuralNet;
using nn::mat;

namespace reversi {

AITrainer::AITrainer() {
  nprocess = 4;
  //ninstances = 1;
  games = new Game[nprocess];
  for (int i = 0 ; i < nprocess ; ++i) {
    games[i] = Game();
  }

  nnet = AI::getmodel();
}


void AITrainer::train(const int ninstances, const int iterations) {
  for (int ins = 0 ; ins < ninstances ; ++ins) {
    mat *data = new mat[nprocess];
    mat *label = new mat[nprocess];
    // prepare data
//#pragma omp parallel for default(none) shared(data, label)
    for (int p = 0 ; p < nprocess ; ++p) {
      Game game = games[p];
      // simulate the game
      game.quicksimulate(true);

      const int winner = game.getwinner();
      vector<Record> records = game.getrecords();
      mat trainingdata(records.size(), AI::INPUT_NODES);
      mat traininglabel(records.size(), AI::OUTPUT_NODES);
      traininglabel.zeros();

      // retreieve record
      for (uint32_t j = 0 ; j < records.size(); ++j) {
        int* input = new int[AI::INPUT_NODES];
        AI::toinput(records[j].state, records[j].move, records[j].turn, input);
        for (int k = 0 ; k < AI::INPUT_NODES ; ++k) {
          trainingdata(j, k) = input[k];
        }
        delete input;

        const int labelindex = (winner == records[j].turn) ? 1 : 0;
        traininglabel(j, labelindex) = 1;
      }

      data[p] = trainingdata;
      label[p] = traininglabel;

      games[p] = Game();
    }

    // training
    unsigned long total = 0;
    for (int p = 0 ; p < nprocess ; ++p) {
#ifdef DEBUG
      const mat result = nnet.predict(data[p]);
      const double accuracy = testaccuracy(result, label[p]);
      cout << endl << "accuracy: " << accuracy << endl;
#endif

      for (int i = 0 ; i < iterations ; ++i) {
        nnet.feeddata(data[p], label[p], false);
        total ++;

#ifdef DEBUG
        cout << "\rinstance: " << p << " | " << data[p].n_rows << " samples"
            << " | iteration: " << i << " | cost: " << nnet.computecost();
#endif
      }

#ifdef DEBUG
      if (total % data[p].n_rows == 0) {
        cout << endl << "instance: " << p << " | " << data[p].n_rows << " samples"
            << " | iteration: " << total
            << " | cost: " << nnet.computecost() << endl;
        const mat result = nnet.predict(data[p]);
        const double accuracy = testaccuracy(result, label[p]);
        cout << endl << "accuracy: " << accuracy << endl;
      }
#endif
    }
  }
}

void AITrainer::saveparam() const {
  ofstream output(AI::AI_PARAM_PATH, std::ios::out);
  for (int i = 0 ; i < AI::HIDDEN_LAYERS ; ++i) {
    mat w = nnet.gethidden(i).getw();
    for (uint32_t j = 0 ; j < w.n_rows ; ++j) {
      for (uint32_t k = 0 ; k < w.n_cols ; ++k) {
        output << w(j, k) << " ";
      }
    }
    output << endl;
  }
  mat w = nnet.getoutput().getw();
  for (uint32_t j = 0 ; j < w.n_rows ; ++j) {
    for (uint32_t k = 0 ; k < w.n_cols ; ++k) {
      output << w(j, k) << " ";
    }
  }
  output << endl;
  output.close();
}

double AITrainer::testaccuracy(const mat result, const mat answer) {
  double correct = 0;
  for (uint32_t i = 0 ; i < result.n_rows ; ++i) {
    if (answer(i, result(i, 0)) == 1) {
      correct ++;
    }
  }
  return correct / result.n_rows * 100;
}

}
