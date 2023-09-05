#include "camera.h"
#include "color.h"
#include "ray.h"
#include "spheres.h"
#include "vec3.h"

#include <math.h>
#include <stddef.h>

PixelDeltas compute_pixel_deltas_location(ViewportVectors uv, ImageSize size) {
  double image_width = (double)size.width;
  double image_height = (double)size.height;
  PixelDeltas dudv = {.du = vec3_div(uv.u, image_width),
                      .dv = vec3_div(uv.v, image_height)};
  return dudv;
}

Vec3 camera_compute_viewport_upper_left(const CameraSystem *cs) {
  ViewportVectors uv = cs->viewport.uv;
  // camera_center - vec3{0,0,focal_lenght} - viewport_u / 2 - viewport_v / 2
  Vec3 distance_from_center = {.x = 0.0, .y = 0.0, .z = cs->focal_length};
  Vec3 viewport_add = vec3_add(uv.u, uv.v);

  // camera_center - (ditance_from_center + 0.5 (viewport_u + viewport_v))
  return vec3_sub(cs->center,
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

#define MAX_HITS 10

Color ray_color(Ray r, const Spheres *s, size_t n_spheres) {
  HitInfo front_facing[MAX_HITS] = {0};
  HitInfo non_front_facing[MAX_HITS] = {0};
  Interval interval = {.tmin = 0.0, .tmax = INFINITY};

  Hits h = hit_spheres(s, n_spheres, r, front_facing, MAX_HITS,
                       non_front_facing, MAX_HITS, interval);
  if (h.hit_anything) {
    return (Color){
        .r = 0.5 * (h.normal.x + 1),
        .g = 0.5 * (h.normal.y + 1),
        .b = 0.5 * (h.normal.z + 1),
    };
  }

  Vec3 unit_direction = vec3_normalize(r.direction);
  double a = 0.5 * (unit_direction.y + 1.0);
  return (Color){.r = (1.0 - a) + (a * 0.5),
                 .g = (1.0 - a) + (a * 0.7),
                 .b = (1.0 - a) + (a * 1.0)};
}

void render(const CameraSystem *cs, ImageSize s, Spheres* world, size_t world_size) {
  PixelDeltas dudv = compute_pixel_deltas_location(cs->viewport.uv, s);
  Vec3 viewport_upper_left = camera_compute_viewport_upper_left(cs);
  Vec3 pixel00_loc = compute_pixel_00_location(viewport_upper_left, dudv);

  printf("P3\n%zu %zu\n255\n", s.width, s.height);
  for (size_t j = 0; j < s.height; ++j) {
    for (size_t i = 0; i < s.width; ++i) {
      ImagePos pos = {.col = i, .row = j};
      Vec3 pixel_center = compute_pixel_center(pixel00_loc, dudv, pos);
      Vec3 ray_direction = vec3_sub(pixel_center, cs->center);
      Ray r = {.direction = ray_direction, .origin = cs->center};
      Color pixel = ray_color(r, world, world_size);
      color_write(stdout, pixel);
    }
  }
}
