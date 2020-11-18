#pragma once

#define max(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b;      \
  })


#define min(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b;      \
  })

#define unlikely(c) __builtin_expect((c), 0)

#include <ck/vec.h>


namespace gfx {

  struct rect final {
    // simple!
    int x, y, w, h;

    inline rect() { x = y = w = h = 0; }

    inline rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
    inline rect(int w, int h) : x(0), y(0), w(w), h(h) {}
    inline rect(const rect &o) { operator=(o); }

    inline rect &operator=(const rect &o) {
      x = o.x;
      y = o.y;
      w = o.w;
      h = o.h;
      return *this;
    }


    inline void set_location(int x, int y) {
      this->x = x;
      this->y = y;
    }

    inline int left() const { return x; }
    inline int right() const { return x + w - 1; }
    inline int top() const { return y; }
    inline int bottom() const { return y + h - 1; }


    inline int center_x(void) const { return x + w / 2; }

    inline int center_y(void) const { return y + h / 2; }


		ck::vec<gfx::rect, 4> shatter(const gfx::rect& hammer) const;

    inline void grow(int n = 1) {
      x -= n;
      y -= n;
      w += n * 2;
      h += n * 2;
    }

    inline void shrink(int n = 1) { grow(-n); }

    inline struct rect intersect(const struct rect &other) const {
      gfx::rect in;
      int l = max(left(), other.left());
      int r = min(right(), other.right());
      int t = max(top(), other.top());
      int b = min(bottom(), other.bottom());

      if (l > r || t > b) {
        return in;
      }

      in.x = l;
      in.y = t;
      in.w = (r - l) + 1;
      in.h = (b - t) + 1;

      return in;
    }


    inline bool is_empty() const { return w <= 0 || w <= 0; }

    inline bool intersects(const rect &other) const {
      return left() <= other.right() && other.left() <= right() && top() <= other.bottom() && other.top() <= bottom();
    }


    // is a rect fully within me?
    inline bool contains(const gfx::rect &other) {
      return left() <= other.left() && right() >= other.right() && top() <= other.top() && bottom() >= other.bottom();
    }

    inline bool contains(int x, int y) const { return x >= left() && x <= right() && y >= top() && y <= bottom(); }


    void center_within(const rect &other) {
      center_horizontally_within(other);
      center_vertically_within(other);
    }

    void center_horizontally_within(const rect &other) { x = other.center_x() - w / 2; }

    void center_vertically_within(const rect &other) { y = other.center_y() - h / 2; }
  };


};  // namespace gfx

#undef min
#undef max
