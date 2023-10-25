#include "color.h"
#include "interval.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define CLAMP_VAL 255.999

/* almost all computer programs assume that an image is “gamma *
 corrected” before being written into an image file. This means that *
 the 0 to 1 values have some transform applied before being stored *
 as a byte. Images with data that are written without being *
 transformed are said to be in linear space, whereas images that are *
 transformed are said to be in gamma space.  As a simple
 approximation, we can use “gamma 2” as our transform, which is the
 power that you use when going from gamma space to linear space. We
 need to go from linear space to gamma space, which means taking the
 inverse of “gamma 2", which means an exponent of 1/gamma, which is
 just the square-root. */
static double
color_linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}

void
color_write(FILE *f, Color pixel, size_t samples_per_pixel)
{
    double scale = 1.0 / (double) samples_per_pixel;
    // Refactor
    double gamma_r = color_linear_to_gamma(pixel.r * scale);
    double gamma_g = color_linear_to_gamma(pixel.g * scale);
    double gamma_b = color_linear_to_gamma(pixel.b * scale);
    static const Interval intensity = { .tmin = 0.000, .tmax = 0.999 };
    uint8_t r = (uint8_t) (CLAMP_VAL * interval_clamp(intensity, gamma_r));
    uint8_t g = (uint8_t) (CLAMP_VAL * interval_clamp(intensity, gamma_g));
    uint8_t b = (uint8_t) (CLAMP_VAL * interval_clamp(intensity, gamma_b));
    fprintf(f, "%d %d %d\n", r, g, b);
}
