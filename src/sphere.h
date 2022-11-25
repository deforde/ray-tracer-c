#ifndef SPHERE_H
#define SPHERE_H

#include <stdbool.h>

#include "material.h"
#include "hit_record.h"
#include "hittable.h"
#include "ray.h"
#include "vec.h"

typedef struct {
    hittable_t hittable;
    point_t centre;
    float radius;
    material_t* material;
} sphere_t;

void sphere_init(sphere_t* sphere, point_t centre, float radius, material_t* material);
bool sphere_hit(void* object, ray_t *r, float t_min, float t_max, hit_record_t* rec);

#endif //SPHERE_H
