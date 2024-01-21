#ifndef SPHERES_H_
#define SPHERES_H_
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#include <stddef.h>


typedef enum
{
    MATERIAL_METAL = 0,
    MATERIAL_LAMBERTIAN,
} Material;

typedef struct
{
    Vec3 center;
    double radius;
    Material material;
} Sphere;

typedef struct
{
    Vec3 normal;
    Vec3 point;
    int hit_anything;
} Hits;

Hits hit_spheres(const Sphere *s, size_t n_spheres, Ray r, Interval interval);

#endif
