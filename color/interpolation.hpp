#pragma once
#include <vector>

#include "palette.hpp"

namespace color {
	
// Perform nearest neighbor interpolation, picking the palette color closest to the point t.
sRgb interpolate_nearest_neighbor(const Palette& palette, float t);

// Interpolate linearly in the sRGB space  and return an sRgb color.
sRgb interpolate_linear(const Palette& palette, float t);

// Interpolate linearly in the HSV space and return an sRgb color.
sRgb interpolate_hsv_linear(const Palette& palette, float t);

// Interpolate linearly in the HSL space and return an sRgb color.
sRgb interpolate_hsl_linear(const Palette& palette, float t);

// Interpolate linearly in the Lab space and return an sRgb color.
sRgb interpolate_lab_linear(const Palette& palette, float t);

// Interpolate linearly in the XYZ space and return an sRgb color.
sRgb interpolate_xyz_linear(const Palette& palette, float t);
	
}
