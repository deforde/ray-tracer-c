#include "ray.h"

point_t ray_at(ray_t *r, float t)
{
    const vec_t x = VEC_MUL_F(&r->dir, t);
    return VEC_ADD_V(&r->orig, &x);
}
