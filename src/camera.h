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
    Vec3 u;
    Vec3 v;
    Vec3 w;
} Camera_Frame_Basis;

typedef struct
{
    Viewport viewport;
    Camera_Frame_Basis basis;
    Vec3 center;
    size_t samples_per_pixel;
    double focus_distance;
    double defocus_angle;
    double real_aspect_ratio;
    double vfov;  // In radians
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
void init_viewport(Camera *c);
void render(const Camera *cs, Image_size s, Sphere_View world);

#endif
