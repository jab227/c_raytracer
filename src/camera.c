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
camera__compute_viewport_upper_left(Viewport_UV uv,
                                    Vec3 center,
                                    Vec3 w,
                                    double focus_distance)
{
    // camera_center - vec3{0,0,focal_lenght} - viewport_u / 2 - viewport_v / 2
    Vec3 distance_from_center = vec3_mul(w, focus_distance);
    Vec3 viewport_add = vec3_add(uv.u, uv.v);

    // camera_center - (ditance_from_center + 0.5 (viewport_u + viewpofrt_v))
    return vec3_sub(center,
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
camera__get_ray(const Camera *c, Image_Pos pos)
{
    // Until here the values can be cached
    Vec3 pixel_center =
        camera__compute_pixel_center(c->pixel_00_loc, c->dudv, pos);
    Vec3 pixel_sample =
        vec3_add(pixel_center, camera__pixel_sample_square(c->dudv));
    Vec3 ray_origin = (c->defocus_angle <= 0)
                          ? c->center
                          : camera__defocus_disk_sample(c->defocus_disk_u,
                                                        c->defocus_disk_v,
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
    Color attenuation = {
        { 1.0, 1.0, 1.0 }
    };
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
                attenuation = vec3_prod(attenuation, result.attenuation);
                next = result.scattered;
            }
        } else {
            const Vec3 unit_direction = vec3_normalize(next.direction);
            const double a = 0.5 * (unit_direction.y + 1.0);
            Color c = vec3_lerp(
                (Color) {
                    { 0.5, 0.7, 1.0 },
            },
                a);
            return vec3_prod(c, attenuation);
        }
    }
    return (Color) { 0 };
}


#define RT_PI 3.1415926535897932385

static double
degrees_to_radians(double angle)
{
    return angle * RT_PI / 180.0;
}
#include <assert.h>

#define ASPECT_RATIO(width, height) ((double) (width) / (double) (height))

Camera
camera_init(const Camera_Config *cfg)
{
    // Convert angles to radians
    const double vfov_rad = degrees_to_radians(cfg->vfov);
    const double defocus_angle_rad = degrees_to_radians(cfg->defocus_angle);

    // Compute image height
    size_t img_height = (size_t) ((double) cfg->image_width / cfg->aspect_ratio);
    img_height = img_height < 1 ? 1 : img_height;
    assert(img_height >= 1);
    Image_size size = { .width = cfg->image_width, .height = img_height };

    // Real aspect ratio
    const double real_ratio = ASPECT_RATIO(size.width, size.height);

    // Compute camera basis
    const Vec3 camera_view_dir = vec3_sub(cfg->lookfrom, cfg->lookat);
    const Vec3 w = vec3_normalize(camera_view_dir);
    const Vec3 u = vec3_normalize(vec3_cross(cfg->vup, w));
    const Vec3 v = vec3_cross(w, u);

    // Create Viewport
    const double theta = vfov_rad;
    const double h = tan(theta / 2);
    const double height = 2 * h * cfg->focus_distance;
    const Viewport_Size vp_size = {
        .height = height,
        .width = height * real_ratio,
    };
    const Viewport vp = {
        .size = vp_size,
        .uv = { .u = vec3_mul(u, vp_size.width),
               .v = vec3_mul(vec3_neg(v), vp_size.height) }
    };

    const Pixel_Deltas dudv = camera__compute_pixel_deltas_location(vp.uv, size);
    Vec3 vp_upper_left = camera__compute_viewport_upper_left(
        vp.uv, cfg->lookfrom, w, cfg->focus_distance);
    Vec3 pixel_00_loc = camera__compute_pixel_00_location(vp_upper_left, dudv);
    double defocus_radius = cfg->focus_distance * tan(defocus_angle_rad / 2.0);
    Vec3 defocus_disk_u = vec3_mul(u, defocus_radius);
    Vec3 defocus_disk_v = vec3_mul(v, defocus_radius);

    return (Camera) {
        .basis = { u, v, w },
        .viewport = vp,
        .dudv = dudv,
        .center = cfg->lookfrom,
        .pixel_00_loc = pixel_00_loc,
        .defocus_disk_u = defocus_disk_u,
        .defocus_disk_v = defocus_disk_v,
        .size = size,
        .defocus_angle = defocus_angle_rad,
        .samples_per_pixel = cfg->samples_per_pixel,
        .max_depth = cfg->max_depth,
    };
}

void
render(const Camera *c, Sphere_View world)
{
    printf("P3\n%zu %zu\n255\n", c->size.width, c->size.height);
    for (size_t j = 0; j < c->size.height; ++j) {
        for (size_t i = 0; i < c->size.width; ++i) {
            Color pixel = { 0 };
            Image_Pos pos = { .col = i, .row = j };
            for (int32_t sample = 0; sample < c->samples_per_pixel; ++sample) {
                Ray r = camera__get_ray(c, pos);
                Color new_color = ray_color(r, world, c->max_depth);
                pixel = vec3_add(pixel, new_color);
            }
            color_write(stdout, pixel, c->samples_per_pixel);
        }
    }
}
