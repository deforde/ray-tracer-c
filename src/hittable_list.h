#ifndef __HITTABLE_LIST_H__
#define __HITTABLE_LIST_H__

#include <stddef.h>

#include "hit_record.h"
#include "hittable.h"
#include "ray.h"

#define MAX_NUM_HITTABLE_OBJECTS 4096

typedef struct {
    hittable_t* objects[MAX_NUM_HITTABLE_OBJECTS];
    size_t n_objects;
} hittable_list_t;

void hittable_list_init(hittable_list_t* world);
void hittable_list_add(hittable_list_t* world, hittable_t* hittable);
void hittable_list_clear(hittable_list_t* world);
bool hittable_list_hit(hittable_list_t* world, ray_t r, float t_min, float t_max, hit_record_t* rec);

#endif //__HITTABLE_LIST_H__
