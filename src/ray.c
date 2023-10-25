#include "ray.h"
#include "vec3.h"

Vec3
ray_at(Ray r, double t)
{
    return vec3_add(r.origin, vec3_mul(r.direction, t));
}
