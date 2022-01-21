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

#define MAX_NUM_RANDOM_SPHERES 23 * 23

colour_t ray_colour(ray_t r, hittable_list_t* world, int32_t depth)
{
    colour_t zero_colour = { .x = 0, .y = 0, .z = 0 };
    if(depth <= 0)
        return zero_colour;

    hit_record_t rec;
    if(hittable_list_hit(world, r, 0.001f, FLT_MAX, &rec)) {
        ray_t scattered = { .orig = {0,0,0}, .dir = {0,0,0} };
        colour_t attenuation = { .x = 0, .y = 0, .z = 0 };

        if(material_scatter(rec.material, r, rec, &attenuation, &scattered)) {
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
    const float aspect_ratio = 3.0f / 2.0f;
    const int32_t image_width = 1200;
    const int32_t image_height = (int32_t)(image_width / aspect_ratio);
    const size_t samples_per_pixel = 500;
    const int32_t max_depth = 50;

    // World
    hittable_list_t world;
    hittable_list_init(&world);

    lambertian_t lambertian_ground;
    colour_t ground_albedo = { 0.5f, 0.5f, 0.5f };
    lambertian_init(&lambertian_ground, ground_albedo);
    sphere_t sphere_ground;
    point_t ground_centre = { 0.0f, -1000.0f, 0.0f };
    sphere_init(&sphere_ground, ground_centre, 1000.0f, (material_t*)&lambertian_ground);
    hittable_list_add(&world, (hittable_t*)&sphere_ground);

    dielectric_t dielectric_obj_1;
    dielectric_init(&dielectric_obj_1, 1.5f);
    sphere_t sphere_obj_1;
    point_t obj_1_centre = { 0.0f, 1.0f, 0.0f };
    sphere_init(&sphere_obj_1, obj_1_centre, 1.0f, (material_t*)&dielectric_obj_1);
    hittable_list_add(&world, (hittable_t*)&sphere_obj_1);

    lambertian_t lambertian_obj_2;
    colour_t obj_2_albedo = { 0.4f, 0.2f, 0.1f };
    lambertian_init(&lambertian_obj_2, obj_2_albedo);
    sphere_t sphere_obj_2;
    point_t obj_2_centre = { -4.0f, 1.0f, 0.0f };
    sphere_init(&sphere_obj_2, obj_2_centre, 1.0f, (material_t*)&lambertian_obj_2);
    hittable_list_add(&world, (hittable_t*)&sphere_obj_2);

    metal_t metal_obj_3;
    colour_t obj_3_albedo = { 0.7f, 0.6f, 0.5f };
    metal_init(&metal_obj_3, obj_3_albedo, 0.0f);
    sphere_t sphere_obj_3;
    point_t obj_3_centre = { 4.0f, 1.0f, 0.0f };
    sphere_init(&sphere_obj_3, obj_3_centre, 1.0f, (material_t*)&metal_obj_3);
    hittable_list_add(&world, (hittable_t*)&sphere_obj_3);

    lambertian_t random_lambertians[MAX_NUM_RANDOM_SPHERES];
    size_t n_random_lambertians = 0;
    metal_t random_metals[MAX_NUM_RANDOM_SPHERES];
    size_t n_random_metals = 0;
    dielectric_t random_dielectrics[MAX_NUM_RANDOM_SPHERES];
    size_t n_random_dielectrics = 0;
    sphere_t random_spheres[MAX_NUM_RANDOM_SPHERES];
    size_t n_random_spheres = 0;

    const int32_t random_sphere_idx_max = ((int32_t)sqrtf(MAX_NUM_RANDOM_SPHERES) - 1) / 2;
    const int32_t random_sphere_idx_min = -random_sphere_idx_max;
    for(int32_t a = random_sphere_idx_min; a < random_sphere_idx_max; a++) {
        for(int32_t b = random_sphere_idx_min; b < random_sphere_idx_max; b++) {
            const float choose_mat = random_f();
            const point_t centre = { .x = a + 0.9f * random_f(), .y = 0.2f, .z = b + 0.9f * random_f() };
            const point_t p = { .x = 4.0f, .y = 0.2f, .z = 0.0f };

            if(vec_length(VEC_SUB_V(centre, p)) > 0.9f) {
                if (choose_mat < 0.8) {
                    // diffuse
                    lambertian_init(&random_lambertians[n_random_lambertians], VEC_MUL_V(vec_random(), vec_random()));
                    sphere_init(&random_spheres[n_random_spheres], centre, 0.2f, (material_t*)&random_lambertians[n_random_lambertians]);
                    hittable_list_add(&world, (hittable_t*)&random_spheres[n_random_spheres]);
                    ++n_random_lambertians;
                    ++n_random_spheres;
                }
                else if(choose_mat < 0.95f) {
                    // metal
                    metal_init(&random_metals[n_random_metals], vec_random_mm(0.5f, 1.0f), random_f_mm(0.0f, 0.5f));
                    sphere_init(&random_spheres[n_random_spheres], centre, 0.2f, (material_t*)&random_metals[n_random_metals]);
                    hittable_list_add(&world, (hittable_t*)&random_spheres[n_random_spheres]);
                    ++n_random_metals;
                    ++n_random_spheres;
                }
                else {
                    // glass
                    dielectric_init(&random_dielectrics[n_random_dielectrics], 1.5f);
                    sphere_init(&random_spheres[n_random_spheres], centre, 0.2f, (material_t*)&random_dielectrics[n_random_dielectrics]);
                    hittable_list_add(&world, (hittable_t*)&random_spheres[n_random_spheres]);
                    ++n_random_dielectrics;
                    ++n_random_spheres;
                }
            }
        }
    }

    // Camera
    const point_t lookfrom = {13,2,3};
    const point_t lookat = {0,0,0};
    const vec_t vup = {0,1,0};
    const float vfov = 20.0f;
    const float dist_to_focus = 10.0f;
    const float aperture = 0.1f;
    camera_t cam;
    camera_init(&cam, lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

    // Render
    FILE* image_file = fopen("image.ppm", "w");
    char buf[128] = {0};
    const size_t buf_len = snprintf(buf, sizeof(buf), "P3\n%u %u\n255\n", image_width, image_height);
    fwrite(buf, buf_len, 1, image_file);
    memset(buf, 0, sizeof(buf));

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
