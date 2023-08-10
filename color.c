#include "color.h"
#include <stdint.h>
#include <stdio.h>

#define CLAMP 255.999
void color_write(FILE *f, color_t pixel)
{
    uint8_t r = (uint8_t) (CLAMP * pixel.r);
    uint8_t g = (uint8_t) (CLAMP * pixel.g);
    uint8_t b = (uint8_t) (CLAMP * pixel.b);

    fprintf(f, "%d %d %d\n", r, g, b);
}
