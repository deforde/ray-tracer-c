#include "ray.h"

point_t ray_at(ray_t *r, float t) {
  return VEC_ADD_V(r->orig, vec_mul_f(&r->dir, t));
}
