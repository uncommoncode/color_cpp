#include "palette.hpp"

#include "transformation.hpp"

namespace color {
	
Palette create_palette(const color::rgb888_t* data, std::size_t size) {
  color::Palette palette;
  palette.reserve(size);
  for (std::size_t i = 0; i < size; i++) {
    palette.emplace_back(to_srgb(data[i]));
  }
  return palette;
}
	
} // namespace color
