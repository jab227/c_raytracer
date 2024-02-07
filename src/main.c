#include "camera.h"
#include "spheres.h"
#include "vec3.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#define ASPECT_RATIO(width, height) ((double) width / (double) height)
#define RT_PI 3.1415926535897932385

static double
degrees_to_radians(double angle)
{
    return angle * RT_PI / 180.0;
}

int
main(void)
{
    const double aspect_ratio = 16.0 / 9.0;
    const size_t image_width = 400;
    size_t image_height = (size_t) ((double) image_width / aspect_ratio);
    image_height = image_height < 1 ? 1 : image_height;
    assert(image_height >= 1);
    Image_size size = { .width = image_width, .height = image_height };
    const double real_ratio = ASPECT_RATIO(size.width, size.height);
    const double vfov = degrees_to_radians(20.0);
    const Vec3 lookfrom = { -2.0, 2.0, 1.0 };
    const Vec3 lookat = { 0.0, 0.0, -1.0 };
    const Vec3 vup = { 0.0, 1.0, 0.0 };
    const Vec3 camera_view_dir = vec3_sub(lookfrom, lookat);
    const Vec3 w = vec3_normalize(camera_view_dir);
    const Vec3 u = vec3_normalize(vec3_cross(vup, w));
    const Vec3 v = vec3_cross(w, u);
    // Camera settings
    // TODO(juan): Make cmd parser for config
    Camera cs = {
        .max_depth = 50,
        .real_aspect_ratio = real_ratio,
        .samples_per_pixel = 100,
        .center = lookfrom,
        .vfov = vfov,
        .defocus_angle = degrees_to_radians(10.0),
        .focus_distance = 3.4,
        .basis = {u, v, w}
    };
    init_viewport(&cs);
#define N_SPHERES 5
    // TODO(juan): render to a buffer to make it more format agnostic
    Sphere ground = {
        .center = { 0.0, -100.5, -1.0 },
        .radius = 100.0,
        .material = { .type = MATERIAL_TYPE_LAMBERTIAN, .albedo = { 0.8, 0.8, 0.0 } }
    };

    Sphere center = {
        .center = { 0.0, 0.0, -1.0 },
        .radius = 0.5,
        .material = { .type = MATERIAL_TYPE_LAMBERTIAN, .albedo = { 0.1, 0.2, 0.5 } }
    };
    // clang-format off
    Sphere left_inner = {
        .center = {-1.0, 0.0, -1.0},
        .radius = -0.4,
        .material = {
		.type = MATERIAL_TYPE_DIELECTRIC,
		.coefficient = 1.5,
	}
    };
    Sphere left = {
        .center = {-1.0, 0.0, -1.0},
        .radius = 0.5,
        .material = {
		.type = MATERIAL_TYPE_DIELECTRIC,
		.coefficient = 1.5,
	}
    };

    Sphere right = {
        .center = { 1.0, 0.0, -1.0 },
        .radius = 0.5,
        .material = {
		.type = MATERIAL_TYPE_METAL,
		.albedo = { 0.8, 0.6, 0.2 },
		.coefficient = 0.0,
	}
    };
    // clang-format on

    // Dont forget the order
    Sphere data[N_SPHERES] = { ground, center, left_inner, left, right };
    Sphere_View world = sphere_view_from_ptr(data, N_SPHERES);
    render(&cs, size, world);
    return 0;
}
