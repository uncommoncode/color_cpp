#pragma once

#include "space.hpp"

#include <vector>

namespace color {
	
// NOTE(emmett):  Use a vector rather than array because it would require templating or autoing all
// usages of a palette which sucks. The array length isn't a super relevant type trait here as long
// as functions use the .size() routine appropriately.
using Palette = std::vector<sRgb>;

// Create a palette from a rgb888_t array pointer and size.
Palette create_palette(const rgb888_t* data, std::size_t size);

// Create a palette from a fixed size rgb888_t array.
template <std::size_t Size> Palette create_palette(const rgb888_t(&values)[Size]) {
  return create_palette(values, Size);
}
	
} // namespace color
