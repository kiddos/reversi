#ifndef WINDOW_H
#define WINDOW_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <string.h>

#include "game.h"

namespace reversi {

struct Rect {
  double x, y, w, h;
  Rect(const double x, const double y, const double w, const double h)
      : x(x), y(y), w(w), h(h) {}
};

struct Color {
  int r, g, b;
  Color(int r, int g, int b) : r(r), g(g), b(b) {}
  Color(const Color& c) : r(c.r), g(c.g), b(c.b) {}
  Color& operator= (const Color& c) {r=c.r;g=c.g;b=c.b; return *this;}
};

class MCoord {
 public:
  explicit MCoord(const Rect r);
  MCoord(const MCoord& mcoord);
  MCoord& operator= (const MCoord& mcoord);

  void press(const int mx, const int my);
  bool confirm(const int mx, const int my);
  Move getmove() const;

 private:
  const Rect r;
  const double BOX;
  int mx, my;
};

class ScorePanel {
 public:
  ScorePanel();
  ScorePanel(const Rect r, const int fontsize);
  ScorePanel(const ScorePanel& panel);
  ScorePanel& operator= (const ScorePanel& panel);
  ~ScorePanel();

  void update(const int wcount, const int bcount);
  void render() const;

 private:
  Rect r;
  int FONT_SIZE;

  ALLEGRO_FONT* font;
  char whitecount[64];
  char blackcount[64];
};

class Window {
 public:
  Window();
  ~Window();
  void render();
  void update();
  bool mainloop();

  static const Color BG, BOARD_BG, BOARD_BORDER;
  static const Color WHITE_DISC, BLACK_DISC;

 private:
  void init();
  const int WIDTH, HEIGHT, PADDING;
  const double FPS;

  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *equeue;
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT event;
  bool redraw;

  ScorePanel scorepanel;
  Game game;
  int dboard[8][8];
};

}

#endif /* end of include guard: WINDOW_H */
