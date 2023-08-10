#ifndef VEC3_H_
#define VEC3_H_

typedef struct {
    double x;
    double y;
    double z;
} vec3_t;

vec3_t vec3_neg(vec3_t v);
vec3_t vec3_add(vec3_t lhs, vec3_t rhs);
vec3_t vec3_sub(vec3_t lhs, vec3_t rhs);
vec3_t vec3_cross(vec3_t lhs, vec3_t rhs);
vec3_t vec3_prod(vec3_t lhs, vec3_t rhs);
vec3_t vec3_mul(vec3_t v, double multiplier);
vec3_t vec3_div(vec3_t v, double divisor);
vec3_t vec3_normalize(vec3_t v);
double vec3_dot(vec3_t lhs, vec3_t rhs);
double vec3_norm_squared(vec3_t v);
double vec3_norm(vec3_t v);

#endif
