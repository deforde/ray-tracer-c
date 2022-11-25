#include "camera.h"

#include <math.h>

#include "util.h"

void camera_init(camera_t* cam, point_t lookfrom, point_t lookat, vec_t vup, float vfov, float aspect_ratio, float aperture, float focus_dist)
{
    const float theta = degrees_to_radians(vfov);
    const float h = tanf(theta / 2.0f);
    const float viewport_height = 2.0f * h;
    const float viewport_width = aspect_ratio * viewport_height;

    const vec_t x = VEC_SUB_V(&lookfrom, &lookat);
    cam->w = vec_unit(&x);
    const vec_t y = vec_cross(&vup, &cam->w);
    cam->u = vec_unit(&y);
    cam->v = vec_cross(&cam->w, &cam->u);

    cam->origin = lookfrom;
    cam->horizontal = VEC_MUL_F(&cam->u, viewport_width, focus_dist);
    cam->vertical = VEC_MUL_F(&cam->v, viewport_height, focus_dist);
    const vec_t w = VEC_DIV_F(&cam->horizontal, 2.0f);
    const vec_t z = VEC_DIV_F(&cam->vertical, 2.0f);
    const vec_t d = VEC_MUL_F(&cam->w, focus_dist);
    cam->lower_left_corner = VEC_SUB_V(&cam->origin, &w, &z, &d);

    cam->lens_radius = aperture / 2.0f;
}

ray_t camera_get_ray(camera_t* cam, float u, float v)
{
    const vec_t x = vec_random_in_unit_disk();
    const vec_t rd = VEC_MUL_F(&x, cam->lens_radius);
    const vec_t y = VEC_MUL_F(&cam->u, rd.x);
    const vec_t w = VEC_MUL_F(&cam->v, rd.y);
    const vec_t offset = VEC_ADD_V(&y, &w);

    const vec_t hori = VEC_MUL_F(&cam->horizontal, u);
    const vec_t vert = VEC_MUL_F(&cam->vertical, v);
    const vec_t sum = VEC_ADD_V(&cam->lower_left_corner, &hori, &vert);
    ray_t r = {
        .orig = VEC_ADD_V(&cam->origin, &offset),
        .dir = VEC_SUB_V(&sum, &cam->origin, &offset)
    };

    return r;
}
