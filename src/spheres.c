#include "spheres.h"
#include "prng.h"
#include "vec3.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>

Hits
sphere_hit(Sphere_View spheres, Ray r, Interval interval)
{
    int hit_anything = 0;
    int is_front_face = 0;
    Vec3 last_normal = { 0 };
    Vec3 hit_point = { 0 };
    Material last_material = { 0 };

    for (size_t i = 0; i < spheres.len; ++i) {
        double radius = spheres.data[i].radius;
        Vec3 center = spheres.data[i].center;
         Material current_material = spheres.data[i].material;
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
            is_front_face = 1;
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
        .is_front_face = is_front_face,
    };
}

static Vec3
material__reflect(Vec3 v, Vec3 normal)
{
    // v - 2*<v,normal>*normal
    double v_dot_n = vec3_dot(v, normal);
    Vec3 two_b = vec3_mul(normal, v_dot_n * 2);
    return vec3_sub(v, two_b);
}

static Vec3
material__refract(Vec3 incident_dir, Vec3 normal, double eta_coeff)
{
    double dot_prod = vec3_dot(vec3_neg(incident_dir), normal);
    double cos_theta = fmin(dot_prod, 1.0);
    Vec3 sum = vec3_add(incident_dir, vec3_mul(normal, cos_theta));
    Vec3 perpendicular_out_ray_dir = vec3_mul(sum, eta_coeff);
    double x = fabs(1.0 - vec3_norm_squared(perpendicular_out_ray_dir));
    double parallel_out_ray_abs = -sqrt(x);
    Vec3 parallel_out_ray_dir = vec3_mul(normal, parallel_out_ray_abs);
    return vec3_add(parallel_out_ray_dir, perpendicular_out_ray_dir);
}


static Scatter_Result
material__scatter_lambertian(Ray r, const Hits *record, double epsilon)
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
material__scatter_metal(Ray r, const Hits *record, double epsilon)
{
    (void) epsilon;
    Vec3 unit_dir = vec3_normalize(r.direction);
    Vec3 reflected_direction = material__reflect(unit_dir, record->normal);
    Vec3 fuzz_vec = vec3_mul(vec3_random_unit_vec_in_unit_sphere(),
                             record->material.coefficient);
    Ray scattered = {
        .direction = vec3_add(reflected_direction, fuzz_vec),
        .origin = record->point,
    };

    return (Scatter_Result){
        .scattered = scattered,
        .attenuation = record->material.albedo,
        .hit_anything = vec3_dot(scattered.direction, record->normal) > 0,
    };
}

static double
material__reflectance(double cosine, double refraction_idx)
{
    // Schlicks approximation for reflectance
    double r0 = (1.0 - refraction_idx) / (1.0 + refraction_idx);
    r0 = r0 * r0;
    double x = 1.0 - cosine;
    return r0 + ((1.0 - r0) * (x * x * x * x * x));
}

static Scatter_Result
material__scatter_dielectric(Ray r, const Hits *record, double epsilon)
{
    (void) epsilon;
    Color attenuation = { 1.0, 1.0, 1.0 };
    double coeff = record->material.coefficient;
    double refraction_ratio = record->is_front_face ? (1.0 / coeff) : coeff;

    Vec3 unit_dir = vec3_normalize(r.direction);
    Vec3 normal = record->normal;

    double dot_prod = vec3_dot(vec3_neg(unit_dir), normal);
    double cos_theta = fmin(dot_prod, 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    int cannot_refract = refraction_ratio * sin_theta > 1.0;
    int reflect = material__reflectance(cos_theta, refraction_ratio) > randomd();
    Vec3 direction = { 0 };
    if (cannot_refract || reflect) {
        direction = material__reflect(unit_dir, normal);
    } else {
        direction = material__refract(unit_dir, normal, refraction_ratio);
    }

    Ray scattered = {
        .direction = direction,
        .origin = record->point,
    };
    return (Scatter_Result){
        .scattered = scattered,
        .attenuation = attenuation,
        .hit_anything = 1,
    };
}

Scatter_Result
material_scatter(Ray r, const Hits *record, double epsilon)
{
    Scatter_Result result = { 0 };
    switch (record->material.type) {
    case MATERIAL_TYPE_LAMBERTIAN:
        result = material__scatter_lambertian(r, record, epsilon);
        break;
    case MATERIAL_TYPE_METAL:
        result = material__scatter_metal(r, record, epsilon);
        break;
    case MATERIAL_TYPE_DIELECTRIC:
        result = material__scatter_dielectric(r, record, epsilon);
        break;
    };
    return result;
}

Sphere_View
sphere_view_from_ptr(const Sphere *data, size_t len)
{
    Sphere_View view = { .data = data, .len = len };
    return view;
}
