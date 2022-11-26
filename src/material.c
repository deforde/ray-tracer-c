#include "material.h"

bool material_scatter(material_t *material, ray_t *r_in, hit_record_t *rec,
                      colour_t *attenuation, ray_t *scattered) {
  return material->vtable->scatter_func(material, r_in, rec, attenuation,
                                        scattered);
}
