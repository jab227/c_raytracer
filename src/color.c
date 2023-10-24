#include "color.h"
#include "interval.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define CLAMP 255.999

static Color
linear_to_gamma(Color c)
{
    return (Color){
        sqrt(c.r),
        sqrt(c.g),
        sqrt(c.b),
    };
}

static Color
color_scale(Color c, double scale)
{
    return (Color){
        .r = c.r * scale,
        .g = c.g * scale,
        .b = c.b * scale,
    };
}

void
color_write(FILE *f, Color pixel, size_t samples_per_pixel)
{
    double scale = 1.0 / (double) samples_per_pixel;
    // Refactor

    Color scaled_pixel = color_scale(pixel, scale);
    Color gamma_pixel = linear_to_gamma(scaled_pixel);

    static const Interval intensity = { .tmin = 0.000, .tmax = 0.999 };
    uint8_t r = (uint8_t) (CLAMP * interval_clamp(intensity, gamma_pixel.r));
    uint8_t g = (uint8_t) (CLAMP * interval_clamp(intensity, gamma_pixel.g));
    uint8_t b = (uint8_t) (CLAMP * interval_clamp(intensity, gamma_pixel.b));
    fprintf(f, "%d %d %d\n", r, g, b);
}

Color
color_lerp(Color c, double a)
{
    return (Color){ .r = (1.0 - a) + (a * c.r),
                    .g = (1.0 - a) + (a * c.g),
                    .b = (1.0 - a) + (a * c.b) };
}
