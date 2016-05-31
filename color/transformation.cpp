#include "transformation.hpp"

#include "internal/math.hpp"

namespace color {

// Defined for the CIE 1931 2 degree Standard Illuminant D65.
// D65 is the most standard illuminant outside of D50 within printing, representing an "average noon
// daylight from the northern sky."
//
// Given the xyY coordinates for D65:
//  x = 0.312727
//  y = 0.329023
//  Y = 1.0        (because the lightness is 1 for white)
// You can convert to XYZ:
//  X = x * Y / y = x / y
//  Y = Y = 1
//  Z = (1 - x - y) * Y / y = (1 - x - y) / y
//
// NOTE: Others seem to round these values to only a few significant figures, perhaps due to copy
// and pasting.
// Philosophically we are chosing accuracy over constants found in other code bases.
// It seems that having exact values here cause chromacity coordinates in L*a*b* to be non-zero with
// a white point, suggesting that some of the conversion code is more sensitive to these illuminant
// values than meets the eye.
//
// The values here happen to match what D3's implementation uses converting to Lab.
//
// References:
//  https://en.wikipedia.org/wiki/Illuminant_D65
//  https://en.wikipedia.org/wiki/Talk:Illuminant_D65
//  https://www.w3.org/Graphics/Color/srgb
//  https://en.wikipedia.org/wiki/SRGB
//  https://engineering.purdue.edu/~bouman/ece637/notes/pdf/ColorSpaces.pdf
//  http://www.easyrgb.com/index.php?X=MATH&H=15
//  https://github.com/d3/d3-color/blob/v0.4.2/src/lab.js
//  http://www.babelcolor.com/index_htm_files/A%20review%20of%20RGB%20color%20spaces.pdf
//  https://www.konicaminolta.eu/fileadmin/content/eu/Measuring_Instruments/4_Learning_Centre/L_D/Light_sources_and_illuminants/Apps_Note_1_-_Light_sources_and_illuminants.pdf
static const constexpr Xyz kWhitePointD65 = {0.95047f, 1.0f, 1.08883f};

// Convert from XYZ tristimulus values with a D65 reference whitepoint to linear sRGB.
// References:
//  http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
//  https://www.w3.org/Graphics/Color/srgb
struct XyzToRgbMatrix {
  static constexpr const float values[9] = {3.2404542f,  -1.5371385f, -0.4985314f,
                                            -0.9692660f, 1.8760108f,  0.0415560f,
                                            0.0556434f,  -0.2040259f, 1.0572252f};
};

constexpr const float XyzToRgbMatrix::values[];

// Convert from linear sRGB to XYZ tristimulus values with a D65 reference whitepoint.
// References:
//  http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
//  https://www.w3.org/Graphics/Color/srgb
struct RgbToXyzMatrix {
  static constexpr const float values[9] = {0.4124564f, 0.3575761f, 0.1804375f,
                                            0.2126729f, 0.7151522f, 0.0721750f,
                                            0.0193339f, 0.1191920f, 0.9503041f};
};

constexpr const float RgbToXyzMatrix::values[];

static const float kXyzGammaA = 0.055f;
static const float kXyzGammaExp = 2.4f;

inline float xyz_linear_value_to_s_value(float value) {
  static const float exponent = 1.0f / kXyzGammaExp;

  return internal::clampf(
      (value <= 0.0031308f) ? (12.92f * value)
                            : ((1.0f + kXyzGammaA) * internal::powf(value, exponent) - kXyzGammaA));
}

inline float xyz_s_value_to_linear_value(float value) {
  return (value <= 0.04045f)
             ? (value / 12.92f)
             : (internal::powf((value + kXyzGammaA) / (1.0f + kXyzGammaA), kXyzGammaExp));
}

sRgb to_srgb(const Xyz& xyz) {
  internal::Vector3f rgb_linear;
  internal::const_matrix_multiply<XyzToRgbMatrix>(xyz.values, rgb_linear);
  sRgb srgb;
  internal::vector3f_map(rgb_linear, srgb.values, xyz_linear_value_to_s_value);
  return srgb;
}

Xyz to_xyz(const sRgb& srgb) {
  internal::Vector3f rgb_linear;
  internal::vector3f_map(srgb.values, rgb_linear, xyz_s_value_to_linear_value);
  Xyz xyz;
  internal::const_matrix_multiply<RgbToXyzMatrix>(rgb_linear, xyz.values);
  return xyz;
}

static const float kLabDivision = 6.0f / 29.0f;
static const float kLabOffset = 4.0f / 29.0f;

// TODO(emmett): Looks like there could be some numerical issues here. Explore higher precision or
// other manipulations for better numerical stability.
float lab_nonlinearity(float value) {
  if (value > internal::const_powf<3>(kLabDivision)) {
    return internal::powf(value, 1.0f / 3.0f);
  } else {
    return value * (internal::const_powf<2>(1.0f / kLabDivision) / 3.0f) + kLabOffset;
  }
}

float lab_inverse_nonlinearity(float value) {
  if (value > kLabDivision) {
    return internal::powf(value, 3.0f);
  } else {
    return (3.0f * internal::const_powf<2>(kLabDivision)) * (value - kLabOffset);
  }
}

Lab to_lab(const Xyz& xyz) {
  Lab lab;
  const float xp = lab_nonlinearity(xyz.x / kWhitePointD65.x);
  const float yp = lab_nonlinearity(xyz.y / kWhitePointD65.y);
  const float zp = lab_nonlinearity(xyz.z / kWhitePointD65.z);

  lab.lightness = 116.0f * yp - 16.0f;
  lab.a = 500.0f * (xp - yp);
  lab.b = 200.0f * (yp - zp);
  return lab;
}

Xyz to_xyz(const Lab& lab) {
  Xyz xyz;
  const float scaled_lightness = (lab.lightness + 16.0f) / 116.0f;
  const float x_offset = internal::isnanf(lab.a) ? 0.0f : lab.a / 500.0f;
  xyz.x = kWhitePointD65.x * lab_inverse_nonlinearity(scaled_lightness + x_offset);
  xyz.y = kWhitePointD65.y * lab_inverse_nonlinearity(scaled_lightness);
  const float z_offset = internal::isnanf(lab.b) ? 0.0f : lab.b / 200.0f;
  xyz.z = kWhitePointD65.z * lab_inverse_nonlinearity(scaled_lightness - z_offset);
  return xyz;
}

template <typename HscType> sRgb hsc_to_srgb(const HscType& hsc, float chroma, float dv) {
  const float hue_p = hsc.hue * 360.0f / 60.0f;
  const float t = (1.0f - internal::abs(internal::modf(hue_p, 2.0f) - 1.0f));
  const float x = chroma * t;
  internal::Vector3f rgb_hue_chroma{0.0f};

  const int segment = int(internal::floorf(hue_p));
  switch (segment) {
  case 0:
    rgb_hue_chroma[0] = chroma;
    rgb_hue_chroma[1] = x;
    rgb_hue_chroma[2] = 0.0f;
    break;
  case 1:
    rgb_hue_chroma[0] = x;
    rgb_hue_chroma[1] = chroma;
    rgb_hue_chroma[2] = 0.0f;
    break;
  case 2:
    rgb_hue_chroma[0] = 0.0f;
    rgb_hue_chroma[1] = chroma;
    rgb_hue_chroma[2] = x;
    break;
  case 3:
    rgb_hue_chroma[0] = 0.0f;
    rgb_hue_chroma[1] = x;
    rgb_hue_chroma[2] = chroma;
    break;
  case 4:
    rgb_hue_chroma[0] = x;
    rgb_hue_chroma[1] = 0.0f;
    rgb_hue_chroma[2] = chroma;
    break;
  case 5:
    rgb_hue_chroma[0] = chroma;
    rgb_hue_chroma[1] = 0.0f;
    rgb_hue_chroma[2] = x;
    break;
  }

  sRgb srgb;
  internal::vector3f_map(rgb_hue_chroma, srgb.values,
                         [dv](float value) -> float { return value + dv; });
  return srgb;
}

rgb888_t to_rgb888(sRgb srgb) {
  rgb888_t rgb888 = ((uint8_t(srgb.red * 0xff) & 0xff) << 16) |
                    ((uint8_t(srgb.green * 0xff) & 0xff) << 8) |
                    ((uint8_t(srgb.blue * 0xff) & 0xff) << 0);
  return rgb888;
}

rgb888_t to_rgb888(uRgb urgb) {
  rgb888_t rgb888 = (urgb.red << 16) | (urgb.green << 8) | (urgb.blue << 0);
  return rgb888;
}

uRgb to_urgb(rgb888_t rgb888) {
  uRgb urgb;
  urgb.red = (rgb888 >> 16) & 0xff;
  urgb.green = (rgb888 >> 8) & 0xff;
  urgb.blue = (rgb888 >> 0) & 0xff;
  return urgb;
}

uRgb to_urgb(const sRgb& srgb) {
  uRgb urgb;
  urgb.red = int(internal::roundf(srgb.red * 255.0f)) & 0xff;
  urgb.green = int(internal::roundf(srgb.green * 255.0f)) & 0xff;
  urgb.blue = int(internal::roundf(srgb.blue * 255.0f)) & 0xff;
  return urgb;
}

template <typename HscType, typename Finisher>
HscType srgb_to_hsc(const sRgb& srgb, Finisher finisher) {
  // Determine the hue of a color.
  // https://en.wikipedia.org/wiki/HSL_and_HSV#Hue_and_chroma
  HscType hsc;

  const float min = internal::min(srgb.red, srgb.green, srgb.blue);
  const float max = internal::max(srgb.red, srgb.green, srgb.blue);

  const float dv = max - min;
  if (dv == 0.0f) {
    hsc.hue = 0.0;
  } else if (srgb.red == max) {
    hsc.hue = internal::modf((srgb.green - srgb.blue) / dv, 6.0f);
  } else if (srgb.green == max) {
    hsc.hue = 2.0f + (srgb.blue - srgb.red) / dv;
  } else {
    hsc.hue = 4.0f + (srgb.red - srgb.green) / dv;
  }
  // Normalize to [0, 1] by multiplying by 60 degrees / 360 degrees
  hsc.hue = hsc.hue / 6.0f;

  // Apply any lightness or value specific adjustments to the result.
  finisher(&hsc, max, min, dv);
  return hsc;
}

Hsl to_hsl(const sRgb& srgb) {
  return srgb_to_hsc<Hsl>(srgb, [](Hsl* hsl, float max, float min, float dv) {
    hsl->lightness = (max + min) * 0.5f;
    if (dv == 0.0f) {
      hsl->saturation = 0.0f;
    } else {
      const float chroma = (1.0f - internal::abs(2.0f * hsl->lightness - 1.0f));
      hsl->saturation = dv / chroma;
    }
  });
}

Hsv to_hsv(const sRgb& srgb) {
  return srgb_to_hsc<Hsv>(srgb, [](Hsv* hsv, float max, float min, float dv) {
    hsv->value = max;
    if (max == 0.0f) {
      hsv->saturation = 0.0f;
    } else {
      hsv->saturation = dv / max;
    }
  });
}

sRgb to_srgb(rgb888_t rgb888) {
  sRgb srgb;
  srgb.red = float((rgb888 >> 16) & 0xff) / 255.0f;
  srgb.green = float((rgb888 >> 8) & 0xff) / 255.0f;
  srgb.blue = float((rgb888 >> 0) & 0xff) / 255.0f;
  return srgb;
}

sRgb to_srgb(uRgb urgb) {
  sRgb srgb;
  srgb.red = float(urgb.red) / 255.0f;
  srgb.green = float(urgb.green) / 255.0f;
  srgb.blue = float(urgb.blue) / 255.0f;
  return srgb;
}

sRgb to_srgb(const Hsv& hsv) {
  const float chroma = hsv.value * hsv.saturation;
  const float dv = hsv.value - chroma;
  return hsc_to_srgb(hsv, chroma, dv);
}

sRgb to_srgb(const Hsl& hsl) {
  const float chroma = (1.0f - internal::abs(2.0f * hsl.lightness - 1.0f)) * hsl.saturation;
  const float dv = hsl.lightness - 0.5f * chroma;
  return hsc_to_srgb(hsl, chroma, dv);
}

} // namespace color
