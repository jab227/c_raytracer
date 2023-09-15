#ifndef SPHERES_H_
#define SPHERES_H_
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#include <stdbool.h>
#include <stddef.h>

#define N_SPHERES 2

typedef struct {
  Vec3 centers[N_SPHERES];
  double radiuses[N_SPHERES];
} Spheres;


typedef struct {
  Vec3 normal;
  bool hit_anything;
} Hits;

Hits hit_spheres(const Spheres *s, size_t n_spheres, Ray r, Interval interval);

#endif
