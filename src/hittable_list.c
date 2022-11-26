#include "hittable_list.h"

#include <assert.h>

void hittable_list_init(hittable_list_t *world) { world->n_objects = 0; }

void hittable_list_add(hittable_list_t *world, hittable_t *hittable) {
  assert(world->n_objects < MAX_NUM_HITTABLE_OBJECTS);
  world->objects[world->n_objects++] = hittable;
}

void hittable_list_clear(hittable_list_t *world) { world->n_objects = 0; }

bool hittable_list_hit(hittable_list_t *world, ray_t *r, float t_min,
                       float t_max, hit_record_t *rec) {
  hit_record_t temp_rec;
  bool hit_anything = false;
  float closest_so_far = t_max;

  for (size_t i = 0; i < world->n_objects; ++i) {
    hittable_t *hittable = world->objects[i];
    if (hittable_hit(hittable, r, t_min, closest_so_far, &temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      *rec = temp_rec;
    }
  }

  return hit_anything;
}
