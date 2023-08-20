#include "camera.h"
#include "vec3.h"

PixelDeltas compute_pixel_deltas_location(ViewportVectors uv, ImageSize size) {
  double image_width = (double)size.width;
  double image_height = (double)size.height;
  PixelDeltas dudv = {.du = vec3_div(uv.u, image_width),
                      .dv = vec3_div(uv.v, image_height)};
  return dudv;
}

Vec3 camera_compute_viewport_upper_left(Camera c) {
  ViewportVectors uv = c.viewport.uv;
  // camera_center - vec3{0,0,focal_lenght} - viewport_u / 2 - viewport_v / 2
  Vec3 distance_from_center = {.x = 0.0, .y = 0.0, .z = c.focal_length};
  Vec3 viewport_add = vec3_add(uv.u, uv.v);

  // camera_center - (ditance_from_center + 0.5 (viewport_u + viewport_v))
  return vec3_sub(c.center,
                  vec3_add(distance_from_center, vec3_mul(viewport_add, 0.5)));
}

Vec3 compute_pixel_00_location(Vec3 viewport_upper_left, PixelDeltas dudv) {
  Vec3 a = vec3_mul(vec3_add(dudv.du, dudv.dv), 0.5);
  return vec3_add(viewport_upper_left, a);
}

Vec3 compute_pixel_center(Vec3 pixel_00_loc, PixelDeltas dudv, ImagePos p) {
  double col = (double)p.col;
  double row = (double)p.row;

  Vec3 a = vec3_mul(dudv.du, col);
  Vec3 b = vec3_mul(dudv.dv, row);

  return vec3_add(pixel_00_loc, vec3_add(a, b));
}
