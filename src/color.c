#include "color.h"
#include "spheres.h"
#include <stdint.h>
#include <stdio.h>

#define CLAMP 255.999
void color_write(FILE *f, Color pixel, size_t samples_per_pixel) {
  double scale = 1.0 / (double)samples_per_pixel;
  // Refactor
  static const Interval intensity = {.tmin = 0.000, .tmax = 0.999};
  uint8_t r = (uint8_t)(CLAMP * interval_clamp(intensity, pixel.r * scale));
  uint8_t g = (uint8_t)(CLAMP * interval_clamp(intensity, pixel.g * scale));
  uint8_t b = (uint8_t)(CLAMP * interval_clamp(intensity, pixel.b * scale));
  fprintf(f, "%d %d %d\n", r, g, b);
}
