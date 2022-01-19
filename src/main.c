#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "sphere.h"
#include "util.h"
#include "vec.h"

colour_t ray_colour(ray_t r, hittable_list_t* world)
{
    hit_record_t rec;
    if(hittable_list_hit(world, r, 0.0f, FLT_MAX, &rec)) {
        colour_t c = { .x = 1, .y = 1, .z = 1 };
        return VEC_MUL_F(VEC_ADD_V(rec.normal, c), 0.5f);
    }
    vec_t unit_dir = vec_unit(r.dir);
    const float t = 0.5f * (unit_dir.y + 1.0f);
    const colour_t a = { .x = 1.0f, .y = 1.0f, .z = 1.0f};
    const colour_t b = { .x = 0.5f, .y = 0.7f, .z = 1.0f};
    return VEC_ADD_V(VEC_MUL_F(a, 1.0f - t), VEC_MUL_F(b, t));
}

int main()
{
    // Image
    const float aspect_ratio = 16.0f / 9.0f;
    const int32_t image_width = 400;
    const int32_t image_height = (int32_t)(image_width / aspect_ratio);

    // World
    hittable_list_t world;

    sphere_t sphere_0 = { .centre = {0.0f, 0.0f, -1.0f }, .radius = 0.5f };
    hittable_t hittable_sphere_0 = { .object = &sphere_0, .hit_func = sphere_hit };

    sphere_t sphere_1 = { .centre = {0.0f, -100.5f, -1.0f }, .radius = 100.0f };
    hittable_t hittable_sphere_1 = { .object = &sphere_1, .hit_func = sphere_hit };

    hittable_list_add(&world, hittable_sphere_0);
    hittable_list_add(&world, hittable_sphere_1);

    // Camera
    const float viewport_height = 2.0f;
    const float viewport_width = aspect_ratio * viewport_height;
    const float focal_length = 1.0f;

    const point_t origin = { .x = 0, .y = 0, .z = 0 };
    const vec_t horizontal = { .x = viewport_width, .y = 0, .z = 0 };
    const vec_t vertical = { .x = 0, .y = viewport_height, .z = 0 };
    const vec_t focal_point = { .x = 0, .y = 0, .z = focal_length };
    const vec_t lower_left_corner = VEC_SUB_V(origin, VEC_DIV_F(horizontal, 2.0f), VEC_DIV_F(vertical, 2.0f), focal_point);

    // Render
    FILE* image_file = fopen("image.ppm", "w");
    char buf[128] = {0};
    const size_t buf_len = snprintf(buf, sizeof(buf), "P3\n%u %u\n255\n", image_width, image_height);
    fwrite(buf, buf_len, 1, image_file);
    memset(buf, 0, sizeof(buf));

    // Render
    printf("Rendering...\n");
    for(int32_t i = (image_height - 1); i >= 0; --i) {
        printf("%3u%%\r", (uint32_t)(100.0f * (1.0f - (float)i / image_height)));
        fflush(stdout);
        for(int32_t j = 0; j < image_width; ++j) {
            const float u = (float)j / (image_width - 1);
            const float v = (float)i / (image_height - 1);
            ray_t r = {
                .orig = origin,
                .dir = VEC_SUB_V(VEC_ADD_V(lower_left_corner, VEC_MUL_F(horizontal, u), VEC_MUL_F(vertical, v)), origin)
            };
            colour_t pixel_colour = ray_colour(r, &world);
            write_colour(image_file, pixel_colour);
        }
    }
    printf("100%%\n");

    fclose(image_file);

    return EXIT_SUCCESS;
}
