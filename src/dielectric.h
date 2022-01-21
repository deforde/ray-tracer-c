#ifndef __DIELECTRIC_H__
#define __DIELECTRIC_H__

#include <stdbool.h>

#include "hit_record.h"
#include "material.h"
#include "ray.h"
#include "vec.h"

typedef struct {
    material_t material;
    float ir;
} dielectric_t;

void dielectric_init(dielectric_t* dielectric, float ir);
bool dielectric_scatter(void* object, ray_t r_in, hit_record_t rec, colour_t* attenuation, ray_t* scattered);

#endif //__DIELECTRIC_H__
