#ifndef CAMERA_H_
#define CAMERA_H_

#include "color.h"
#include "ray.h"
#include "spheres.h"
#include "vec3.h"

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    double height;
    double width;
} ViewportSize;

typedef struct
{
    Vec3 u;
    Vec3 v;
} ViewportUV;

typedef struct
{
    ViewportUV uv;
    ViewportSize size;
} Viewport;

typedef struct
{
    Vec3 du;
    Vec3 dv;
} PixelDeltas;

typedef struct
{
    Viewport viewport;
    Vec3 center;
    size_t samples_per_pixel;
    double focal_length;
    int32_t max_depth;
} CameraSystem;

typedef struct
{
    size_t height;
    size_t width;
} ImageSize;

typedef struct
{
    size_t row;
    size_t col;
} ImagePos;

Color ray_color(Ray r, const Spheres *s, size_t n_spheres, int32_t depth);
void render(const CameraSystem *cs,
            ImageSize s,
            const Spheres *world,
            size_t world_size);

#endif
