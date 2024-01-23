#ifndef SPHERES_H_
#define SPHERES_H_
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#include <stddef.h>


typedef enum
{
    MATERIAL_TYPE_LAMBERTIAN = 0,
    MATERIAL_TYPE_METAL,
} Material_Type;

typedef struct
{
    Color albedo;
    Material_Type type;
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
    Material material;
    int hit_anything;
} Hits;

typedef struct
{
    Ray scattered;
    Color attenuation;
    int hit_anything;
} Scatter_Result;


Hits sphere_hit(const Sphere *s, size_t n_spheres, Ray r, Interval interval);
Scatter_Result sphere_scatter(Ray r, const Hits *record, double epsilon);
#endif
