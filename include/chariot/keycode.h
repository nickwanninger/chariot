#pragma once


enum keycode : unsigned char {
  key_invalid = 0,
  key_escape,
  key_tab,
  key_backspace,
  key_return,
  key_insert,
  key_delete,
  key_printscreen,
  key_sysrq,
  key_home,
  key_end,
  key_left,
  key_up,
  key_right,
  key_down,
  key_pageup,
  key_pagedown,
  key_leftshift,
  key_rightshift,
  key_control,
  key_alt,
  key_capslock,
  key_numlock,
  key_scrolllock,
  key_f1,
  key_f2,
  key_f3,
  key_f4,
  key_f5,
  key_f6,
  key_f7,
  key_f8,
  key_f9,
  key_f10,
  key_f11,
  key_f12,
  key_space,
  key_exclamationpoint,
  key_doublequote,
  key_hashtag,
  key_dollar,
  key_percent,
  key_ampersand,
  key_apostrophe,
  key_leftparen,
  key_rightparen,
  key_asterisk,
  key_plus,
  key_comma,
  key_minus,
  key_period,
  key_slash,
  key_0,
  key_1,
  key_2,
  key_3,
  key_4,
  key_5,
  key_6,
  key_7,
  key_8,
  key_9,
  key_colon,
  key_semicolon,
  key_lessthan,
  key_equal,
  key_greaterthan,
  key_questionmark,
  key_atsign,
  key_a,
  key_b,
  key_c,
  key_d,
  key_e,
  key_f,
  key_g,
  key_h,
  key_i,
  key_j,
  key_k,
  key_l,
  key_m,
  key_n,
  key_o,
  key_p,
  key_q,
  key_r,
  key_s,
  key_t,
  key_u,
  key_v,
  key_w,
  key_x,
  key_y,
  key_z,
  key_leftbracket,
  key_rightbracket,
  key_backslash,
  key_circumflex,
  key_underscore,
  key_leftbrace,
  key_rightbrace,
  key_pipe,
  key_tilde,
  key_backtick,
  key_logo,

  key_shift = key_leftshift,
};

enum keymodifier {
  mod_none = 0x00,
  mod_alt = 0x01,
  mod_ctrl = 0x02,
  mod_shift = 0x04,
  mod_logo = 0x08,
  mod_mask = 0x0f,

  is_pressed = 0x80,
};

#define KEY_EVENT_MAGIC 0xFFF8

struct keyboard_packet_t {
  unsigned short magic;
  keycode key{key_invalid};
  unsigned char character{0};
  unsigned char flags{0};
  bool alt() const {
    return flags & mod_alt;
  }
  bool ctrl() const {
    return flags & mod_ctrl;
  }
  bool shift() const {
    return flags & mod_shift;
  }
  bool logo() const {
    return flags & mod_logo;
  }
  unsigned modifiers() const {
    return flags & mod_mask;
  }
  bool is_press() const {
    return flags & is_pressed;
  }
};
