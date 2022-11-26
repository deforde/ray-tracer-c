#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include <stdbool.h>

#include "ray.h"
#include "vec.h"

typedef struct material_struct_t material_t;

typedef struct {
  point_t p;
  vec_t normal;
  float t;
  bool front_face;
  material_t *material;
} hit_record_t;

void set_face_normal(hit_record_t *rec, ray_t *r, const vec_t *outward_normal);

#endif // HIT_RECORD_H
