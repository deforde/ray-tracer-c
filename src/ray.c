#include "ray.h"

point_t ray_at(ray_t *r, float t) {
  const vec_t x = vec_mul_f(&r->dir, t);
  return vec_add_v(&r->orig, 1, (const vec_t[]){x});
}
