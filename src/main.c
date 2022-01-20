#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "camera.h"
#include "dielectric.h"
#include "hittable.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "material.h"
#include "metal.h"
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
        ray_t scattered = { .orig = {0,0,0}, .dir = {0,0,0} };
        colour_t attenuation = { .x = 0, .y = 0, .z = 0 };

        if(rec.material->scatter_func(rec.material->object, r, rec, &attenuation, &scattered)) {
            return VEC_MUL_V(attenuation, ray_colour(scattered, world, depth - 1));
        }

        colour_t zero_colour = { .x = 0, .y = 0, .z = 0 };
        return zero_colour;
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

    // Materials
    lambertian_t lambertian_ground = { .albedo = { 0.8f, 0.8f, 0.0f } };
    lambertian_t lambertian_centre = { .albedo = { 0.1f, 0.2f, 0.5f } };
    // metal_t metal_left = { .albedo = { 0.8f, 0.8f, 0.8f }, .fuzz = 0.3f };
    // dielectric_t dielectric_centre = { .ir = 1.5f };
    dielectric_t dielectric_left = { .ir = 1.5f };
    metal_t metal_right = { .albedo = { 0.8f, 0.6f, 0.2f }, .fuzz = 0.0f };

    material_t material_ground = { .object = &lambertian_ground, .scatter_func=lambertian_scatter };
    material_t material_centre = { .object = &lambertian_centre, .scatter_func=lambertian_scatter };
    // material_t material_left = { .object = &metal_left, .scatter_func=metal_scatter };
    // material_t material_centre = { .object = &dielectric_centre, .scatter_func=dielectric_scatter };
    material_t material_left = { .object = &dielectric_left, .scatter_func=dielectric_scatter };
    material_t material_right = { .object = &metal_right, .scatter_func=metal_scatter };

    // Hittable Objects
    sphere_t sphere_0 = { .centre = {0.0f, -100.5f, -1.0f }, .radius = 100.0f, .material = &material_ground };
    hittable_t hittable_sphere_0 = { .object = &sphere_0, .hit_func = sphere_hit };

    sphere_t sphere_1 = { .centre = {0.0f, 0.0f, -1.0f }, .radius = 0.5f, .material= &material_centre };
    hittable_t hittable_sphere_1 = { .object = &sphere_1, .hit_func = sphere_hit };

    sphere_t sphere_2 = { .centre = {-1.0f, 0.0f, -1.0f }, .radius = 0.5f, .material= &material_left };
    hittable_t hittable_sphere_2 = { .object = &sphere_2, .hit_func = sphere_hit };

    sphere_t sphere_2_inner = { .centre = {-1.0f, 0.0f, -1.0f }, .radius = -0.4f, .material= &material_left };
    hittable_t hittable_sphere_2_inner = { .object = &sphere_2_inner, .hit_func = sphere_hit };

    sphere_t sphere_3 = { .centre = {1.0f, 0.0f, -1.0f }, .radius = 0.5f, .material= &material_right };
    hittable_t hittable_sphere_3 = { .object = &sphere_3, .hit_func = sphere_hit };

    // World
    hittable_list_t world;
    hittable_list_add(&world, hittable_sphere_0);
    hittable_list_add(&world, hittable_sphere_1);
    hittable_list_add(&world, hittable_sphere_2);
    hittable_list_add(&world, hittable_sphere_2_inner);
    hittable_list_add(&world, hittable_sphere_3);

    // Camera
    const point_t lookfrom = {3,3,2};
    const point_t lookat = {0,0,-1};
    const vec_t vup = {0,1,0};
    const float vfov = 20.0f;
    const float dist_to_focus = vec_length(VEC_SUB_V(lookfrom, lookat));
    const float aperture = 2.0f;
    camera_t cam;
    camera_init(&cam, lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

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
