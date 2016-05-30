#include "interpolation.hpp"

#include "internal/math.hpp"
#include "space.hpp"
#include "transformation.hpp"

namespace color {
	
sRgb interpolate_nearest_neighbor(const Palette& palette, float t) {
  const float indexf = (palette.size() - 1) * t;
  const int i = internal::roundf(indexf);
  return palette[i];
}

template <typename Type> struct Converter {};

template <> struct Converter<Hsv> {
  static Hsv from(const sRgb& srgb) { return to_hsv(srgb); }
};

template <> struct Converter<Hsl> {
  static Hsl from(const sRgb& srgb) { return to_hsl(srgb); }
};

template <> struct Converter<Xyz> {
  static Xyz from(const sRgb& srgb) { return to_xyz(srgb); }
};

template <> struct Converter<Lab> {
  static Lab from(const sRgb& srgb) { return to_lab(to_xyz(srgb)); }
};

template <> struct Converter<sRgb> {
  static sRgb from(const Hsv& hsv) { return to_srgb(hsv); }

  static sRgb from(const Hsl& hsl) { return to_srgb(hsl); }

  static sRgb from(const sRgb& srgb) { return srgb; }
};

template <typename ColorSpaceType>
ColorSpaceType interpolate_color_space_linear(const Palette& palette, float t) {
  const float indexf = (palette.size() - 1) * t;
  const int i_0 = int(internal::floorf(indexf));
  const int i_1 = int(internal::ceilf(indexf));

  const float remainder = indexf - i_0;

  ColorSpaceType p0 = Converter<ColorSpaceType>::from(palette[i_0]);
  ColorSpaceType p1 = Converter<ColorSpaceType>::from(palette[i_1]);
  ColorSpaceType lerp;

  for (int i = 0; i < 3; i++) {
    lerp.values[i] = p0.values[i] * (1.0f - remainder) + p1.values[i] * remainder;
  }

  return lerp;
}

sRgb interpolate_linear(const Palette& palette, float t) {
  return interpolate_color_space_linear<sRgb>(palette, t);
}

sRgb interpolate_hsv_linear(const Palette& palette, float t) {
  return to_srgb(interpolate_color_space_linear<Hsv>(palette, t));
}

sRgb interpolate_hsl_linear(const Palette& palette, float t) {
  return to_srgb(interpolate_color_space_linear<Hsl>(palette, t));
}

sRgb interpolate_xyz_linear(const Palette& palette, float t) {
  return to_srgb(interpolate_color_space_linear<Xyz>(palette, t));
}

sRgb interpolate_lab_linear(const Palette& palette, float t) {
  return to_srgb(to_xyz(interpolate_color_space_linear<Lab>(palette, t)));
}
	
} // namespace color