#include <errno.h>
#include <gfx/image.h>
#include <lumen.h>
#include <lumen/msg.h>
#include <string.h>
#include <unistd.h>
#include "internal.h"
uint32_t blend(uint32_t fgi, uint32_t bgi) {
  // only blend if we have to!
  if ((fgi & 0xFF000000) >> 24 == 0xFF) {
    return fgi;
  }

  uint32_t res = 0;
  auto result = (unsigned char *)&res;
  auto fg = (unsigned char *)&fgi;
  auto bg = (unsigned char *)&bgi;

  // spooky math follows
  uint32_t alpha = fg[3] + 1;
  uint32_t inv_alpha = 256 - fg[3];
  result[0] = (unsigned char)((alpha * fg[0] + inv_alpha * bg[0]) >> 8);
  result[1] = (unsigned char)((alpha * fg[1] + inv_alpha * bg[1]) >> 8);
  result[2] = (unsigned char)((alpha * fg[2] + inv_alpha * bg[2]) >> 8);
  result[3] = 0xff;

  return res;
}

void draw_bmp_scaled(ck::ref<gfx::bitmap> bmp, lumen::screen &screen, int xo,
                     int yo, float scale) {}


void draw_bmp(ck::ref<gfx::bitmap> bmp, lumen::screen &screen, int xo, int yo) {
  if (bmp) {
    for (size_t y = 0; y < bmp->height(); y++) {
      for (size_t x = 0; x < bmp->width(); x++) {
        uint32_t bp = bmp->get_pixel(x, y);
        uint32_t sp = screen.get_pixel(x + xo, y + yo);
        uint32_t pix = blend(bp, sp);
        screen.set_pixel(x + xo, y + yo, pix);
      }
    }
  }
}

lumen::context::context(void) : screen(1024, 768) {
  // clear the screen
  // memset(screen.pixels(), 0, screen.screensize());

  screen.clear(0xFF00FF);

  /*
auto test = gfx::load_png("/usr/res/misc/test.png");
draw_bmp(test, screen, 0, 0);
  */

  auto logo = gfx::load_png_shared("/usr/res/misc/cat.png");
  auto copy = gfx::shared_bitmap::get(logo->shared_name(), logo->width() * 2,
                                      logo->height() * 2);
  auto copy2 = gfx::shared_bitmap::get(logo->shared_name(), logo->width() / 2,
                                       logo->height() * 2);

  draw_bmp(logo, screen, 0, 0);
  draw_bmp(copy, screen, 0, logo->height());
  draw_bmp(copy2, screen, 0, copy->height());

  printf("done!\n");



  keyboard.open("/dev/keyboard", "r+");
  keyboard.on_read([this] {
    while (1) {
      keyboard_packet_t pkt;
      keyboard.read(&pkt, sizeof(pkt));
      if (errno == EAGAIN) break;
      handle_keyboard_input(pkt);
    }
  });


  mouse.open("/dev/mouse", "r+");
  mouse.on_read([this] {
    while (1) {
      struct mouse_packet pkt;
      mouse.read(&pkt, sizeof(pkt));
      if (errno == EAGAIN) break;
      handle_mouse_input(pkt);
      // printf("mouse: dx: %-3d dy: %-3d buttons: %02x\n", pkt.dx, pkt.dy,
      // pkt.buttons);
    }
  });


  server.listen("/usr/servers/lumen", [this] { accept_connection(); });
}


void lumen::context::handle_keyboard_input(keyboard_packet_t &pkt) {
  printf("keyboard: code: %02x ch: %02x (%c)\n", pkt.key, pkt.character,
         pkt.character);
}

void lumen::context::handle_mouse_input(struct mouse_packet &pkt) {
  printf("mouse: dx: %-3d dy: %-3d buttons: %02x\n", pkt.dx, pkt.dy,
         pkt.buttons);
}



void lumen::context::accept_connection() {
  auto id = next_guest_id++;
  // accept the connection
  auto *guest = server.accept();
  guests.set(id, new lumen::guest(id, *this, guest));
}

void lumen::context::guest_closed(long id) {
  auto c = guests[id];
  guests.remove(id);
  delete c;
}


#define HANDLE_TYPE(t, data_type)       \
  if (auto arg = (data_type *)msg.data; \
      msg.type == t && msg.len == sizeof(data_type))


