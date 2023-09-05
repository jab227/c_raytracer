#include "spheres.h"
#include "ray.h"
#include "vec3.h"

#include <assert.h>
#include <math.h>
#include <stddef.h>

Hits hit_spheres(const Spheres *s, size_t n_spheres, Ray r,
                 HitInfo *front_facing, size_t n_front_facing,
                 HitInfo *non_front_facing, size_t n_non_front_facing,
                 Interval interval) {

  assert(s != NULL);
  assert(front_facing != NULL);
  assert(non_front_facing != NULL);

  size_t front_facing_idx = 0;
  size_t non_front_facing_idx = 0;
  bool hit_anything = false;
  Vec3 last_normal = {0};

  for (size_t i = 0; i < n_spheres; ++i) {
    double radius = s->radiuses[i];
    Vec3 center = s->centers[i];

    Vec3 oc = vec3_sub(r.origin, center);
    double a = vec3_norm_squared(r.direction);
    double half_b = vec3_dot(oc, r.direction);
    double c = vec3_norm_squared(oc) - radius * radius;

    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
      continue;
    }
    double sqrt_discriminant = sqrt(discriminant);
    double root = (-half_b - sqrt_discriminant) / a;

    if (!INTERVAL_SURROUNDS(interval, root)) {
      root = (-half_b + sqrt_discriminant) / a;
      if (!INTERVAL_SURROUNDS(interval, root)) {
        continue;
      }
    }

    hit_anything = true;
    Vec3 hit_point = ray_at(r, root);
    Vec3 outward_normal = vec3_div(vec3_sub(hit_point, center), radius);

    if (vec3_dot(r.direction, outward_normal) < 0.0) {
      assert(front_facing_idx < n_front_facing);
      last_normal = outward_normal;
      front_facing[front_facing_idx] =
          (HitInfo){.normal = outward_normal, .point = hit_point, .t = root};
      ++front_facing_idx;
    } else {
      assert(non_front_facing_idx < n_non_front_facing);
      last_normal = vec3_neg(outward_normal);
      non_front_facing[non_front_facing_idx] = (HitInfo){
          .normal = vec3_neg(outward_normal), .point = hit_point, .t = root};
      ++n_non_front_facing;
    }
  }

  return (Hits){.front_facing_hits = front_facing_idx,
                .non_front_facing_hits = non_front_facing_idx,
                .hit_anything = hit_anything,
                .normal = last_normal};
}
