#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "color.h"

int main(void)
{
    const size_t image_width = 256;
    const size_t image_height = 256;

    printf("P3\n%zu %zu\n255\n", image_width, image_height);
    for (size_t j = 0; j < image_height; ++j) {
        for (size_t i = 0; i < image_width; ++i) {
            color_t pixel = { .r = (double) i / (double) (image_width - 1),
                              .g = (double) j / (double) (image_height - 1),
                              .b = 0 };
            color_write(stdout, pixel);
        }
    }
    return 0;
}
