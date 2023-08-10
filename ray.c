#include "ray.h"

vec3_t ray_at(ray_t r, double t)
{
    return vec3_add(r.origin, vec3_mul(r.direction, t));
}
