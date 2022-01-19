#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "ray.h"
#include "vec.h"

typedef struct {
    point_t origin;
    point_t lower_left_corner;
    vec_t horizontal;
    vec_t vertical;
} camera_t;

void camera_init(camera_t* cam);
ray_t camera_get_ray(camera_t* cam, float u, float v);

#endif //__CAMERA_H__
