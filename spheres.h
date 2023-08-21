#ifndef SPHERES_H_
#define SPHERES_H_
#include "stdbool.h"
#include "vec3.h"

#define N_SPHERES 1

typedef struct {
  Vec3 point;
  Vec3 normal;
  double t;
} HitInfo;

typedef struct {
  Vec3 centers[N_SPHERES];
  Vec3 radiuses[N_SPHERES];
} Spheres;

typedef struct {
  HitInfo info;
  bool is_hit;
} HitResult;

typedef struct {
  double tmin;
  double tmax;
} HitValidInterval;

HitResult hit_spheres(const Spheres *s, HitValidInterval interval);

#endif
