#include "dielectric.h"

#include <math.h>

#include "util.h"

material_vtable_t dielectric_vtable = {
    .scatter_func = dielectric_scatter
};

void dielectric_init(dielectric_t* dielectric, float ir)
{
    dielectric->material.vtable = &dielectric_vtable;
    dielectric->ir = ir;
}

bool dielectric_scatter(void* object, ray_t r_in, hit_record_t rec, colour_t* attenuation, ray_t* scattered)
{
    dielectric_t* dielectric = (dielectric_t*)object;

    attenuation->x = 1.0f;
    attenuation->y = 1.0f;
    attenuation->z = 1.0f;

    const float refraction_ratio = rec.front_face ? (1.0f / dielectric->ir) : dielectric->ir;

    const vec_t unit_dir = vec_unit(r_in.dir);

    const float dot_prod = vec_dot(VEC_MUL_F(unit_dir, -1.0f), rec.normal);
    const float cos_theta = dot_prod < 1.0f ? dot_prod : 1.0f;
    const float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);

    const bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
    vec_t dir;
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_f())
        dir = vec_reflect(unit_dir, rec.normal);
    else
        dir = vec_refract(unit_dir, rec.normal, refraction_ratio);

    scattered->orig = rec.p;
    scattered->dir = dir;

    return true;
}
