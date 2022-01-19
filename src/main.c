#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "sphere.h"
#include "util.h"
#include "vec.h"

colour_t ray_colour(ray_t r, hittable_list_t* world, int32_t depth)
{
    colour_t zero_colour = { .x = 0, .y = 0, .z = 0 };
    if(depth <= 0)
        return zero_colour;

    hit_record_t rec;
    if(hittable_list_hit(world, r, 0.001f, FLT_MAX, &rec)) {
        point_t target = VEC_ADD_V(rec.p, vec_random_in_hemisphere(rec.normal));
        ray_t s = {
            .orig = rec.p,
            .dir = VEC_SUB_V(target, rec.p)
        };
        return VEC_MUL_F(ray_colour(s, world, depth - 1), 0.5f);
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
    const size_t samples_per_pixel = 100;
    const int32_t max_depth = 50;

    // World
    hittable_list_t world;

    sphere_t sphere_0 = { .centre = {0.0f, 0.0f, -1.0f }, .radius = 0.5f };
    hittable_t hittable_sphere_0 = { .object = &sphere_0, .hit_func = sphere_hit };

    sphere_t sphere_1 = { .centre = {0.0f, -100.5f, -1.0f }, .radius = 100.0f };
    hittable_t hittable_sphere_1 = { .object = &sphere_1, .hit_func = sphere_hit };

    hittable_list_add(&world, hittable_sphere_0);
    hittable_list_add(&world, hittable_sphere_1);

    // Camera
    camera_t cam;
    camera_init(&cam);

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
            colour_t pixel_colour = { .x = 0, .y = 0, .z = 0 };
            for(size_t s = 0; s < samples_per_pixel; ++s) {
                const float u = (j + random_f()) / (image_width - 1);
                const float v = (i + random_f()) / (image_height - 1);
                ray_t r = camera_get_ray(&cam, u, v);
                pixel_colour = VEC_ADD_V(pixel_colour, ray_colour(r, &world, max_depth));
            }
            write_colour(image_file, pixel_colour, samples_per_pixel);
        }
    }
    printf("100%%\n");

    fclose(image_file);

    return EXIT_SUCCESS;
}
