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
camera__compute_pixel_deltas_location(Viewport_UV uv, Image_size size)
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
    Vec3 distance_from_center = vec3_mul(cs->basis.w, cs->focus_distance);
    Vec3 viewport_add = vec3_add(uv.u, uv.v);

    // camera_center - (ditance_from_center + 0.5 (viewport_u + viewpofrt_v))
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

static Vec3
camera__defocus_disk_sample(Vec3 defocus_disk_u, Vec3 defocus_disk_v, Vec3 center)
{
    Vec3 p = vec3_random_in_unit_disk();
    Vec3 u = vec3_mul(defocus_disk_u, p.x);
    Vec3 v = vec3_mul(defocus_disk_v, p.y);
    return vec3_add(center, vec3_add(u, v));
}

static Ray
camera__get_ray(const Camera *c, Image_Pos pos, Image_size s)
{
    Pixel_Deltas dudv = camera__compute_pixel_deltas_location(c->viewport.uv, s);
    Vec3 viewport_upper_left = camera__compute_viewport_upper_left(c);
    Vec3 pixel00_loc =
        camera__compute_pixel_00_location(viewport_upper_left, dudv);
    double defocus_radius = c->focus_distance * tan(c->defocus_angle / 2.0);
    Vec3 defocus_disk_u = vec3_mul(c->basis.u, defocus_radius);
    Vec3 defocus_disk_v = vec3_mul(c->basis.v, defocus_radius);
    // Until here the values can be cached
    Vec3 pixel_center = camera__compute_pixel_center(pixel00_loc, dudv, pos);
    Vec3 pixel_sample = vec3_add(pixel_center, camera__pixel_sample_square(dudv));
    Vec3 ray_origin = (c->defocus_angle <= 0)
                          ? (Vec3){ 0 }
                          : camera__defocus_disk_sample(defocus_disk_u,
                                                        defocus_disk_v,
                                                        c->center);
    Vec3 ray_direction = vec3_sub(pixel_sample, ray_origin);
    Ray r = {
        .direction = ray_direction,
        .origin = ray_origin,
    };

    return r;
}

Color
ray_color(Ray r, Sphere_View spheres, int32_t depth)
{
    Color attenuation = { 1.0, 1.0, 1.0 };
    Ray next = r;
    // NOTE(juan): Fix shadow acne -> tmin from 0.0 to 0.001
    Interval interval = { .tmin = 0.001, .tmax = INFINITY };
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
init_viewport(Camera *c)
{
    double theta = c->vfov;
    double h = tan(theta / 2);
    double height = 2 * h * c->focus_distance;
    Viewport_Size vp_size = {
        .height = height,
        .width = height * c->real_aspect_ratio,
    };

    Viewport vp = {
        .size = vp_size,
        .uv = {.u = vec3_mul(c->basis.u, vp_size.width),
               .v = vec3_mul(vec3_neg(c->basis.v), vp_size.height)}
    };
    c->viewport = vp;
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
