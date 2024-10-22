#ifndef VEC3_H_
#define VEC3_H_

typedef union
{
    struct
    {
        double x;
        double y;
        double z;
    };

    struct
    {
        double r;
        double g;
        double b;
    };

    double v[3];
} Vec3;

Vec3 vec3_neg(Vec3 v);
Vec3 vec3_add(Vec3 lhs, Vec3 rhs);
Vec3 vec3_sub(Vec3 lhs, Vec3 rhs);
Vec3 vec3_cross(Vec3 lhs, Vec3 rhs);
Vec3 vec3_prod(Vec3 lhs, Vec3 rhs);
Vec3 vec3_mul(Vec3 v, double multiplier);
Vec3 vec3_div(Vec3 v, double divisor);
Vec3 vec3_normalize(Vec3 v);
Vec3 vec3_lerp(Vec3 c, double a);
Vec3 vec3_random(void);
Vec3 vec3_random_in(double min, double max);
Vec3 vec3_random_in_unit_sphere(void);
Vec3 vec3_random_unit_vec_in_unit_sphere(void);
Vec3 vec3_random_on_hemisphere(Vec3 normal);
Vec3 vec3_random_in_unit_disk(void);
double vec3_dot(Vec3 lhs, Vec3 rhs);
double vec3_norm_squared(Vec3 v);
double vec3_norm(Vec3 v);
int vec3_near_zero(Vec3 v, double epsilon);
#endif
