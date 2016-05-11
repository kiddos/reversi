#include "ai.h"

#ifdef DEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif

using std::vector;
using std::string;
using std::ifstream;
using nn::NeuralNet;
using nn::Layer;
using nn::InputLayer;
using nn::SoftmaxOutput;
using nn::mat;

namespace reversi {

const int AI::INPUT_NODES = 66;
const int AI::HIDDEN_LAYERS = 3;
const int AI::OUTPUT_NODES = 2;
const string AI::AI_PARAM_PATH = "./ai.param";

AI::AI() {
  srand(time(NULL));
  loaded = false;

  nnet = getmodel();
  ifstream input(AI_PARAM_PATH, std::ios::in);
  if (input.is_open()) {
#ifdef DEBUG
    cout << "loading model..." << endl;
#endif

    for (int i = 0 ; i < HIDDEN_LAYERS ; ++i) {
      mat w = nnet.gethidden(i).getw();
      for (uint32_t j = 0 ; j < w.n_rows ; ++j) {
        for (uint32_t k = 0 ; k < w.n_cols ; ++k) {
          double val = 0;
          input >> val;
          w(j, k) = val;
          nnet.gethidden(i).setw(w);
        }
      }
    }
    mat w = nnet.getoutput().getw();
    for (uint32_t j = 0 ; j < w.n_rows ; ++j) {
      for (uint32_t k = 0 ; k < w.n_cols ; ++k) {
        double val = 0;
        input >> val;
        w(j, k) = val;
      }
    }
    nnet.getoutput().setw(w);

    loaded = true;
    input.close();
  }
}

AI::AI(const AI& ai) : current(ai.current) {}

AI& AI::operator= (const AI& ai) {
  current = ai.current;
  return *this;
}

Move AI::decidemove(const Board current, bool smart, bool output) {
  this->current = current;
  vector<Move> moves = this->current.getvalidmoves();

  if (moves.size() == 0) {
    return Move(-1, -1);
  }

  if (smart && loaded) {
    mat samplemoves(moves.size(), INPUT_NODES);
    for (uint32_t i = 0 ; i < moves.size() ; ++i) {
      int *input = new int[INPUT_NODES];
      toinput(current, moves[i], current.getturn(), input);
      for (int j = 0 ; j < INPUT_NODES ; ++j) {
        samplemoves(i, j) = input[j];
      }
      delete input;
    }
    const mat prediction = nnet.predict(samplemoves);
    const mat result = nnet.getresult();
    int maxindex = 0;
    double maxval = result(0, 1);
    for (uint32_t i = 1 ; i < prediction.n_rows ; ++i) {
      if (result(i, 1) > maxval) {
        maxval = result(i, 1);
        maxindex = i;
      }
    }
#ifdef DEBUG
    cout << result << endl;
#endif

    return moves[maxindex];
  } else {
    const int index = rand() % moves.size();
    Move move = moves[index];

#ifdef DEBUG
    if (output) cout << "ai choose: " << move.x << ", " << move.y << endl;
#endif

    return move;
  }
}

NeuralNet AI::getmodel() {
  const double lambda = 1e-1;
  const double lrate = 1e-2;
  InputLayer input(INPUT_NODES);
  vector<Layer> hiddens = {
    Layer(INPUT_NODES, 100, lrate, lambda, nn::sigmoid),
    Layer(100, 50, lrate, lambda, nn::sigmoid),
    Layer(50, 25, lrate, lambda, nn::sigmoid)
  };
  SoftmaxOutput output(25, OUTPUT_NODES, lrate, lambda);
  NeuralNet nnet(input, output, hiddens);
  return nnet;
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
