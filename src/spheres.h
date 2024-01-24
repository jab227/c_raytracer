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
    MATERIAL_TYPE_METAL = 1,
    MATERIAL_TYPE_DIELECTRIC = 2,
} Material_Type;

typedef struct
{
    Color albedo;
    double coefficient;
    Material_Type type;
} Material;

typedef struct
{
    Material material;
    Vec3 center;
    double radius;
} Sphere;

typedef struct
{
    const Sphere *data;
    size_t len;
} Sphere_View;

typedef struct
{
    Vec3 normal;
    Vec3 point;
    Material material;
    int hit_anything;
    int is_front_face;
} Hits;

typedef struct
{
    Ray scattered;
    Color attenuation;
    int hit_anything;
} Scatter_Result;


Sphere_View sphere_view_from_ptr(const Sphere *data, size_t len);
Hits sphere_hit(Sphere_View s, Ray r, Interval interval);
Scatter_Result material_scatter(Ray r, const Hits *record, double epsilon);
#endif
