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
    Vec3 lookfrom;
    Vec3 lookat;
    Vec3 vup;
    double aspect_ratio;
    double vfov;           // In degrees
    double defocus_angle;  // In degrees
    double focus_distance;
    size_t image_width;
    int32_t samples_per_pixel;
    int32_t max_depth;
} Camera_Config;

typedef struct
{
    size_t height;
    size_t width;
} Image_size;

typedef struct
{
    Camera_Frame_Basis basis;
    Viewport viewport;
    Pixel_Deltas dudv;
    Vec3 center;
    Vec3 pixel_00_loc;
    Vec3 defocus_disk_u;
    Vec3 defocus_disk_v;
    Image_size size;
    double defocus_angle;  // In radians
    int32_t samples_per_pixel;
    int32_t max_depth;
} Camera;


typedef struct
{
    size_t row;
    size_t col;
} Image_Pos;

Color ray_color(Ray r, Sphere_View spheres, int32_t depth);
Camera camera_init(const Camera_Config *cfg);
void render(const Camera *cs, Sphere_View world);

#endif
