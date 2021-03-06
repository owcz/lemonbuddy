#pragma once

#include "config.hpp"

#ifndef ENABLE_RANDR_EXT
#error "RandR extension is disabled..."
#endif

#include "common.hpp"
#include "utils/memory.hpp"
#include "x11/connection.hpp"

LEMONBUDDY_NS

struct backlight_values {
  uint32_t atom = 0;
  uint32_t min = 0;
  uint32_t max = 0;
  uint32_t val = 0;
};

struct randr_output {
  string name;
  uint16_t w = 0;
  uint16_t h = 0;
  int16_t x = 0;
  int16_t y = 0;
  xcb_randr_output_t output;
  backlight_values backlight;

  /**
   * Workaround for the inconsistent naming
   * of outputs between my intel and nvidia
   * drivers (xf86-video-intel drops the dash)
   */
  bool match(const string& o, bool strict = false) const {
    if (strict && name != o)
      return false;
    return name == o || name == string_util::replace(o, "-", "");
  }
};

using monitor_t = shared_ptr<randr_output>;

namespace randr_util {
  monitor_t make_monitor(xcb_randr_output_t randr, string name, uint16_t w, uint16_t h, int16_t x, int16_t y);
  vector<monitor_t> get_monitors(connection& conn, xcb_window_t root, bool connected_only = false);

  void get_backlight_range(connection& conn, const monitor_t& mon, backlight_values& dst);
  void get_backlight_value(connection& conn, const monitor_t& mon, backlight_values& dst);
}

LEMONBUDDY_NS_END
