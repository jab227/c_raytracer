#ifndef COLOR_H_
#define COLOR_H_

#include <stdio.h>

typedef struct {
    double r;
    double g;
    double b;
} color_t;

void color_write(FILE *f, color_t pixel);
#endif
