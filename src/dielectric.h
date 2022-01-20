#ifndef __DIELECTRIC_H__
#define __DIELECTRIC_H__

#include "hit_record.h"
#include "ray.h"
#include "vec.h"

typedef struct {
    float ir;
} dielectric_t;

bool dielectric_scatter(void* object, ray_t r_in, hit_record_t rec, colour_t* attenuation, ray_t* scattered);

#endif //__DIELECTRIC_H__
