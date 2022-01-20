#ifndef __HIT_RECORD_H__
#define __HIT_RECORD_H__

#include <stdbool.h>

#include "ray.h"
#include "vec.h"

typedef struct material_struct_t material_t;

typedef struct {
    point_t p;
    vec_t normal;
    float t;
    bool front_face;
    material_t* material;
} hit_record_t;

void set_face_normal(hit_record_t* rec, ray_t r, vec_t outward_normal);

#endif //__HIT_RECORD_H__
