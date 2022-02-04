#ifndef HITTABLE_H
#define HITTABLE_H

#include <stdbool.h>

#include "hit_record.h"
#include "ray.h"

typedef bool (*hittable_hit_func_t)(void* object, ray_t r, float t_min, float t_max, hit_record_t* rec);

typedef struct {
    hittable_hit_func_t hit_func;
} hittable_vtable_t;

typedef struct {
    hittable_vtable_t* vtable;
} hittable_t;

bool hittable_hit(hittable_t* hittable, ray_t r, float t_min, float t_max, hit_record_t* rec);

#endif //HITTABLE_H
