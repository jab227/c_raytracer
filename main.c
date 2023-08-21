#include "camera.h"
#include "color.h"
#include "ray.h"
#include "spheres.h"
#include "vec3.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define ASPECT_RATIO(width, height) ((double)width / (double)height)
#define MAX_HITS 10
Color ray_color(Ray r, const Spheres *s, size_t n_spheres) {
  HitInfo front_facing[MAX_HITS] = {0};
  HitInfo non_front_facing[MAX_HITS] = {0};
  HitValidInterval interval = {.tmin = 0.0, .tmax = INFINITY};

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

int main(void) {
  const double aspect_ratio = 16.0 / 9.0;
  const size_t image_width = 400;
  size_t image_height = (size_t)((double)image_width / aspect_ratio);
  image_height = image_height < 1 ? 1 : image_height;
  assert(image_height >= 1);
  ImageSize size = {.width = image_width, .height = image_height};

  double viewport_height = 2.0;
  double real_ratio = ASPECT_RATIO(size.width, size.height);
  double viewport_width = viewport_height * real_ratio;

  // Camera settings
  Camera c = {
      .focal_length = 1.0,
      .center = {.x = 0.0, .y = 0.0, .z = 0.0},
      .viewport = {.size = {.height = viewport_height, .width = viewport_width},
                   .uv = {.u = {.x = viewport_width, .y = 0.0, .z = 0.0},
                          .v = {.x = 0.0, .y = -viewport_height, .z = 0.0}}}};

  PixelDeltas dudv = compute_pixel_deltas_location(c.viewport.uv, size);
  Vec3 viewport_upper_left = camera_compute_viewport_upper_left(c);
  Vec3 pixel00_loc = compute_pixel_00_location(viewport_upper_left, dudv);

  // What
  Spheres s = {
      .centers = {{.x = 0.0, .y = -100.5, .z = -1.0},
                  {.x = 0.0, .y = 0.0, .z = -1.0}},
      .radiuses = {100.0, 0.5},
  };

  printf("P3\n%zu %zu\n255\n", image_width, image_height);
  for (size_t j = 0; j < image_height; ++j) {
    for (size_t i = 0; i < image_width; ++i) {
      ImagePos pos = {.col = i, .row = j};
      Vec3 pixel_center = compute_pixel_center(pixel00_loc, dudv, pos);
      Vec3 ray_direction = vec3_sub(pixel_center, c.center);
      Ray r = {.direction = ray_direction, .origin = c.center};
      Color pixel = ray_color(r, &s, N_SPHERES);
      color_write(stdout, pixel);
    }
  }
  return 0;
}
