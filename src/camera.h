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
} Viewport_Size;

typedef struct
{
    Vec3 u;
    Vec3 v;
} Viewport_UV;

typedef struct
{
    Viewport_UV uv;
    Viewport_Size size;
} Viewport;

typedef struct
{
    Vec3 du;
    Vec3 dv;
} Pixel_Deltas;

typedef struct
{
    Viewport viewport;
    Vec3 center;
    size_t samples_per_pixel;
    double focal_length;
    int32_t max_depth;
} Camera;

typedef struct
{
    size_t height;
    size_t width;
} Image_size;

typedef struct
{
    size_t row;
    size_t col;
} Image_Pos;

Color ray_color(Ray r, Sphere_View spheres, int32_t depth);
void render(const Camera *cs, Image_size s, Sphere_View world);

#endif
