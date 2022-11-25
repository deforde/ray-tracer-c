#include "metal.h"

material_vtable_t metal_vtable = {
    .scatter_func = metal_scatter
};

void metal_init(metal_t* metal, colour_t albedo, float fuzz)
{
    metal->material.vtable = &metal_vtable;
    metal->albedo = albedo;
    metal->fuzz = fuzz;
}

bool metal_scatter(void* object, ray_t *r_in, hit_record_t *rec, colour_t* attenuation, ray_t* scattered)
{
    metal_t* metal = (metal_t*)object;

    const vec_t x = vec_unit(&r_in->dir);
    const vec_t reflected = vec_reflect(&x, &rec->normal);
    scattered->orig = rec->p;
    const vec_t y = vec_random_in_unit_sphere();
    const vec_t w = VEC_MUL_F(&y, metal->fuzz);
    scattered->dir = VEC_ADD_V(&reflected, &w);
    *attenuation = metal->albedo;

    return vec_dot(&scattered->dir, &rec->normal) > 0.0f;
}
