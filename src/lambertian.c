#include "lambertian.h"

bool lambertian_scatter(void* object, ray_t r_in __attribute__((unused)), hit_record_t rec, colour_t* attenuation, ray_t* scattered)
{
    lambertian_t* lambertian = (lambertian_t*)object;

    vec_t scatter_direction = VEC_ADD_V(rec.normal, vec_random_unit());

    if(vec_near_zero(scatter_direction))
        scatter_direction = rec.normal;

    scattered->orig = rec.p;
    scattered->dir = scatter_direction;
    *attenuation = lambertian->albedo;

    return true;
}
