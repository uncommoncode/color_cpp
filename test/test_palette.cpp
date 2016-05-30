#include <gtest/gtest.h>

#include "test_util.hpp"

#include <color/palette.hpp>

namespace color {
TEST(Palette, CreatePointerAndSize) {
  const int kSize = 3;
  const rgb888_t data[kSize] = {0xef8a62, 0xf7f7f7, 0x67a9cf};
  const sRgb colors[kSize] = {{0.937255f, 0.5411765f, 0.38431373f},
                              {0.96862745f, 0.96862745f, 0.96862745f},
                              {0.40392157f, 0.66274512f, 0.81176472f}};

  Palette palette = create_palette(data, kSize);
  ASSERT_EQ(kSize, palette.size());
  for (int i = 0; i < kSize; i++) {
    COLOR_ASSERT_FLOAT_EQ(colors[i], palette[i]);
  }
}

TEST(Palette, CreateArray) {
  const int kSize = 3;
  const rgb888_t data[kSize] = {0xef8a62, 0xf7f7f7, 0x67a9cf};
  const sRgb colors[kSize] = {{0.937255f, 0.5411765f, 0.38431373f},
                              {0.96862745f, 0.96862745f, 0.96862745f},
                              {0.40392157f, 0.66274512f, 0.81176472f}};

  Palette palette = create_palette(data);
  ASSERT_EQ(kSize, palette.size());
  for (int i = 0; i < kSize; i++) {
    COLOR_ASSERT_FLOAT_EQ(colors[i], palette[i]);
  }
}
}
