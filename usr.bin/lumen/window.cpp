#include <gfx/font.h>
#include "internal.h"

#define TITLE_HEIGHT 18


static uint32_t theme_color() {
  // return 0xFFFFFF;
  // return 0xFFC9C9;  // poolside.fm
  return 0xACCED8;  // off-blue
  // return 0x858585;  // gray
}

lumen::window::window(int id, lumen::guest &g, int w, int h)
    : id(id), guest(g) {
  bitmap = ck::make_ref<gfx::shared_bitmap>(w, h);


  set_mode(window_mode::normal);
}

lumen::window::~window(void) {}

void lumen::window::set_mode(window_mode mode) {
  this->mode = mode;
  switch (mode) {
    case window_mode::normal:
      // no side borders
      this->rect.w = bitmap->width() + 2;
      this->rect.h = bitmap->height() + TITLE_HEIGHT + 2;
      break;
  }
}

void lumen::window::translate_invalidation(gfx::rect &r) {
  if (mode == window_mode::normal) {
    r.y += TITLE_HEIGHT;
  }
}


void lumen::window::handle_mouse_input(gfx::point &r, struct mouse_packet &p) {

	int x = 0;
	int y = 0;
  if (mode == window_mode::normal) {
    x = r.x() - 1;
    y = r.y() - TITLE_HEIGHT;
  }

	if (x >= 0 && x < bitmap->width() && y >= 0 && y < bitmap->height()) {

		struct lumen::input_msg m;

		m.window_id = this->id;
		m.type = LUMEN_INPUT_MOUSE;
		m.mouse.xpos = x;
		m.mouse.ypos = y;
		m.mouse.dx = p.dx;
		m.mouse.dy = p.dy;
		m.mouse.buttons = p.buttons;

		guest.send_msg(LUMEN_MSG_INPUT, m);
	}



}

gfx::rect lumen::window::bounds() { return rect; }


int noise(uint64_t seed) {
  seed = 6364136223846793005ULL * seed + 1;
  return seed >> 33;
}

void lumen::window::draw(gfx::scribe &scribe) {
  // draw normal window mode.
  if (mode == window_mode::normal) {
    // draw the window bitmap, offset by the title bar
    auto bmp_rect = gfx::rect(0, 0, bitmap->width(), bitmap->height());

    scribe.blit(gfx::point(1, TITLE_HEIGHT), *bitmap, bmp_rect);

    auto bg = theme_color();


    scribe.draw_frame(gfx::rect(0, 0, rect.w, TITLE_HEIGHT), bg);

    /*
if (hovered) {
}
    */


    auto st = gfx::scribe::text_thunk(4, 0, rect.w);
    scribe.draw_text(st, *gfx::font::get_default(), name.get(), 0x000000, 0);


    for (int i = 0; i < 5; i++) {
      int x = st.pos.x() + 4;
      int y = 4 + (i * 2);
      scribe.draw_line(x, y, rect.w - 10, y, 0);
    }


    scribe.draw_line(0, TITLE_HEIGHT, 0, rect.h - 2, 0x000000);
    scribe.draw_line(rect.w, TITLE_HEIGHT, rect.w, rect.h - 2, 0x000000);
    scribe.draw_line(0, rect.h - 2, rect.w, rect.h - 2, 0x000000);

    return;
  }
}
