#include "vec3.h"
#include <math.h>
vec3_t neg(vec3_t v)
{
    return (vec3_t){ .x = -v.x, .y = -v.y, .z = -v.z };
}

vec3_t vec3_add(vec3_t lhs, vec3_t rhs)
{
    return (vec3_t){ .x = rhs.x + lhs.x,
                     .y = rhs.y + lhs.y,
                     .z = rhs.z + lhs.z };
}

vec3_t vec3_sub(vec3_t lhs, vec3_t rhs)
{
    return (vec3_t){ .x = lhs.x - rhs.x,
                     .y = lhs.y - rhs.y,
                     .z = lhs.z - rhs.z };
}

vec3_t vec3_prod(vec3_t lhs, vec3_t rhs)
{
    return (vec3_t){ .x = lhs.x * rhs.x,
                     .y = lhs.y * rhs.y,
                     .z = lhs.z * rhs.z };
}

vec3_t vec3_div(vec3_t v, double divisor)
{
    return (vec3_t){ .x = v.x / divisor,
                     .y = v.y / divisor,
                     .z = v.z / divisor };
}

vec3_t vec3_mul(vec3_t v, double divisor)
{
    return (vec3_t){ .x = v.x * divisor,
                     .y = v.y * divisor,
                     .z = v.z * divisor };
}

vec3_t vec3_cross(vec3_t lhs, vec3_t rhs)
{
    return (vec3_t){
        .x = lhs.y * rhs.z - lhs.z * rhs.y,
        .y = lhs.z * rhs.x - lhs.x * rhs.z,
        .z = lhs.x * rhs.y - lhs.y * rhs.x,
    };
}

double vec3_dot(vec3_t lhs, vec3_t rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

double vec3_norm_squared(vec3_t v) { return vec3_dot(v, v); }

double vec3_norm(vec3_t v) { return sqrt(vec3_norm(v)); }

vec3_t vec3_normalize(vec3_t v) { return vec3_div(v, vec3_norm(v)); }
