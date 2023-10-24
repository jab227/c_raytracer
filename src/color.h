#ifndef COLOR_H_
#define COLOR_H_

#include <stdio.h>

typedef struct
{
    double r;
    double g;
    double b;
} Color;

void color_write(FILE *f, Color pixel, size_t samples_per_pixel);
Color color_lerp(Color c, double a);
#endif
