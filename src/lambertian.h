#ifndef __LAMBERTIAN_H__
#define __LAMBERTIAN_H__

#include "hit_record.h"
#include "ray.h"
#include "vec.h"

typedef struct {
    colour_t albedo;
} lambertian_t;

bool lambertian_scatter(void* object, ray_t r_in, hit_record_t rec, colour_t* attenuation, ray_t* scattered);

#endif //__LAMBERTIAN_H__
