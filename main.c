#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "color.h"
#include "ray.h"
#include "vec3.h"


double hit_sphere(Vec3 center, double radius, Ray r)
{
    Vec3 oc = vec3_sub(r.origin, center);
    double a = vec3_norm_squared(r.direction);
    double half_b = vec3_dot(oc, r.direction);
    double c = vec3_norm_squared(oc) - radius * radius;

    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

Color ray_color(Ray r)
{
    Vec3 center = { .x = 0., .y = 0., .z = -1. };
    double t = hit_sphere(center, 0.5, r);
    if (t > 0.0) {
        Vec3 hit_direction = ray_at(r, t);
        Vec3 normal_vector = vec3_sub(hit_direction, center);
        Vec3 unit_normal_vector = vec3_normalize(normal_vector);
        return (Color){ .r = 0.5 * (unit_normal_vector.x + 1.0),
                        .g = 0.5 * (unit_normal_vector.y + 1.0),
                        .b = 0.5 * (unit_normal_vector.z + 1.0) };
    }
    Vec3 unit_direction = vec3_normalize(r.direction);
    double a = 0.5 * (unit_direction.y + 1.0);
    return (Color){ .r = (1.0 - a) + (a * 0.5),
                    .g = (1.0 - a) + (a * 0.7),
                    .b = (1.0 - a) + (a * 1.0) };
}

Vec3 compute_viewport_upper_left(Vec3 camera_center,
                                 Vec3 viewport_u,
                                 Vec3 viewport_v,
                                 double focal_length)
{
    // camera_center - vec3{0,0,focal_lenght} - viewport_u / 2 - viewport_v / 2
    Vec3 distance_from_center = { .x = 0.0, .y = 0.0, .z = focal_length };
    Vec3 viewport_add = vec3_add(viewport_u, viewport_v);

    // camera_center - (ditance_from_center + 0.5 (viewport_u + viewport_v))
    return vec3_sub(camera_center,
                    vec3_add(distance_from_center, vec3_mul(viewport_add, 0.5)));
}

Vec3 compute_pixel_00_location(Vec3 viewport_upper_left,
                               Vec3 pixel_delta_u,
                               Vec3 pixel_delta_v)
{
    // viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    Vec3 a = vec3_mul(vec3_add(pixel_delta_u, pixel_delta_v), 0.5);
    return vec3_add(viewport_upper_left, a);
}

Vec3 compute_pixel_center(
    Vec3 pixel_00_loc, Vec3 pixel_delta_u, Vec3 pixel_delta_v, size_t i, size_t j)
{
    // pixel_00_loc + (i * pixel_delta_u) + (j * pixel_delta_v)
    double id = (double) i;
    double jd = (double) j;

    Vec3 a = vec3_mul(pixel_delta_u, id);
    Vec3 b = vec3_mul(pixel_delta_v, jd);
    Vec3 c = vec3_add(a, b);

    return vec3_add(pixel_00_loc, c);
}

int main(void)
{
    const double aspect_ratio = 16.0 / 9.0;
    const size_t image_width = 400;
    // Computes de image height an ensure that it's atleast 1
    size_t image_height = (size_t) ((double) image_width / aspect_ratio);
    image_height = image_height < 1 ? 1 : image_height;
    assert(image_height >= 1);

    // Camera settings
    const double focal_length = 1.0;
    const double viewport_height = 2.0;
    const double viewport_width =
        viewport_height * ((double) image_width / (double) image_height);
    Vec3 camera_center = { .x = 0.0, .y = 0.0, .z = 0.0 };

    Vec3 viewport_u = { .x = viewport_width, .y = 0.0, .z = 0.0 };
    Vec3 viewport_v = { .x = 0.0, .y = -viewport_height, .z = 0.0 };

    Vec3 pixel_delta_u = vec3_div(viewport_u, (double) image_width);
    Vec3 pixel_delta_v = vec3_div(viewport_v, (double) image_height);

    // Compute the location of the upper left pixel;
    Vec3 viewport_upper_left = compute_viewport_upper_left(
        camera_center, viewport_u, viewport_v, focal_length);
    Vec3 pixel00_loc = compute_pixel_00_location(viewport_upper_left,
                                                 pixel_delta_u,
                                                 pixel_delta_v);
    printf("P3\n%zu %zu\n255\n", image_width, image_height);
    for (size_t j = 0; j < image_height; ++j) {
        for (size_t i = 0; i < image_width; ++i) {
            Vec3 pixel_center = compute_pixel_center(
                pixel00_loc, pixel_delta_u, pixel_delta_v, i, j);
            Vec3 ray_direction = vec3_sub(pixel_center, camera_center);
            Ray r = { .direction = ray_direction, .origin = camera_center };
            Color pixel = ray_color(r);
            color_write(stdout, pixel);
        }
    }
    return 0;
}
