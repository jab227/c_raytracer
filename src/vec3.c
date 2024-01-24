#include "vec3.h"
#include "prng.h"

#include <math.h>
Vec3
vec3_neg(Vec3 v)
{
    return (Vec3){
        .x = -v.x,
        .y = -v.y,
        .z = -v.z,
    };
}

Vec3
vec3_add(Vec3 lhs, Vec3 rhs)
{
    return (Vec3){
        .x = rhs.x + lhs.x,
        .y = rhs.y + lhs.y,
        .z = rhs.z + lhs.z,
    };
}

Vec3
vec3_sub(Vec3 lhs, Vec3 rhs)
{
    return (Vec3){
        .x = lhs.x - rhs.x,
        .y = lhs.y - rhs.y,
        .z = lhs.z - rhs.z,
    };
}

Vec3
vec3_prod(Vec3 lhs, Vec3 rhs)
{
    return (Vec3){
        .x = lhs.x * rhs.x,
        .y = lhs.y * rhs.y,
        .z = lhs.z * rhs.z,
    };
}

Vec3
vec3_div(Vec3 v, double divisor)
{
    return (Vec3){
        .x = v.x / divisor,
        .y = v.y / divisor,
        .z = v.z / divisor,
    };
}

Vec3
vec3_mul(Vec3 v, double multiplier)
{
    return (Vec3){
        .x = v.x * multiplier,
        .y = v.y * multiplier,
        .z = v.z * multiplier,
    };
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

Vec3
vec3_random_in(double min, double max)
{
    return (Vec3){
        .x = randomd_in(min, max),
        .y = randomd_in(min, max),
        .z = randomd_in(min, max),
    };
}

Vec3
vec3_random_in_unit_sphere()
{
    for (;;) {
        Vec3 v = vec3_random_in(-1.0, 1.0);
        if (vec3_norm_squared(v) < 1.0) {
            return v;
        }
    }
}

Vec3
vec3_random_unit_vec_in_unit_sphere()
{
    return vec3_normalize(vec3_random_in_unit_sphere());
}

Vec3
vec3_random_on_hemisphere(Vec3 normal)
{
    Vec3 v = vec3_random_unit_vec_in_unit_sphere();
    if (vec3_dot(v, normal) > 0.0) {
        return v;
    } else {
        return vec3_neg(v);
    }
}

int
vec3_near_zero(Vec3 v, double epsilon)
{
    return (fabs(v.x) < epsilon) && (fabs(v.y) < epsilon) &&
           (fabs(v.z) < epsilon);
}

