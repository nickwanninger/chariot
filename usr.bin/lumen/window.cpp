#include <gfx/font.h>
#include <gfx/image.h>
#include "internal.h"

#define TITLE_HEIGHT 29
#define PADDING 0


#define BGCOLOR 0xF6F6F6
#define FGCOLOR 0xF0F0F0
#define BORDERCOLOR 0x000000

// #define TITLECOLOR 0x6261A1
#define TITLECOLOR 0xFFFFFF



static gfx::rect close_button() { return gfx::rect(4, 4, 9, 9); }


lumen::window::window(int id, lumen::guest &g, int w, int h) : id(id), guest(g) {
  bitmap = ck::make_ref<gfx::shared_bitmap>(w, h);


  set_mode(window_mode::normal);
}

lumen::window::~window(void) {}

void lumen::window::set_mode(window_mode mode) {
  this->mode = mode;

  this->rect.w = bitmap->width();
  this->rect.h = bitmap->height();
  return;

  switch (mode) {
    case window_mode::normal:
      // no side borders
      this->rect.w = bitmap->width();   // + (PADDING * 2);
      this->rect.h = bitmap->height();  //  + TITLE_HEIGHT + (PADDING * 2);
      break;
  }
}

void lumen::window::translate_invalidation(gfx::rect &r) {
  if (mode == window_mode::normal) {
    return;
  }
}


int lumen::window::handle_mouse_input(gfx::point &r, struct mouse_packet &p) {
  int x = 0;
  int y = 0;
  if (mode == window_mode::normal) {
    x = r.x();
    y = r.y();
  }

  if (y < TITLE_HEIGHT) {
    // this region is draggable
    return WINDOW_REGION_DRAG;
  }

  if (x >= 0 && x < (int)bitmap->width() && y >= 0 && y < (int)bitmap->height()) {
    struct lumen::input_msg m;

    m.window_id = this->id;
    m.type = LUMEN_INPUT_MOUSE;
    m.mouse.xpos = x;
    m.mouse.ypos = y;
    m.mouse.dx = p.dx;
    m.mouse.dy = p.dy;
    m.mouse.buttons = p.buttons;

    guest.send_msg(LUMEN_MSG_INPUT, m);
    return WINDOW_REGION_NORM;
  }


  return 0;
}




int lumen::window::handle_keyboard_input(keyboard_packet_t &p) {
  struct lumen::input_msg m;
  m.window_id = this->id;
  m.type = LUMEN_INPUT_KEYBOARD;
  m.keyboard.c = p.character;
  m.keyboard.flags = p.flags;
  m.keyboard.keycode = p.key;



  guest.send_msg(LUMEN_MSG_INPUT, m);
  return 0;
}



gfx::rect lumen::window::bounds() { return rect; }


constexpr int clamp(int val, int max, int min) {
  if (val > max) return max;
  if (val < min) return min;
  return val;
}

static constexpr uint32_t brighten(uint32_t color, float amt) {
  uint32_t fin = 0;
  for (int i = 0; i < 3; i++) {
    int off = i * 8;
    int c = (color >> off) & 0xFF;

    c = clamp(c * amt, 255, 0);
    fin |= c << off;
  }

  return fin;
}


void lumen::window::draw(gfx::scribe &scribe) {
  // draw normal window mode.
  if (mode == window_mode::normal) {
    // draw the window bitmap, offset by the title bar
    auto bmp_rect = bitmap->rect();

    scribe.blit(gfx::point(0, 0), *bitmap, bmp_rect);

// #define BUTTON_PADDING 4
    // gfx::rect button(0, 0, 21, 21);

    // button.x = rect.w - button.w;

    // button.shrink(1);
    // button.shrink(BUTTON_PADDING);

    // scribe.draw_frame(button, 0xFFFFFF, 0x666666);

  }

  return;
}
