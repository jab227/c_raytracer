#include "spheres.h"

#include <assert.h>
#include <math.h>
#include <stddef.h>

Hits
sphere_hit(const Sphere *s, size_t n_spheres, Ray r, Interval interval)
{
    assert(s != NULL);
    int hit_anything = 0;
    Vec3 last_normal = { 0 };
    Vec3 hit_point = { 0 };
    Material last_material = { 0 };

    for (size_t i = 0; i < n_spheres; ++i) {
        double radius = s[i].radius;
        Vec3 center = s[i].center;
        Material current_material = s[i].material;
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
        last_material = current_material;
    }

    return (Hits){
        .hit_anything = hit_anything,
        .material = last_material,
        .normal = last_normal,
        .point = hit_point,
    };
}


static Scatter_Result
sphere__scatter_lambertian(Ray r, const Hits *record, double epsilon)
{
    (void) r;

    Vec3 rand_vec = vec3_random_on_hemisphere(record->normal);
    Vec3 scatter_direction = vec3_add(record->point, rand_vec);
    if (vec3_near_zero(scatter_direction, epsilon)) {
        scatter_direction = record->normal;
    }

    Ray scattered = {
        .direction = scatter_direction,
        .origin = record->point,
    };

    return (Scatter_Result){
        .scattered = scattered,
        .attenuation = record->material.albedo,
        .hit_anything = 1,
    };
}

static Scatter_Result
sphere__scatter_metal(Ray r, const Hits *record, double epsilon)
{
    (void) epsilon;
    Vec3 unit_dir = vec3_normalize(r.direction);
    Vec3 reflected_direction = vec3_reflect_normal(unit_dir, record->normal);

    Ray scattered = {
        .direction = reflected_direction,
        .origin = record->point,
    };

    return (Scatter_Result){
        .scattered = scattered,
        .attenuation = record->material.albedo,
        .hit_anything = 1,
    };
}

Scatter_Result
sphere_scatter(Ray r, const Hits *record, double epsilon)
{
    Scatter_Result result = { 0 };
    switch (record->material.type) {
    case MATERIAL_TYPE_LAMBERTIAN:
        result = sphere__scatter_lambertian(r, record, epsilon);
        break;
    case MATERIAL_TYPE_METAL:
        result = sphere__scatter_metal(r, record, epsilon);
        break;
    };
    return result;
}
