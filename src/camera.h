#ifndef CAMERA_H_
#define CAMERA_H_

#include "color.h"
#include "ray.h"
#include "spheres.h"
#include "vec3.h"

#include <stddef.h>

typedef struct {
  double height;
  double width;
} ViewportSize;

typedef struct {
  Vec3 u;
  Vec3 v;
} ViewportVectors;

typedef struct {
  ViewportVectors uv;
  ViewportSize size;
} Viewport;

typedef struct {
  Vec3 du;
  Vec3 dv;
} PixelDeltas;

typedef struct {
  Viewport viewport;
  Vec3 center;
  double focal_length;
} CameraSystem;

typedef struct {
  size_t height;
  size_t width;
} ImageSize;

typedef struct {
  size_t row;
  size_t col;
} ImagePos;

PixelDeltas compute_pixel_deltas_location(ViewportVectors uv, ImageSize size);
Vec3 camera_compute_viewport_upper_left(const CameraSystem *cs);
Vec3 compute_pixel_00_location(Vec3 viewport_upper_left, PixelDeltas dudv);
Vec3 compute_pixel_center(Vec3 pixel_00_loc, PixelDeltas dudv, ImagePos p);
Color ray_color(Ray r, const Spheres *s, size_t n_spheres);
void render(const CameraSystem *cs, ImageSize s, Spheres *world,
            size_t world_size);

#endif
