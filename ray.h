#ifndef RAY_H_
#define RAY_H_

#include "vec3.h"
typedef struct {
    Vec3 origin;
    Vec3 direction;
} ray_t;

Vec3 ray_at(ray_t r, double t);

#endif
