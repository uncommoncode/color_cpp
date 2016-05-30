#pragma once

#include <utility>

namespace color {

namespace internal {
bool isnanf(float value);
float modf(float value, float divisor);
float roundf(float value);
float floorf(float value);
float ceilf(float value);
float powf(float base, float exponent);

using Vector3f = float[3];

template <int row> inline constexpr int const_matrix_row_offset() { return row * 3; }

template <class Matrix, int row> inline constexpr float const_vector_dot(const Vector3f vector) {
  // Force accumulation in a double to minimize floating point summation errors.
  return float(double(Matrix::values[0 + const_matrix_row_offset<row>()] * vector[0]) +
               double(Matrix::values[1 + const_matrix_row_offset<row>()] * vector[1]) +
               double(Matrix::values[2 + const_matrix_row_offset<row>()] * vector[2]));
}

template <class Matrix> inline void const_matrix_multiply(const Vector3f input, Vector3f output) {
  output[0] = const_vector_dot<Matrix, 0>(input);
  output[1] = const_vector_dot<Matrix, 1>(input);
  output[2] = const_vector_dot<Matrix, 2>(input);
}

template <typename Type> constexpr Type abs(Type value) { return value < 0 ? -value : value; }

inline float clampf(float value) { return (value < 0.0f) ? 0.0f : ((value > 1.0f) ? 1.0f : value); }

template <int exponent> constexpr float const_powf(float value) {
  return value * const_powf<exponent - 1>(value);
}

template <> constexpr float const_powf<0>(float value) { return 1.0f; }

enum class Comparator { Min, Max };

template <Comparator comparator, typename Type> struct Compare {};

template <typename Type> struct Compare<Comparator::Min, Type> {
  static inline Type compare(Type a, Type b) { return a < b ? a : b; }
};

template <typename Type> struct Compare<Comparator::Max, Type> {
  static inline Type compare(Type a, Type b) { return a > b ? a : b; }
};

template <Comparator comparator, typename Type> Type cmp(Type a) { return a; }

template <Comparator comparator, typename Type> Type cmp(Type a, Type b) {
  return Compare<comparator, Type>::compare(a, b);
}

template <Comparator comparator, typename Type, typename... Args> Type cmp(Type a, Args&&... args) {
  return cmp<comparator, Type>(a, cmp<comparator, Type>(std::forward<Args>(args)...));
}

template <typename Type, typename... Args> Type min(Type a, Args&&... args) {
  return cmp<Comparator::Min, Type>(a, std::forward<Args>(args)...);
}

template <typename Type, typename... Args> Type max(Type a, Args&&... args) {
  return cmp<Comparator::Max, Type>(a, std::forward<Args>(args)...);
}

template <typename FunctionType>
void vector3f_map(const Vector3f input, Vector3f output, FunctionType function) {
  for (int i = 0; i < 3; i++) {
    output[i] = function(input[i]);
  }
}

} // namespace internal

} // namespace color
