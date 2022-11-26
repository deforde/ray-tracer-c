#ifndef METAL_H
#define METAL_H

#include <stdbool.h>

#include "hit_record.h"
#include "material.h"
#include "ray.h"
#include "vec.h"

typedef struct {
  material_t material;
  colour_t albedo;
  float fuzz;
} metal_t;

void metal_init(metal_t *metal, colour_t albedo, float fuzz);
bool metal_scatter(void *object, ray_t *r_in, hit_record_t *rec,
                   colour_t *attenuation, ray_t *scattered);

#endif // METAL_H
