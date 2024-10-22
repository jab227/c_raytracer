#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>
#include <stdio.h>
#include "vec3.h"

typedef Vec3 Color;

void color_write(FILE *f, Color pixel, int32_t samples_per_pixel);

#endif
