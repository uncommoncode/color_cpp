#pragma once

#include <stdint.h>

namespace color {
	
typedef uint32_t rgb888_t;

struct uRgb {
  union {
    uint8_t values[3];
    struct {
      uint8_t red, green, blue;
    };
  };
};

struct sRgb {
  union {
    float values[3];
    struct {
      float red, green, blue;
    };
  };
};

struct Hsl {
  union {
    float values[3];
    struct {
      float hue, saturation, lightness;
    };
  };
};

struct Hsv {
  union {
    float values[3];
    struct {
      float hue, saturation, value;
    };
  };
};

struct Xyz {
  union {
    float values[3];
    struct {
      float x, y, z;
    };
  };
};

struct Lab {
  union {
    float values[3];
    struct {
      float lightness, a, b;
    };
  };
};

} // namespace color
