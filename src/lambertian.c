#include "lambertian.h"

material_vtable_t lambertian_vtable = {.scatter_func = lambertian_scatter};

void lambertian_init(lambertian_t *lambertian, colour_t albedo) {
  lambertian->material.vtable = &lambertian_vtable;
  lambertian->albedo = albedo;
}

bool lambertian_scatter(void *object, __attribute__((unused)) ray_t *r_in,
                        hit_record_t *rec, colour_t *attenuation,
                        ray_t *scattered) {
  lambertian_t *lambertian = (lambertian_t *)object;

  const vec_t rand_unit = vec_random_unit();
  vec_t scatter_direction =
      vec_add_v(&rec->normal, 1, (const vec_t[]){rand_unit});

  if (vec_near_zero(&scatter_direction))
    scatter_direction = rec->normal;

  scattered->orig = rec->p;
  scattered->dir = scatter_direction;
  *attenuation = lambertian->albedo;

  return true;
}
