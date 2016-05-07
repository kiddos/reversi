#ifndef WINDOW_H
#define WINDOW_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>

#include "game.h"

namespace reversi {

class MCoord {
 public:
  MCoord(const int x, const int y, const int size);
  MCoord(const MCoord& mcoord);
  MCoord& operator= (const MCoord& mcoord);

  void press(const int mx, const int my);
  bool confirm(const int mx, const int my);
  Move getmove() const;

 private:
  const int X, Y, SIZE;
  const double BOX;
  int mx, my;
};

class Window {
 public:
  Window();
  ~Window();
  void render();
  bool mainloop();

 private:
  void init();
  const int WIDTH, HEIGHT, PADDING;
  const double FPS;
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *equeue;
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT event;
  bool redraw;

  Game game;
  int dboard[8][8];

  ALLEGRO_COLOR bg;
  ALLEGRO_COLOR boardbg, boardline;
  ALLEGRO_COLOR whitedisc, blackdisc;
};

}

#endif /* end of include guard: WINDOW_H */
