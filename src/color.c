#include "color.h"
#include "interval.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define CLAMP 255.999

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
static Color
color__linear_to_gamma(Color linear_color)
{
    return (Color){
        sqrt(linear_color.r),
        sqrt(linear_color.g),
        sqrt(linear_color.b),
    };
}

Color
color_scale(Color c, double scale)
{
    return (Color){
        .r = c.r * scale,
        .g = c.g * scale,
        .b = c.b * scale,
    };
}

Color
color_attenuate(Color c, Color attenuation)
{
    return (Color){
        .r = c.r * attenuation.r,
        .g = c.g * attenuation.g,
        .b = c.b * attenuation.b,
    };
}

void
color_write(FILE *f, Color pixel, size_t samples_per_pixel)
{
    double scale = 1.0 / (double) samples_per_pixel;

    Color scaled_pixel = color_scale(pixel, scale);
    Color gamma_pixel = color__linear_to_gamma(scaled_pixel);

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

Color
color_add(Color lhs, Color rhs)
{
    return (Color){
        .r = rhs.r + lhs.r,
        .g = rhs.g + lhs.g,
        .b = rhs.b + lhs.b,
    };
}
