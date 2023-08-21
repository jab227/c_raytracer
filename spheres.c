#include <math.h>

#include "ray.h"
#include "spheres.h"
#include "vec3.h"

static HitResult hit_sphere(Vec3 center, double radius, Ray r,
                            HitValidInterval interval) {
  Vec3 oc = vec3_sub(r.origin, center);
  double a = vec3_norm_squared(r.direction);
  double half_b = vec3_dot(oc, r.direction);
  double c = vec3_norm_squared(oc) - radius * radius;

  double discriminant = half_b * half_b - a * c;

  if (discriminant < 0) {
    return (HitResult){.is_hit = false};
  }
  double sqrt_discriminant = sqrt(discriminant);
  double root = (-half_b - sqrt_discriminant) / a;

  if (root <= interval.tmin || root >= interval.tmax) {
    root = (-half_b + sqrt_discriminant) / a;
    if (root <= interval.tmin || root >= interval.tmax) {
      return (HitResult){.is_hit = false};
    }
  }

  Vec3 hit_point = ray_at(r, root);
  HitResult res = {
      .is_hit = true,
      .info = {.t = root,
               .point = hit_point,
               .normal = vec3_div(vec3_sub(hit_point, center), radius)}};

  return res;
}



