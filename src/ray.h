#ifndef __RAY_H__
#define __RAY_H__

#include "vec.h"

typedef struct {
    point_t orig;
    vec_t dir;
} ray_t;

point_t ray_at(ray_t r, float t);

#endif //__RAY_H__
