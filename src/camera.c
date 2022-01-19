#include "camera.h"

void camera_init(camera_t* cam)
{
    const float aspect_ratio = 16.0f / 9.0f;
    const float viewport_height = 2.0f;
    const float viewport_width = aspect_ratio * viewport_height;
    const float focal_length = 1.0f;

    cam->origin.x = 0;
    cam->origin.y = 0;
    cam->origin.z = 0;

    cam->horizontal.x = viewport_width;
    cam->horizontal.y = 0;
    cam->horizontal.z = 0;

    cam->vertical.x = 0;
    cam->vertical.y = viewport_height;
    cam->vertical.z = 0;

    vec_t focal_point = { .x = 0, .y = 0, .z = focal_length };

    cam->lower_left_corner = VEC_SUB_V(cam->origin, VEC_DIV_F(cam->horizontal, 2.0f), VEC_DIV_F(cam->vertical, 2.0f), focal_point);
}

ray_t camera_get_ray(camera_t* cam, float u, float v)
{
    ray_t r = {
        .orig = cam->origin,
        .dir = VEC_SUB_V(VEC_ADD_V(cam->lower_left_corner, VEC_MUL_F(cam->horizontal, u), VEC_MUL_F(cam->vertical, v)), cam->origin)
    };
    return r;
}
