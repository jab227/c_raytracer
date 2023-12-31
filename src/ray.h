#ifndef RAY_H_
#define RAY_H_

#include "vec3.h"
typedef struct
{
    Vec3 origin;
    Vec3 direction;
} Ray;

Vec3 ray_at(Ray r, double t);

#endif
