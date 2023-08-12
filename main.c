#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "color.h"
#include "ray.h"
#include "vec3.h"

color_t ray_color(ray_t r)
{
    vec3_t unit_direction = vec3_normalize(r.direction);
    double scale = 0.5 * (unit_direction.y + 1.0);
    return (color_t){ .r = (1.0 - scale) * 1.0 - scale * 0.5,
                      .g = (1.0 - scale) * 1.0 - scale * 0.7,
                      .b = (1.0 - scale) * 1.0 - scale * 1.0 };
}

int main(void)
{
    const double aspect_ratio = 16.0 / 9.0;
    const size_t image_width = 256;
    size_t image_height = (size_t) ((double) image_width / aspect_ratio);
    image_height = image_height < 1 ? 1 : image_height;

    // Camera settings
    const double focal_length = 1.0;
    const double viewport_height = 2.0;
    const double viewport_width =
        viewport_height * ((double) (image_width) / (double) image_height);
    vec3_t camera_center = { .x = 0., .y = 0., .z = 0 };
    vec3_t viewport_u = { .x = viewport_width, .y = 0., .z = 0. };
    vec3_t viewport_v = { .x = 0., .y = -viewport_height, .z = 0. };

    vec3_t pixel_delta_u = vec3_div(viewport_u, (double) image_width);
    vec3_t pixel_delta_v = vec3_div(viewport_v, (double) image_height);

    // Calculate the location of the upper left pixel;
    vec3_t viewport_upper_left = vec3_sub(
        vec3_sub(camera_center, (vec3_t){ .x = 0., .y = 0., .z = focal_length }),
        vec3_sub(vec3_div(viewport_u, 2.), vec3_div(viewport_v, 2.)));
    vec3_t pixel00_loc =
        vec3_add(viewport_upper_left,
                 vec3_mul(vec3_add(pixel_delta_u, pixel_delta_v), 0.5));

    printf("P3\n%zu %zu\n255\n", image_width, image_height);
    for (size_t j = 0; j < image_height; ++j) {
        for (size_t i = 0; i < image_width; ++i) {
            vec3_t pixel_center =
                vec3_add(pixel00_loc,
                         vec3_add(vec3_mul(pixel_delta_u, (double) i),
                                  vec3_mul(pixel_delta_v, (double) j)));
            vec3_t ray_direction = vec3_sub(pixel_center, camera_center);
            ray_t r = { .direction = ray_direction, .origin = camera_center };
            color_t pixel = ray_color(r);
            color_write(stdout, pixel);
        }
    }
    return 0;
}
