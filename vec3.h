#ifndef VEC3_H_
#define VEC3_H_

typedef struct {
    double x;
    double y;
    double z;
} Vec3;

Vec3 vec3_neg(Vec3 v);
Vec3 vec3_add(Vec3 lhs, Vec3 rhs);
Vec3 vec3_sub(Vec3 lhs, Vec3 rhs);
Vec3 vec3_cross(Vec3 lhs, Vec3 rhs);
Vec3 vec3_prod(Vec3 lhs, Vec3 rhs);
Vec3 vec3_mul(Vec3 v, double multiplier);
Vec3 vec3_div(Vec3 v, double divisor);
Vec3 vec3_normalize(Vec3 v);
double vec3_dot(Vec3 lhs, Vec3 rhs);
double vec3_norm_squared(Vec3 v);
double vec3_norm(Vec3 v);

#endif
