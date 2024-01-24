#include "camera.h"
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "spheres.h"
#include "vec3.h"
#include <stdint.h>
#include <stdio.h>


#include <math.h>
#include <stddef.h>

#include "prng.h"


#define EPSILON 1e-8

static Pixel_Deltas
compute__pixel_deltas_location(Viewport_UV uv, Image_size size)
{
    double image_width = (double) size.width;
    double image_height = (double) size.height;
    Pixel_Deltas dudv = {
        .du = vec3_div(uv.u, image_width),
        .dv = vec3_div(uv.v, image_height),
    };
    return dudv;
}

static Vec3
camera__compute_viewport_upper_left(const Camera *cs)
{
    Viewport_UV uv = cs->viewport.uv;
    // camera_center - vec3{0,0,focal_lenght} - viewport_u / 2 - viewport_v / 2
    Vec3 distance_from_center = { .x = 0.0, .y = 0.0, .z = cs->focal_length };
    Vec3 viewport_add = vec3_add(uv.u, uv.v);

    // camera_center - (ditance_from_center + 0.5 (viewport_u + viewport_v))
    return vec3_sub(cs->center,
                    vec3_add(distance_from_center, vec3_mul(viewport_add, 0.5)));
}

static Vec3
camera__compute_pixel_00_location(Vec3 viewport_upper_left, Pixel_Deltas dudv)
{
    Vec3 a = vec3_mul(vec3_add(dudv.du, dudv.dv), 0.5);
    return vec3_add(viewport_upper_left, a);
}

static Vec3
camera__compute_pixel_center(Vec3 pixel_00_loc, Pixel_Deltas dudv, Image_Pos p)
{
    double col = (double) p.col;
    double row = (double) p.row;

    Vec3 a = vec3_mul(dudv.du, col);
    Vec3 b = vec3_mul(dudv.dv, row);

    return vec3_add(pixel_00_loc, vec3_add(a, b));
}

static Vec3
camera__pixel_sample_square(Pixel_Deltas dudv)
{
    double px = -0.5 + randomd();
    double py = -0.5 + randomd();

    return vec3_add(vec3_mul(dudv.du, px), vec3_mul(dudv.dv, py));
}

static Ray
camera__get_ray(const Camera *cs, Image_Pos pos, Image_size s)
{
    Pixel_Deltas dudv = compute__pixel_deltas_location(cs->viewport.uv, s);
    Vec3 viewport_upper_left = camera__compute_viewport_upper_left(cs);
    Vec3 pixel00_loc =
        camera__compute_pixel_00_location(viewport_upper_left, dudv);

    Vec3 pixel_center = camera__compute_pixel_center(pixel00_loc, dudv, pos);
    Vec3 pixel_sample = vec3_add(pixel_center, camera__pixel_sample_square(dudv));
    Vec3 ray_direction = vec3_sub(pixel_sample, cs->center);

    Ray r = { .direction = ray_direction, .origin = cs->center };

    return r;
}

Color
ray_color(Ray r, Sphere_View spheres, int32_t depth)
{
    // NOTE(juan): Fix shadow acne -> tmin from 0.0 to 0.001
    Interval interval = { .tmin = 0.001, .tmax = INFINITY };
    Color attenuation = { 1.0, 1.0, 1.0 };
    Ray next = r;
    for (int32_t i = 0; i < depth; ++i) {
        Hits record = sphere_hit(spheres, next, interval);
        if (record.hit_anything) {
            // This was the important part when transforming the
            // algorithm from recursive to iterative. The thing being
            // modified was the reflectance
            Scatter_Result result = material_scatter(next, &record, EPSILON);
            if (result.hit_anything) {
                attenuation = color_attenuate(attenuation, result.attenuation);
                next = result.scattered;
            } else {
                break;
            }
        } else {
            const Vec3 unit_direction = vec3_normalize(next.direction);
            const double a = 0.5 * (unit_direction.y + 1.0);
            Color c = color_lerp((Color){ 0.5, 0.7, 1.0 }, a);
            return color_attenuate(c, attenuation);
        }
    }
    return (Color){ 0 };
}


void
render(const Camera *cs, Image_size s, Sphere_View world)
{
    printf("P3\n%zu %zu\n255\n", s.width, s.height);
    for (size_t j = 0; j < s.height; ++j) {
        for (size_t i = 0; i < s.width; ++i) {
            Color pixel = { 0 };
            Image_Pos pos = { .col = i, .row = j };
            for (size_t sample = 0; sample < cs->samples_per_pixel; ++sample) {
                Ray r = camera__get_ray(cs, pos, s);
                Color new_color = ray_color(r, world, cs->max_depth);
                pixel = color_add(pixel, new_color);
            }
            color_write(stdout, pixel, cs->samples_per_pixel);
        }
    }
}
