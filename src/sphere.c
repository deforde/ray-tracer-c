#include "sphere.h"

#include <math.h>

hittable_vtable_t sphere_vtable = {.hit_func = sphere_hit};

void sphere_init(sphere_t *sphere, point_t centre, float radius,
                 material_t *material) {
  sphere->hittable.vtable = &sphere_vtable;
  sphere->centre = centre;
  sphere->radius = radius;
  sphere->material = material;
}

bool sphere_hit(void *object, ray_t *r, float t_min, float t_max,
                hit_record_t *rec) {
  sphere_t *sphere = (sphere_t *)object;

  const vec_t oc = VEC_SUB_V(r->orig, sphere->centre);
  const float a = vec_length_squared(&r->dir);
  const float half_b = vec_dot(&oc, &r->dir);
  const float c = vec_length_squared(&oc) - sphere->radius * sphere->radius;

  const float discriminant = half_b * half_b - a * c;
  if (discriminant < 0.0f) {
    return false;
  }
  const float sqrtd = sqrtf(discriminant);

  // Find the nearest root that lies in the acceptable range.
  float root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec->t = root;
  rec->p = ray_at(r, rec->t);
  const vec_t v = VEC_SUB_V(rec->p, sphere->centre);
  const vec_t outward_normal = vec_div_f(&v, sphere->radius);
  set_face_normal(rec, r, &outward_normal);
  rec->material = sphere->material;

  return true;
}
