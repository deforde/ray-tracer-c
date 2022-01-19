#ifndef __HIT_RECORD_H__
#define __HIT_RECORD_H__

#include <stdbool.h>

#include "ray.h"
#include "vec.h"

typedef struct {
    point_t p;
    vec_t normal;
    float t;
    bool front_face;
} hit_record_t;

void set_face_normal(hit_record_t* rec, ray_t r, vec_t outward_normal);

#endif //__HIT_RECORD_H__
