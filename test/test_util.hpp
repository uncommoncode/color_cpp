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

#define COLOR_ASSERT_NEAR(expected, actual, epsilon)                                               \
  {                                                                                                \
    ASSERT_NEAR(expected.values[0], actual.values[0], epsilon);                                    \
    ASSERT_NEAR(expected.values[1], actual.values[1], epsilon);                                    \
    ASSERT_NEAR(expected.values[2], actual.values[2], epsilon);                                    \
  }