void lumen::context::process_message(lumen::guest &c, lumen::msg &msg) {
  compose();  // XXX: remove me!
  HANDLE_TYPE(LUMEN_MSG_CREATE_WINDOW, lumen::create_window_msg) {
    (void)arg;

    ck::string name(arg->name, LUMEN_NAMESZ);

    printf("window wants to be made! ('%s', %dx%d)\n", name.get(), arg->width,
           arg->height);

    struct lumen::window_created_msg res;
    // TODO: figure out a better position to open to
    auto *window = c.new_window(name, arg->width, arg->height);
    if (window != NULL) {
      res.window_id = window->id;
      strncpy(res.bitmap_name, window->bitmap->shared_name(), LUMEN_NAMESZ - 1);
    } else {
      res.window_id = -1;
    }
    c.respond(msg, LUMEN_MSG_WINDOW_CREATED, res);
    return;
  }

  HANDLE_TYPE(LUMEN_MSG_GREET, lumen::greet_msg) {
    (void)arg;
    // responed to that
    struct lumen::greetback_msg res;
    res.magic = LUMEN_GREETBACK_MAGIC;
    res.guest_id = c.id;
    c.respond(msg, msg.type, res);
  };
}




void lumen::context::window_opened(lumen::window *w) {
  // TODO lock

  // [sanity check] make sure the window isn't already in the list :^)
  for (auto *e : windows) {
    if (w == e) {
      fprintf(stderr, "Window already exists!!\n");
      exit(EXIT_FAILURE);
    }
  }

  windows.insert(0, w);
  // TODO: set the focused window to the new one
}


void lumen::context::window_closed(lumen::window *w) {
  // TODO lock

  // Remove the window from our list
  for (int i = 0; i < windows.size(); i++) {
    if (windows[i] == w) {
      windows.remove(i);
      break;
    }
  }
}

void lumen::context::compose(void) {
	screen.clear(0xFFFFFF);
  for (auto *win : windows) {
		for (int y = 0; y < win->bitmap->height(); y++) {
			for (int x = 0; x < win->bitmap->width(); x++) {
				int ax = x + win->rect.x;
				int ay = y + win->rect.y;

				screen.set_pixel(ax, ay, win->bitmap->get_pixel(x, y));
			}
		}
  }
}




lumen::guest::guest(long id, struct context &ctx, ck::localsocket *conn)
    : id(id), ctx(ctx), connection(conn) {
  // printf("got a connection\n");

  connection->on_read([this] { this->on_read(); });
}

lumen::guest::~guest(void) {
  for (auto kv : windows) {
    printf("window '%s' (id: %d) removed due to guest being destructed!\n",
           kv.value->name.get(), kv.key);

    ctx.window_closed(kv.value);
    delete kv.value;
  }
  delete connection;
}

void lumen::guest::on_read(void) {
  bool failed = false;
  auto msgs = lumen::drain_messages(*connection, failed);
  // handle messages

  for (auto *msg : msgs) {
    process_message(*msg);
    free(msg);
  }

  // if the guest is at EOF *or* it otherwise failed, consider it
  // disconnected
  if (connection->eof() || failed) {
    this->ctx.guest_closed(id);
  }
}



struct lumen::window *lumen::guest::new_window(ck::string name, int w, int h) {
  auto id = next_window_id++;
  auto win = new lumen::window(id, *this, w, h);
  win->name = name;

	// XXX: allow the user to request a position
	win->rect.x = rand() % (ctx.screen.width() - w);
	win->rect.y = rand() % (ctx.screen.height() - h);

  printf("guest %d made a new window, '%s'\n", this->id, name.get());
  windows.set(win->id, win);


  ctx.window_opened(win);
  return win;
}

void lumen::guest::process_message(lumen::msg &msg) {
  // defer to the window server's function
  ctx.process_message(*this, msg);
}


long lumen::guest::send_raw(int type, int id, void *payload,
                            size_t payloadsize) {
  size_t msgsize = payloadsize + sizeof(lumen::msg);
  auto msg = (lumen::msg *)malloc(msgsize);

  msg->magic = LUMEN_MAGIC;
  msg->type = type;
  msg->id = id;
  msg->len = payloadsize;

  if (payloadsize > 0) memcpy(msg + 1, payload, payloadsize);

  auto w = connection->write((const void *)msg, msgsize);

  free(msg);
  return w;
}