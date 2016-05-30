# Color

![TravisCI Build Status](https://travis-ci.org/uncommoncode/color_cpp.svg?branch=master)


This C+11 library provides a self-contained colorspace conversion and interpolation for common color formats useful in artistic and visualization settings.

The currently supported colorspaces include:

 * sRGB
 * RGB 888 (such as "hex" `0xffffff`)
 * HSV
 * HSL
 * CIE-XYZ
 * `L*a*b*`

# Example
There are conversion helpers to allow converting from one space to another. For example, converting a hex color from `0xd7f310` to HSV and printing the output:

```
#include <iostream>

#include <color/transformation.hpp>

int main() {
	color::sRgb srgb(color::to_srgb(0xd7f310));
	color::Hsv hsv(color::to_hsv(srgb));
	
	std::cout << hsv.hue << ", " << hsv.saturation << ", " << hsv.value << std::endl;
	
	return 0;
}

```
