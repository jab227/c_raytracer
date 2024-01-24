#include "camera.h"
#include "spheres.h"

#include <assert.h>
#include <stddef.h>

#define ASPECT_RATIO(width, height) ((double) width / (double) height)

int
main(void)
{
    const double aspect_ratio = 16.0 / 9.0;
    const size_t image_width = 400;
    size_t image_height = (size_t) ((double) image_width / aspect_ratio);
    image_height = image_height < 1 ? 1 : image_height;
    assert(image_height >= 1);
    Image_size size = { .width = image_width, .height = image_height };

    double viewport_height = 2.0;
    double real_ratio = ASPECT_RATIO(size.width, size.height);
    double viewport_width = viewport_height * real_ratio;


    // Camera settings
    // TODO(juan): Make cmd parser for config
    Camera cs = {
        .max_depth = 50,
        .focal_length = 1.0,
        .samples_per_pixel = 500,
        .center = {0},
        .viewport = {
	     .size = {.height = viewport_height, .width = viewport_width,},
	     .uv = {
		  .u = {.x = viewport_width},
		  .v = {.y = -viewport_height},
	     }
	},
    };
    // TODO(juan): render to a buffer to make it more format agnostic
#define N_SPHERES 4
    Sphere ground = {
        .center = { 0.0, -100.5, -1.0 },
        .radius = 100.0,
        .material = { .type = MATERIAL_TYPE_LAMBERTIAN, .albedo = { 0.8, 0.8, 0.0 } }
    };

    Sphere center = {
        .center = { 0.0, 0.0, -1.0 },
        .radius = 0.5,
        .material = { .type = MATERIAL_TYPE_DIELECTRIC, .coefficient = 1.5, }
    };
    // clang-format off
    Sphere left = {
        .center = {-1.0, 0.0, -1.0},
        .radius = 0.5,
        .material = { 
		.type = MATERIAL_TYPE_DIELECTRIC,
		.albedo = { 0.8, 0.8, 0.8 },
		.coefficient = 1.5,
	}
    };

    Sphere right = {
        .center = { 1.0, 0.0, -1.0 },
        .radius = 0.5,
        .material = {
		.type = MATERIAL_TYPE_METAL,
		.albedo = { 0.8, 0.6, 0.2 },
		.coefficient = 1.0,
	}
    };
    // clang-format on

    // Dont forget the order
    Sphere data[N_SPHERES] = { ground, left, right, center };
    Sphere_View world = sphere_view_from_ptr(data, N_SPHERES);
    render(&cs, size, world);
    return 0;
}
