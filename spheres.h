#ifndef SPHERES_H_
#define SPHERES_H_
#include "ray.h"
#include "vec3.h"

#include <stdbool.h>
#include <stddef.h>

#define N_SPHERES 2

typedef struct {
  Vec3 point;
  Vec3 normal;
  double t;
} HitInfo;

typedef struct {
  Vec3 centers[N_SPHERES];
  double radiuses[N_SPHERES];
} Spheres;

typedef struct {
  double tmin;
  double tmax;
} HitValidInterval;

typedef struct {
  Vec3 normal;
  size_t front_facing_hits;
  size_t non_front_facing_hits;
  bool hit_anything;
} Hits;

Hits hit_spheres(const Spheres *s, size_t n_spheres, Ray r,
                 HitInfo *front_facing, size_t n_front_facing,
                 HitInfo *non_front_facing, size_t n_non_front_facing,
                 HitValidInterval interval);

#endif
