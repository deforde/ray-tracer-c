#include "camera.h"

#include <math.h>

#include "util.h"

void camera_init(camera_t *cam, point_t lookfrom, point_t lookat, vec_t vup,
                 float vfov, float aspect_ratio, float aperture,
                 float focus_dist) {
  const float theta = degrees_to_radians(vfov);
  const float h = tanf(theta / 2.0f);
  const float viewport_height = 2.0f * h;
  const float viewport_width = aspect_ratio * viewport_height;

  const vec_t x = vec_sub_v(&lookfrom, 1, (const vec_t[]){lookat});
  cam->w = vec_unit(&x);
  const vec_t y = vec_cross(&vup, &cam->w);
  cam->u = vec_unit(&y);
  cam->v = vec_cross(&cam->w, &cam->u);

  cam->origin = lookfrom;
  cam->horizontal = vec_mul_f(&cam->u, viewport_width * focus_dist);
  cam->vertical = vec_mul_f(&cam->v, viewport_height * focus_dist);
  const vec_t w = vec_div_f(&cam->horizontal, 2.0f);
  const vec_t z = vec_div_f(&cam->vertical, 2.0f);
  const vec_t d = vec_mul_f(&cam->w, focus_dist);
  cam->lower_left_corner = vec_sub_v(&cam->origin, 3, (const vec_t[]){w, z, d});

  cam->lens_radius = aperture / 2.0f;
}

ray_t camera_get_ray(camera_t *cam, float u, float v) {
  const vec_t x = vec_random_in_unit_disk();
  const vec_t rd = vec_mul_f(&x, cam->lens_radius);
  const vec_t y = vec_mul_f(&cam->u, rd.x);
  const vec_t w = vec_mul_f(&cam->v, rd.y);
  const vec_t offset = vec_add_v(&y, 1, (const vec_t[]){w});

  const vec_t hori = vec_mul_f(&cam->horizontal, u);
  const vec_t vert = vec_mul_f(&cam->vertical, v);
  const vec_t sum =
      vec_add_v(&cam->lower_left_corner, 2, (const vec_t[]){hori, vert});
  ray_t r = {.orig = vec_add_v(&cam->origin, 1, (const vec_t[]){offset}),
             .dir = vec_sub_v(&sum, 2, (const vec_t[]){cam->origin, offset})};

  return r;
}
