#pragma once
#include "space.hpp"

namespace color {
	
rgb888_t to_rgb888(uRgb urgb);
rgb888_t to_rgb888(sRgb srgb);

uRgb to_urgb(rgb888_t rgb888);
uRgb to_urgb(const sRgb& srgb);

Hsv to_hsv(const sRgb& srgb);
Hsl to_hsl(const sRgb& srgb);

sRgb to_srgb(rgb888_t rgb888);
sRgb to_srgb(uRgb urgb);
sRgb to_srgb(const Hsv& hsv);
sRgb to_srgb(const Hsl& hsl);
sRgb to_srgb(const Xyz& xyz);

Xyz to_xyz(const sRgb& srgb);
Xyz to_xyz(const Lab& lab);

Lab to_lab(const Xyz& xyz);
	
} // namespace color
