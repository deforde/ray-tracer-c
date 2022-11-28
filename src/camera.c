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

  const vec_t x = VEC_SUB_V(lookfrom, lookat);
  cam->w = vec_unit(&x);
  const vec_t y = vec_cross(&vup, &cam->w);
  cam->u = vec_unit(&y);
  cam->v = vec_cross(&cam->w, &cam->u);

  cam->origin = lookfrom;
  cam->horizontal = vec_mul_f(&cam->u, viewport_width * focus_dist);
  cam->vertical = vec_mul_f(&cam->v, viewport_height * focus_dist);
  cam->lower_left_corner = VEC_SUB_V(
      cam->origin, vec_div_f(&cam->horizontal, 2.0f),
      vec_div_f(&cam->vertical, 2.0f), vec_mul_f(&cam->w, focus_dist));

  cam->lens_radius = aperture / 2.0f;
}

ray_t camera_get_ray(camera_t *cam, float u, float v) {
  const vec_t x = vec_random_in_unit_disk();
  const vec_t rd = vec_mul_f(&x, cam->lens_radius);
  const vec_t offset =
      VEC_ADD_V(vec_mul_f(&cam->u, rd.x), vec_mul_f(&cam->v, rd.y));

  const vec_t sum =
      VEC_ADD_V(cam->lower_left_corner, vec_mul_f(&cam->horizontal, u),
                vec_mul_f(&cam->vertical, v));
  ray_t r = {.orig = VEC_ADD_V(cam->origin, offset),
             .dir = VEC_SUB_V(sum, cam->origin, offset)};

  return r;
}
