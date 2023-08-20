#ifndef CAMERA_H_
#define CAMERA_H_

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
} Camera;

typedef struct {
  size_t height;
  size_t width;
} ImageSize;

typedef struct {
  size_t row;
  size_t col;
} ImagePos;

PixelDeltas compute_pixel_deltas_location(ViewportVectors uv, ImageSize size);
Vec3 camera_compute_viewport_upper_left(Camera c);
Vec3 compute_pixel_00_location(Vec3 viewport_upper_left, PixelDeltas dudv);
Vec3 compute_pixel_center(Vec3 pixel_00_loc, PixelDeltas dudv, ImagePos p);
#endif
