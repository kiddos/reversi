#include "window.h"

#ifdef DEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_FPS 60.0
#define BOARD_PADDING 20
#define TOINT(n) static_cast<int>(n)

namespace reversi {

MCoord::MCoord(const int x, const int y, const int size)
    : X(x), Y(y), SIZE(size), BOX(1.0 * size / BOARDSIZE) {
  mx = my = 0;
}

MCoord::MCoord(const MCoord& mcoord)
    : MCoord(mcoord.X, mcoord.Y, mcoord.SIZE) {
  mx = mcoord.mx;
  my = mcoord.my;
}

MCoord& MCoord::operator= (const MCoord& mcoord) {
  mx = mcoord.mx;
  my = mcoord.my;
  return *this;
}

void MCoord::press(const int mx, const int my) {
  this->mx = mx;
  this->my = my;
}

bool MCoord::confirm(const int mx, const int my) {
  if (TOINT((mx - X) / BOX) == TOINT((this->mx - X) / BOX) &&
      TOINT((my - Y) / BOX) == TOINT((this->my - Y) / BOX)) {
    this->mx = mx;
    this->my = my;
    return true;
  }
  return false;
}

Move MCoord::getmove() const {
  return Move(TOINT((mx-X)/BOX), TOINT((my-Y)/BOX));
}

Window::Window() : WIDTH(WINDOW_WIDTH), HEIGHT(WINDOW_HEIGHT),
                   PADDING(BOARD_PADDING), FPS(WINDOW_FPS) {
  if (!al_init()) {
#ifdef DEBUG
    cout << "ERROR: fail to initialize allegro library" << endl;
#endif
  }
  if (!al_install_mouse()) {
#ifdef DEBUG
    cout << "ERROR: fail to initialize allegro mouse" << endl;
#endif
  }
  if (!(display = al_create_display(WIDTH, HEIGHT))) {
#ifdef DEBUG
    cout << "ERROR: fail to create allegro display" << endl;
#endif
  }
  if (!(timer = al_create_timer(1.0 / FPS))) {
#ifdef DEBUG
    cout << "ERROR: fail to create allegro timer" << endl;
#endif
  }
  if (!(equeue = al_create_event_queue())) {
#ifdef DEBUG
    cout << "ERROR: fail to create allegro event queue" << endl;
#endif
  }
  if (equeue && display)
    init();
}

void Window::init() {
  // register event sources
  al_register_event_source(equeue, al_get_display_event_source(display));
  al_register_event_source(equeue, al_get_timer_event_source(timer));
  al_register_event_source(equeue, al_get_mouse_event_source());
  // clear to color black
  bg = al_map_rgb(0, 0, 0);
  boardbg = al_map_rgb(57, 89, 191);
  boardline = al_map_rgb(19, 40, 107);
  whitedisc = al_map_rgb(230, 230, 230);
  blackdisc = al_map_rgb(20, 20, 20);

  al_clear_to_color(bg);
  al_flip_display();
}

void Window::render() {
  // draw the board
  const double SIZE = HEIGHT - 2 * PADDING;
  const double BOX = 1.0 * SIZE / BOARDSIZE;
  const double TO = PADDING + SIZE;
  al_draw_filled_rectangle(PADDING, PADDING, TO, TO, boardbg);
  for (int i = 0 ; i <= BOARDSIZE ; ++i) {
    const double shift = i * BOX + PADDING;
    al_draw_line(shift, PADDING, shift, TO, boardline, 3.0);
    al_draw_line(PADDING, shift, TO, shift, boardline, 3.0);
  }

  // draw the disc
  game.getboard().copyboard(dboard);
  const double DISCSIZE = BOX * 0.5 * 0.8;
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    const double shifty = i * BOX + PADDING + BOX / 2;
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      const double shiftx = j * BOX + PADDING + BOX / 2;
      if (dboard[i][j] == Board::WHITE) {
        al_draw_filled_circle(shiftx, shifty, DISCSIZE, whitedisc);
      } else if (dboard[i][j] == Board::BLACK) {
        al_draw_filled_circle(shiftx, shifty, DISCSIZE, blackdisc);
      }
    }
  }
}

bool Window::mainloop() {
  // start timer and rendering
  al_start_timer(timer);
  redraw = false;

  MCoord mouse(PADDING, PADDING, fmin(HEIGHT, WIDTH) - 2 * PADDING);

  while (true) {
    al_wait_for_event(equeue, &event);
    if (event.type == ALLEGRO_EVENT_TIMER) {
      redraw = true;
    } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      break;
    } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
      mouse.press(event.mouse.x, event.mouse.y);
    } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
      if (mouse.confirm(event.mouse.x, event.mouse.y)) {
#ifdef DEBUG
        cout << "mouse: " << mouse.getmove().x << ", " << mouse.getmove().y << endl;
#endif
        if (game.playerinput(mouse.getmove())) {
          // AI decision
          // may take long
          game.airespond();
        }

        if (game.isgameover()) {
          const int black = game.getboard().getblackcount();
          const int white = game.getboard().getwhitecount();
#ifdef DEBUG
          cout << "black: " << black << endl;
          cout << "white: " << white << endl;
#endif
        }
      }
    }

    if (redraw && al_is_event_queue_empty(equeue)) {
      redraw = false;

      al_clear_to_color(bg);
      render();
      al_flip_display();
    }
  }
  return true;
}

Window::~Window() {
  if (display) {
    al_destroy_display(display);
#ifdef DEBUG
    cout << "destroy allegro display." << endl;
#endif
  }
  if (timer) {
    al_destroy_timer(timer);
#ifdef DEBUG
    cout << "destroy allegro timer." << endl;
#endif
  }
  if (equeue) {
    al_destroy_event_queue(equeue);
#ifdef DEBUG
    cout << "destroy allegro event queue." << endl;
#endif
  }
}

#undef WINDOW_WIDTH
#undef WINDOW_HEIGHT
#undef WINDOW_FPS
#undef BOARD_PADDING
#undef TOINT

}
