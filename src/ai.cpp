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
using nn::QuadraticOutput;
using nn::mat;

namespace reversi {

const int AI::INPUT_NODES = 64;
const int AI::HIDDEN_LAYERS = 3;
const int AI::OUTPUT_NODES = 1;
const string AI::AI_PARAM_PATH = "./ai.param";

AI::AI() {
  srand(time(NULL));
  loaded = false;

  nnet = getmodel();
  loaded = loadparam(nnet);
}

AI::AI(const AI& ai) : current(ai.current) {}

AI& AI::operator= (const AI& ai) {
  current = ai.current;
  return *this;
}

Move AI::decidemove(const Board& current, bool smart, bool output) {
  this->current = current;
  vector<Move> moves = this->current.getvalidmoves();

  if (moves.size() == 0) {
    return Move(-1, -1);
  }

  if (smart) {
    int whichmove = 0;
    const double minval = minimax(current, whichmove, 6);
    Move m = moves[whichmove];

#ifdef DEBUG
    if (output)
      cout << "ai choose: " << minval <<
          "(" << m.x << "," << m.y << ")" << endl;
#endif

    return m;
  } else {
    const int index = rand() % moves.size();
    Move move = moves[index];

#ifdef DEBUG
    if (output) cout << "ai choose: " << move.x << ", " << move.y << endl;
#endif

    return move;
  }
}

double AI::min(vector<double>& values, vector<int>& index) {
  double minval = values[0];
  for (uint32_t i = 1 ; i < values.size() ; ++i) {
    if (values[i] < minval) {
      minval = values[i];
    }
  }
  for (uint32_t i = 0 ; i < values.size() ; ++i) {
    if (values[i] == minval) {
      index.push_back(i);
    }
  }
  return minval;
}

double AI::max(vector<double>& values, vector<int>& index) {
  double maxval = values[0];
  for (uint32_t i = 1 ; i < values.size() ; ++i) {
    if (values[i] > maxval) {
      maxval = values[i];
    }
  }
  for (uint32_t i = 0 ; i < values.size() ; ++i) {
    if (values[i] == maxval) {
      index.push_back(i);
    }
  }
  return maxval;
}

double AI::minimax(const Board& b, int& whichmove, int level) {
  if (level <= 0) {
    vector<Move> validmoves = b.getvalidmoves();
    if (validmoves.size() > 0) {
      vector<double> values;
      vector<int> index;
      for (uint32_t i = 0 ; i < validmoves.size() ; ++i) {
        Move m = validmoves[i];
        Board simboard = b;
        simboard.perform(m);
        values.push_back(eval(simboard));

        // simboard.quickdisplay();
        // cout << "level: " << level;
        // cout << " | score: " << values[i] << endl;
      }

      const double val = min(values, index);
      whichmove = index[rand() % index.size()];
      return val;
    }
  } else {
    vector<Move> validmoves = b.getvalidmoves();
    if (validmoves.size() > 0) {
      vector<double> values;
      vector<int> index;
      for (uint32_t i = 0 ; i < validmoves.size() ; ++i) {
        int nextmove = 0;
        Board simboard = b;
        Move m = validmoves[i];
        simboard.perform(m);
        values.push_back(minimax(simboard, nextmove, level-1));
      }

      double val = 0;
      if (level % 2 == 1) {
        val = max(values, index);
      } else {
        val = min(values, index);
      }
      whichmove = index[rand() % index.size()];
      return val;
    }
    return 0;
  }
  return eval(b);
}

NeuralNet AI::getmodel() {
  const double lambda = 1e-3;
  const double lrate = 1e-6;
  InputLayer input(INPUT_NODES);
  vector<Layer> hiddens = {
    Layer(INPUT_NODES, 100, lrate, lambda, nn::sigmoid),
    Layer(100, 50, lrate, lambda, nn::sigmoid),
    Layer(50, 25, lrate, lambda, nn::sigmoid)
  };
  QuadraticOutput output(25, OUTPUT_NODES, lrate, lambda);
  NeuralNet nnet(input, output, hiddens);
  return nnet;
}

void AI::toinput(const Board& b, const int turn, int data[64]) {
  // first 64 nodes are board state
  int board[BOARDSIZE][BOARDSIZE] = {{0}};
  b.copyto(board);
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
}

bool AI::loadparam(nn::NeuralNet& nnet) {
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

    input.close();
    return true;
  }
  return false;
}

double AI::eval(const Board& b) {
  return b.eval();
}

}
