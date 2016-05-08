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
#define ACOLOR(c) al_map_rgb(c.r, c.g, c.b)

namespace reversi {

MCoord::MCoord(const Rect r) : r(r), BOX(r.w / BOARDSIZE) {
  mx = my = 0;
}

MCoord::MCoord(const MCoord& mcoord)
    : MCoord(mcoord.r) {
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
  if (TOINT((mx - r.x) / BOX) == TOINT((this->mx - r.x) / BOX) &&
      TOINT((my - r.y) / BOX) == TOINT((this->my - r.y) / BOX)) {
    this->mx = mx;
    this->my = my;
    return true;
  }
  return false;
}

Move MCoord::getmove() const {
  return Move(TOINT((mx-r.x)/BOX), TOINT((my-r.y)/BOX));
}

ScorePanel::ScorePanel() : r(0, 0, 0, 0), font(nullptr) {
  memset(whitecount, '\0', 64);
  memset(blackcount, '\0', 64);
}

ScorePanel::ScorePanel(const Rect r, const int fontsize)
    : ScorePanel() {
  this->r = r;
  FONT_SIZE = fontsize;

  if (font) al_destroy_font(font);
  font = al_load_font("./font/Roboto.ttf", FONT_SIZE, 0);
  if (!font) {
#ifdef DEBUG
    cout << "ERROR: fail to load ttf font." << endl;
#endif
  }
}

ScorePanel::~ScorePanel() {
  if (font) {
    al_destroy_font(font);
#ifdef DEBUG
    cout << "destroy allegro font" << endl;
#endif
  }
}

ScorePanel::ScorePanel(const ScorePanel& panel)
    : ScorePanel(panel.r, panel.FONT_SIZE) {
  for (int i = 0 ; i < 64 ; ++i) {
    whitecount[i] = panel.whitecount[i];
    blackcount[i] = panel.blackcount[i];
  }
}

ScorePanel& ScorePanel::operator= (const ScorePanel& panel) {
  for (int i = 0 ; i < 64 ; ++i) {
    whitecount[i] = panel.whitecount[i];
    blackcount[i] = panel.blackcount[i];
  }

  r = panel.r;
  FONT_SIZE = panel.FONT_SIZE;

  if (font) al_destroy_font(font);
  font = al_load_font("./font/Roboto.ttf", FONT_SIZE, 0);
  if (!font) {
#ifdef DEBUG
    cout << "ERROR: fail to load ttf font." << endl;
#endif
  }
  return *this;
}

void ScorePanel::update(const int wcount, const int bcount) {
  memset(whitecount, '\0', 64);
  memset(blackcount, '\0', 64);

  snprintf(whitecount, 64, "white: %d", wcount);
  snprintf(blackcount, 64, "black: %d", bcount);
}

void ScorePanel::render() const {
  // border
  al_draw_filled_rectangle(r.x, r.y, r.x+r.w, r.y+r.h, ACOLOR(Window::BOARD_BG));
  al_draw_rectangle(r.x, r.y, r.x+r.w, r.y+r.h, ACOLOR(Window::BOARD_BORDER), 3.0);

  // text
  al_draw_text(font, ACOLOR(Window::WHITE_DISC),
               r.x+r.w/2, r.y+FONT_SIZE, ALLEGRO_ALIGN_CENTRE, whitecount);
  al_draw_text(font, ACOLOR(Window::BLACK_DISC),
               r.x+r.w/2, r.y+FONT_SIZE*4, ALLEGRO_ALIGN_CENTRE, blackcount);
}

const Color Window::BG = Color(0, 0, 0);
const Color Window::BOARD_BG = Color(57, 89, 191);
const Color Window::BOARD_BORDER = Color(19, 40, 107);
const Color Window::WHITE_DISC = Color(230, 230, 230);
const Color Window::BLACK_DISC = Color(20, 20, 20);

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
  al_init_font_addon();
  if (!al_init_ttf_addon()) {
#ifdef DEBUG
    cout << "ERROR: fail to initialize allegro ttf addon" << endl;
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

  // window component
  scorepanel = ScorePanel(Rect(HEIGHT, 200, 180, HEIGHT/3), 30);

  // clear to color black
  al_clear_to_color(ACOLOR(BG));
  al_flip_display();
}

void Window::render() {
  // draw the board
  const double SIZE = HEIGHT - 2 * PADDING;
  const double BOX = 1.0 * SIZE / BOARDSIZE;
  const double TO = PADDING + SIZE;
  al_draw_filled_rectangle(PADDING, PADDING, TO, TO, ACOLOR(Window::BOARD_BG));
  //al_draw_filled_rectangle(PADDING, PADDING, TO, TO, al_map_rgb(67, 89, 191));
  for (int i = 0 ; i <= BOARDSIZE ; ++i) {
    const double shift = i * BOX + PADDING;
    al_draw_line(shift, PADDING, shift, TO, ACOLOR(BOARD_BORDER), 3.0);
    al_draw_line(PADDING, shift, TO, shift, ACOLOR(BOARD_BORDER), 3.0);
  }

  // window component
  scorepanel.render();

  // draw the disc
  game.getboard().copyboard(dboard);
  const double DISCSIZE = BOX * 0.5 * 0.8;
  for (int i = 0 ; i < BOARDSIZE ; ++i) {
    const double shifty = i * BOX + PADDING + BOX / 2;
    for (int j = 0 ; j < BOARDSIZE ; ++j) {
      const double shiftx = j * BOX + PADDING + BOX / 2;
      if (dboard[i][j] == Board::WHITE) {
        al_draw_filled_circle(shiftx, shifty, DISCSIZE, ACOLOR(WHITE_DISC));
      } else if (dboard[i][j] == Board::BLACK) {
        al_draw_filled_circle(shiftx, shifty, DISCSIZE, ACOLOR(BLACK_DISC));
      }
    }
  }
}

void Window::update() {
  scorepanel.update(game.getboard().getwhitecount(),
                    game.getboard().getblackcount());
}

bool Window::mainloop() {
  // start timer and rendering
  al_start_timer(timer);
  redraw = false;

  const int SIZE = fmin(HEIGHT, WIDTH) - 2 * PADDING;
  MCoord mouse(Rect(PADDING, PADDING, SIZE, SIZE));

  while (true) {
    al_wait_for_event(equeue, &event);
    if (event.type == ALLEGRO_EVENT_TIMER) {
      redraw = true;
      update();
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
          game.saverecords();
          game.saverecords(false);
          break;
        }
      }
    }

    if (redraw && al_is_event_queue_empty(equeue)) {
      redraw = false;

      al_clear_to_color(ACOLOR(BG));
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
