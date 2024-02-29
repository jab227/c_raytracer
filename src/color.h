#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>
#include <stdio.h>

typedef struct
{
    double r;
    double g;
    double b;
} Color;

void color_write(FILE *f, Color pixel, int32_t samples_per_pixel);
Color color_lerp(Color c, double a);
Color color_add(Color lhs, Color rhs);
Color color_prod(Color lhs, Color rhs);
Color color_scale(Color c, double scale);
Color color_attenuate(Color c, Color attenuation);
Color color_random_in(double min, double max);
Color color_random();

#endif
