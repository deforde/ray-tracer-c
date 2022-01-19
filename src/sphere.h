#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "hit_record.h"
#include "ray.h"
#include "vec.h"

typedef struct {
    point_t centre;
    float radius;
} sphere_t;

bool sphere_hit(void* object, ray_t r, float t_min, float t_max, hit_record_t* rec);

#endif //__SPHERE_H__
