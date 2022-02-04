#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec.h"

typedef struct {
    point_t origin;
    point_t lower_left_corner;
    vec_t horizontal;
    vec_t vertical;
    vec_t u;
    vec_t v;
    vec_t w;
    float lens_radius;
} camera_t;

void camera_init(camera_t* cam, point_t lookfrom, point_t lookat, vec_t vup, float vfov, float aspect_ratio, float aperture, float focus_dist);
ray_t camera_get_ray(camera_t* cam, float u, float v);

#endif //CAMERA_H
