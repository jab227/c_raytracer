#include "spheres.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#include <assert.h>
#include <math.h>
#include <stddef.h>

Hits
hit_spheres(const Sphere *s, size_t n_spheres, Ray r, Interval interval)
{
    assert(s != NULL);
    int hit_anything = 0;
    Vec3 last_normal = { 0 };
    Vec3 hit_point = { 0 };

    for (size_t i = 0; i < n_spheres; ++i) {
        double radius = s[i].radius;
        Vec3 center = s[i].center;

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

        hit_anything = 1;
        hit_point = ray_at(r, root);
        Vec3 outward_normal = vec3_div(vec3_sub(hit_point, center), radius);

        if (vec3_dot(r.direction, outward_normal) < 0.0) {
            last_normal = outward_normal;
        } else {
            last_normal = vec3_neg(outward_normal);
        }
    }

    return (Hits){
        .hit_anything = hit_anything,
        .normal = last_normal,
        .point = hit_point,
    };
}
