#include "math.hpp"

#include <cmath>

namespace color {

namespace internal {
// These functions must be supported on your platform for color conversions.

float powf(float base, float exponent) { return std::pow(base, exponent); }

float roundf(float value) { return round(value); }

bool isnanf(float value) { return std::isnan(value); }

float floorf(float value) { return floor(value); }

float ceilf(float value) { return ceil(value); }

float modf(float value, float divisor) { return fmod(value, divisor); }

} // namespace internal

} // namespace color