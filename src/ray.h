#ifndef RAY_H
#define RAY_H

#include "vec.h"

typedef struct {
  point_t orig;
  vec_t dir;
} ray_t;

point_t ray_at(ray_t *r, float t);

#endif // RAY_H
