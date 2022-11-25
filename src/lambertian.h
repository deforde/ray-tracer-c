#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include <stdbool.h>

#include "hit_record.h"
#include "material.h"
#include "ray.h"
#include "vec.h"

typedef struct {
    material_t material;
    colour_t albedo;
} lambertian_t;

void lambertian_init(lambertian_t* lambertian, colour_t albedo);
bool lambertian_scatter(void* object, ray_t *r_in, hit_record_t *rec, colour_t* attenuation, ray_t* scattered);

#endif //LAMBERTIAN_H
