#pragma once

#define COLOR_ASSERT_EQ(expected, actual)                                                          \
  {                                                                                                \
    ASSERT_EQ(expected.values[0], actual.values[0]);                                               \
    ASSERT_EQ(expected.values[1], actual.values[1]);                                               \
    ASSERT_EQ(expected.values[2], actual.values[2]);                                               \
  }

#define COLOR_ASSERT_FLOAT_EQ(expected, actual)                                                    \
  {                                                                                                \
    ASSERT_FLOAT_EQ(expected.values[0], actual.values[0]);                                         \
    ASSERT_FLOAT_EQ(expected.values[1], actual.values[1]);                                         \
    ASSERT_FLOAT_EQ(expected.values[2], actual.values[2]);                                         \
  }

// This epsilon indicates the maximum number of significant figures for accurate color comparasons
// in sRGB.
#define NEAR_EPSILON 1.0e-6f

#define COLOR_ASSERT_NEAR(expected, actual)                                                        \
  {                                                                                                \
    ASSERT_NEAR(expected.values[0], actual.values[0], NEAR_EPSILON);                               \
    ASSERT_NEAR(expected.values[1], actual.values[1], NEAR_EPSILON);                               \
    ASSERT_NEAR(expected.values[2], actual.values[2], NEAR_EPSILON);                               \
  }
