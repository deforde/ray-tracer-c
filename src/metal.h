#ifndef __METAL_H__
#define __METAL_H__

#include "hit_record.h"
#include "ray.h"
#include "vec.h"

typedef struct {
    colour_t albedo;
} metal_t;

bool metal_scatter(void* object, ray_t r_in, hit_record_t rec, colour_t* attenuation, ray_t* scattered);

#endif //__METAL_H__
