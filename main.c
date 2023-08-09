#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  double x;
  double y;
  double z;
} vec3_t;




int main(void) {
  const size_t image_width = 256;
  const size_t image_height = 256;

  printf("P3\n%zu %zu\n255\n", image_width, image_height);
  for (size_t j = image_height - 1; j != (size_t)-1; --j) {
    for (size_t i = 0; i < image_width; ++i) {
      double r = (double)i / (double)(image_width - 1);
      double g = (double)j / (double)(image_height - 1);
      double b = 0;

      uint8_t ru8 = (uint8_t)(255.999 * r);
      uint8_t gu8 = (uint8_t)(255.999 * g);
      uint8_t bu8 = (uint8_t)(255.999 * b);

      printf("%d %d %d\n", ru8, gu8, bu8);
    }
  }
  return 0;
}
