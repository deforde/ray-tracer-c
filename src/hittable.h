#ifndef __HITTABLE_H__
#define __HITTABLE_H__

#include <stddef.h>

#include "hit_record.h"
#include "ray.h"

typedef bool (*hit_func_t)(void* object, ray_t r, float t_min, float t_max, hit_record_t* rec);

typedef struct {
    void* object;
    hit_func_t hit_func;
} hittable_t;

#endif //__HITTABLE_H__
