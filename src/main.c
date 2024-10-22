#include "camera.h"
#include "color.h"
#include "prng.h"
#include "spheres.h"
#include "vec3.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    // Camera settings
    // TODO(juan): Make cmd parser for config
    Camera_Config cfg = {
        .aspect_ratio = 16.0 / 9.0,
        .image_width = 1200,
        .focus_distance = 10.0,
        .defocus_angle = 0.6,
        .vfov = 20.0,
        .lookfrom = { { 13.0, 2.0, 3.0 } },
        .lookat = { { 0.0, 0.0, 0.0 } },
        .vup = { { 0.0, 1.0, 0.0 } },
        .samples_per_pixel = 500,
        .max_depth = 50,
    };
    Camera c = camera_init(&cfg);
    // TODO(juan): render to a buffer to make it more format agnostic
    Sphere ground = {
        .center = { { 0.0, -1000.0, 0.0 } },
        .radius = 1000.0,
        .material = { .type = MATERIAL_TYPE_LAMBERTIAN,
                   .albedo = { { 0.5, 0.5, 0.5 } } }
    };

    Sphere *s = (Sphere *) calloc(488, sizeof(Sphere));
    s[0] = ground;
    int64_t len = 1;
    for (int32_t i = -11; i < 11; ++i) {
        for (int32_t j = -11; j < 11; ++j) {
            Vec3 center = {
                .x = i + 0.9 * randomd(),
                .y = 0.2,
                .z = j + 0.9 * randomd(),
            };
            Vec3 p = {
                { 4.0, 0.2, 0 }
            };
            if (vec3_norm(vec3_sub(center, p)) > 0.9) {
                double choose_material = randomd();
                Material m;
                if (choose_material < 0.8) {
                    m.albedo = vec3_prod(vec3_random(), vec3_random());
                    m.type = MATERIAL_TYPE_LAMBERTIAN;
                } else if (choose_material < 0.95) {
                    m.albedo = vec3_random_in(0.5, 1.0);
                    m.coefficient = randomd_in(0.0, 0.5);
                    m.type = MATERIAL_TYPE_METAL;
                } else {
                    m.coefficient = 1.5;
                    m.type = MATERIAL_TYPE_DIELECTRIC;
                }
                s[len] =
                    (Sphere) { .center = center, .material = m, .radius = 0.2 };
                len++;
            }
        }
    }

    // clang-format off
    s[len] = (Sphere){
        .center = {{0.0, 1.0, 0.0}},
        .radius = 1.0,
        .material = {
		.type = MATERIAL_TYPE_DIELECTRIC,
		.coefficient = 1.5,
	}
    };
    len++;
    s[len]  = (Sphere){
        .center = {{-4.0, 1.0, 0.0}},
        .radius = 1.0,
        .material = {
		.type = MATERIAL_TYPE_LAMBERTIAN,
		.albedo={{0.4, 0.2, 0.1}},
	}
    };
    len++;
    s[len]=(Sphere){
        .center = { {4.0, 1.0, 0.0} },
        .radius = 1.0,
        .material = {
		.type = MATERIAL_TYPE_METAL,
		.albedo = { {0.7, 0.6, 0.5} },
		.coefficient = 0.0,
	}
    };
    len++;
    // clang-format on
    // Dont forget the order
    Sphere_View world = sphere_view_from_ptr(s, (size_t) len);
    render(&c, world);
    free(s);
    return 0;
}
