#include "camera.h"

#include <assert.h>
#include <stddef.h>

#define ASPECT_RATIO(width, height) ((double)width / (double)height)

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
  CameraSystem cs = {
      .focal_length = 1.0,
      .samples_per_pixel = 100,
      .center = {.x = 0.0, .y = 0.0, .z = 0.0},
      .viewport = {.size = {.height = viewport_height, .width = viewport_width},
                   .uv = {.u = {.x = viewport_width, .y = 0.0, .z = 0.0},
                          .v = {.x = 0.0, .y = -viewport_height, .z = 0.0}}}};

  // What
  Spheres world = {
      .centers = {{.x = 0.0, .y = -100.5, .z = -1.0},
                  {.x = 0.0, .y = 0.0, .z = -1.0}},
      .radiuses = {100.0, 0.5},
  };
  render(&cs, size, &world, 2);
  return 0;
}
