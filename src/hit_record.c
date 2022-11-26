#include "hit_record.h"

void set_face_normal(hit_record_t* rec, ray_t *r, const vec_t *outward_normal)
{
    rec->front_face = vec_dot(&r->dir, outward_normal) < 0.0f;
    rec->normal = rec->front_face ? *outward_normal : vec_mul_f(outward_normal, -1.0f);
}
