#include "hittable.h"

bool hittable_hit(hittable_t* hittable, ray_t *r, float t_min, float t_max, hit_record_t* rec)
{
    return hittable->vtable->hit_func(hittable, r, t_min, t_max, rec);
}
