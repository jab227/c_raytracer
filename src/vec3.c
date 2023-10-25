#include "vec3.h"
#include <math.h>
Vec3
vec3_neg(Vec3 v)
{
    return (Vec3){ .x = -v.x, .y = -v.y, .z = -v.z };
}

Vec3
vec3_add(Vec3 lhs, Vec3 rhs)
{
    return (Vec3){ .x = rhs.x + lhs.x, .y = rhs.y + lhs.y, .z = rhs.z + lhs.z };
}

Vec3
vec3_sub(Vec3 lhs, Vec3 rhs)
{
    return (Vec3){ .x = lhs.x - rhs.x, .y = lhs.y - rhs.y, .z = lhs.z - rhs.z };
}

Vec3
vec3_prod(Vec3 lhs, Vec3 rhs)
{
    return (Vec3){ .x = lhs.x * rhs.x, .y = lhs.y * rhs.y, .z = lhs.z * rhs.z };
}

Vec3
vec3_div(Vec3 v, double divisor)
{
    return (Vec3){ .x = v.x / divisor, .y = v.y / divisor, .z = v.z / divisor };
}

Vec3
vec3_mul(Vec3 v, double multiplier)
{
    return (Vec3){ .x = v.x * multiplier,
                   .y = v.y * multiplier,
                   .z = v.z * multiplier };
}

Vec3
vec3_cross(Vec3 lhs, Vec3 rhs)
{
    return (Vec3){
        .x = lhs.y * rhs.z - lhs.z * rhs.y,
        .y = lhs.z * rhs.x - lhs.x * rhs.z,
        .z = lhs.x * rhs.y - lhs.y * rhs.x,
    };
}

double
vec3_dot(Vec3 lhs, Vec3 rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

double
vec3_norm_squared(Vec3 v)
{
    return vec3_dot(v, v);
}

double
vec3_norm(Vec3 v)
{
    return sqrt(vec3_norm_squared(v));
}

Vec3
vec3_normalize(Vec3 v)
{
    return vec3_div(v, vec3_norm(v));
}
