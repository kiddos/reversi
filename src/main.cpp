#include "window.h"
#include "aitrainer.h"

using reversi::Window;
using reversi::AITrainer;

int main(int argc, char *argv[]) {
  if (argc == 3) {
    const int instances = atoi(argv[1]);
    const int iterations = atoi(argv[2]);
    AITrainer trainer;
    trainer.train(instances, iterations);
    trainer.saveparam();
  } else {
    Window window;
    window.mainloop();
  }
  return 0;
}
