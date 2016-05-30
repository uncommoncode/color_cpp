#include <gtest/gtest.h>

#include "test_util.hpp"

#include <color/transformation.hpp>

namespace color {

TEST(Rgb888, Size) {
  rgb888_t rgb888;
  ASSERT_EQ(4, sizeof(rgb888));
}

TEST(Rgb888, Conversions) {
  rgb888_t rgb888;
  rgb888 = 0x00123456;
  uRgb urgb = to_urgb(rgb888);
  ASSERT_EQ(0x12, urgb.red);
  ASSERT_EQ(0x34, urgb.green);
  ASSERT_EQ(0x56, urgb.blue);
  ASSERT_EQ(rgb888, to_rgb888(urgb));

  sRgb srgb = to_srgb(rgb888);
  ASSERT_FLOAT_EQ(0x12 / 255.0f, srgb.red);
  ASSERT_FLOAT_EQ(0x34 / 255.0f, srgb.green);
  ASSERT_FLOAT_EQ(0x56 / 255.0f, srgb.blue);
  ASSERT_EQ(rgb888, to_rgb888(srgb));
}

TEST(uRgb, Conversions) {
  uRgb urgb;
  urgb.red = 0xdd;
  urgb.green = 0x1c;
  urgb.blue = 0x77;

  sRgb srgb = to_srgb(urgb);
  ASSERT_FLOAT_EQ(0xdd / 255.0f, srgb.red);
  ASSERT_FLOAT_EQ(0x1c / 255.0f, srgb.green);
  ASSERT_FLOAT_EQ(0x77 / 255.0f, srgb.blue);

  COLOR_ASSERT_EQ(urgb, to_urgb(srgb));

  rgb888_t rgb888;
  rgb888 = 0x00123456;
  urgb = to_urgb(rgb888);
  ASSERT_EQ(0x12, urgb.red);
  ASSERT_EQ(0x34, urgb.green);
  ASSERT_EQ(0x56, urgb.blue);
}

TEST(sRgb, Conversions) {
  sRgb srgb_black{0.0f, 0.0f, 0.0f};
  sRgb srgb_white{1.0f, 1.0f, 1.0f};

  rgb888_t rgb888_white = 0xffffff;
  rgb888_t rgb888_black = 0x000000;
  ASSERT_EQ(rgb888_white, to_rgb888(srgb_white));
  ASSERT_EQ(rgb888_black, to_rgb888(srgb_black));
  COLOR_ASSERT_FLOAT_EQ(srgb_white, to_srgb(rgb888_white));
  COLOR_ASSERT_FLOAT_EQ(srgb_black, to_srgb(rgb888_black));

  uRgb urgb_white{0xff, 0xff, 0xff};
  uRgb urgb_black{0x00, 0x00, 0x00};
  COLOR_ASSERT_EQ(urgb_white, to_urgb(srgb_white));
  COLOR_ASSERT_EQ(urgb_black, to_urgb(srgb_black));
  COLOR_ASSERT_EQ(srgb_white, to_srgb(urgb_white));
  COLOR_ASSERT_EQ(srgb_black, to_srgb(urgb_black));

  Hsl hsl_white{0.0f, 0.0f, 1.0f};
  Hsl hsl_black{0.0f, 0.0f, 0.0f};
  COLOR_ASSERT_FLOAT_EQ(hsl_white, to_hsl(srgb_white));
  COLOR_ASSERT_FLOAT_EQ(hsl_black, to_hsl(srgb_black));
  COLOR_ASSERT_FLOAT_EQ(srgb_white, to_srgb(hsl_white));
  COLOR_ASSERT_FLOAT_EQ(srgb_black, to_srgb(hsl_black));

  Hsv hsv_white{0.0f, 0.0f, 1.0f};
  Hsv hsv_black{0.0f, 0.0f, 0.0f};
  COLOR_ASSERT_FLOAT_EQ(hsv_white, to_hsv(srgb_white));
  COLOR_ASSERT_FLOAT_EQ(hsv_black, to_hsv(srgb_black));
  COLOR_ASSERT_FLOAT_EQ(srgb_white, to_srgb(hsv_white));
  COLOR_ASSERT_FLOAT_EQ(srgb_black, to_srgb(hsv_black));

  Xyz xyz_white{0.95047f, 1.0f, 1.08883f};
  Xyz xyz_black{0.0f, 0.0f, 0.0f};
  COLOR_ASSERT_FLOAT_EQ(xyz_white, to_xyz(srgb_white));
  COLOR_ASSERT_FLOAT_EQ(xyz_black, to_xyz(srgb_black));
  COLOR_ASSERT_FLOAT_EQ(srgb_white, to_srgb(xyz_white));
  COLOR_ASSERT_FLOAT_EQ(srgb_black, to_srgb(xyz_black));

  Lab lab_white{100.0f, 0.0f, 0.0f};
  Lab lab_black{0.0f, 0.0f, 0.0f};
  COLOR_ASSERT_FLOAT_EQ(lab_white, to_lab(to_xyz(srgb_white)));
  COLOR_ASSERT_FLOAT_EQ(lab_black, to_lab(to_xyz(srgb_black)));
  COLOR_ASSERT_FLOAT_EQ(srgb_white, to_srgb(to_xyz(lab_white)));
  COLOR_ASSERT_FLOAT_EQ(srgb_black, to_srgb(to_xyz(lab_black)));
}

TEST(Hsl, Conversions) {
  sRgb srgb = to_srgb(0x267fd9);
  Hsl hsl;
  hsl.hue = 0.583799f;
  hsl.saturation = 0.7019608f;
  hsl.lightness = 0.5f;
  COLOR_ASSERT_FLOAT_EQ(hsl, to_hsl(srgb));
  // TODO(emmett): there may be some loss of precision here in the HSL code that could be improved.
  COLOR_ASSERT_NEAR(srgb, to_srgb(hsl), 1.0e-6f);
}

TEST(Hsv, Conversions) {
  sRgb srgb = {89 / 255.0f, 134 / 255.0f, 178 / 255.0f};
  Hsv hsv;
  hsv.hue = 0.58239698f;
  hsv.saturation = 0.5f;
  hsv.value = 0.69803923f;
  COLOR_ASSERT_FLOAT_EQ(hsv, to_hsv(srgb));
  COLOR_ASSERT_FLOAT_EQ(srgb, to_srgb(hsv));
}

TEST(Xyz, Conversions) {
  sRgb srgb = to_srgb(0xff7f50);
  Xyz xyz;
  xyz.x = 0.50281972f;
  xyz.y = 0.37024003f;
  xyz.z = 0.12086333f;
  COLOR_ASSERT_FLOAT_EQ(xyz, to_xyz(srgb));
  COLOR_ASSERT_FLOAT_EQ(srgb, to_srgb(xyz));
}

TEST(Lab, Conversions) {
  sRgb srgb = to_srgb(0xf0e68c);
  Xyz xyz = {0.68967015f, 0.77014267f, 0.36038125f};
  COLOR_ASSERT_FLOAT_EQ(xyz, to_xyz(srgb));
  COLOR_ASSERT_FLOAT_EQ(srgb, to_srgb(xyz));
  Lab lab = {90.328178f, -9.0098085f, 44.979263f};
  // TODO(emmett): Values differ quite a bit from Linux and Mac here. It would be good to see why
  // numerically these change so much.
  COLOR_ASSERT_NEAR(lab, to_lab(xyz), 1.0e-4f);
  COLOR_ASSERT_FLOAT_EQ(xyz, to_xyz(lab));
}
}
