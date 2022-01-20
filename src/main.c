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

#define TOTAL_NUM_RANDOM_SPHERES 23 * 23

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
    const float aspect_ratio = 3.0f / 2.0f;
    const int32_t image_width = 1200;
    const int32_t image_height = (int32_t)(image_width / aspect_ratio);
    const size_t samples_per_pixel = 500;
    const int32_t max_depth = 50;

    // World
    hittable_list_t world;
    world.n_objects = 0;

    lambertian_t lambertian_ground = { .albedo = { 0.5f, 0.5f, 0.5f } };
    material_t material_ground = { .object = &lambertian_ground, .scatter_func = lambertian_scatter };
    sphere_t sphere_ground = { .centre = { 0.0f, -1000.0f, 0.0f }, .radius = 1000.0f, .material = &material_ground };
    hittable_t hittable_ground = { .object = &sphere_ground, .hit_func = sphere_hit };
    hittable_list_add(&world, hittable_ground);

    dielectric_t dielectric_obj_1 = { .ir = 1.5f };
    material_t material_obj_1 = { .object = &dielectric_obj_1, .scatter_func = dielectric_scatter };
    sphere_t sphere_obj_1 = { .centre = { 0.0f, 1.0f, 0.0f }, .radius = 1.0f, .material = &material_obj_1 };
    hittable_t hittable_obj_1 = { .object = &sphere_obj_1, .hit_func = sphere_hit };
    hittable_list_add(&world, hittable_obj_1);

    lambertian_t lambertian_obj_2 = { .albedo = { 0.4f, 0.2f, 0.1f } };
    material_t material_obj_2 = { .object = &lambertian_obj_2, .scatter_func = lambertian_scatter };
    sphere_t sphere_obj_2 = { .centre = { -4.0f, 1.0f, 0.0f }, .radius = 1.0f, .material = &material_obj_2 };
    hittable_t hittable_obj_2 = { .object = &sphere_obj_2, .hit_func = sphere_hit };
    hittable_list_add(&world, hittable_obj_2);

    metal_t metal_obj_3 = { .albedo = { 0.7f, 0.6f, 0.5f }, .fuzz = 0.0f };
    material_t material_obj_3 = { .object = &metal_obj_3, .scatter_func = metal_scatter };
    sphere_t sphere_obj_3 = { .centre = { 4.0f, 1.0f, 0.0f }, .radius = 1.0f, .material = &material_obj_3 };
    hittable_t hittable_obj_3 = { .object = &sphere_obj_3, .hit_func = sphere_hit };
    hittable_list_add(&world, hittable_obj_3);

    lambertian_t random_lambertians[TOTAL_NUM_RANDOM_SPHERES];
    size_t n_random_lambertians = 0;
    metal_t random_metals[TOTAL_NUM_RANDOM_SPHERES];
    size_t n_random_metals = 0;
    dielectric_t random_dielectrics[TOTAL_NUM_RANDOM_SPHERES];
    size_t n_random_dielectrics = 0;
    material_t random_materials[TOTAL_NUM_RANDOM_SPHERES];
    size_t n_random_materials = 0;
    sphere_t random_spheres[TOTAL_NUM_RANDOM_SPHERES];
    size_t n_random_spheres = 0;
    hittable_t random_hittables[TOTAL_NUM_RANDOM_SPHERES];
    size_t n_random_hittables = 0;

    const int32_t random_sphere_idx_max = ((int32_t)sqrtf(TOTAL_NUM_RANDOM_SPHERES) - 1) / 2;
    const int32_t random_sphere_idx_min = -random_sphere_idx_max;
    for(int32_t a = random_sphere_idx_min; a < random_sphere_idx_max; a++) {
        for(int32_t b = random_sphere_idx_min; b < random_sphere_idx_max; b++) {
            const float choose_mat = random_f();
            const point_t centre = { .x = a + 0.9f * random_f(), .y = 0.2f, .z = b + 0.9f * random_f() };
            const point_t p = { .x = 4.0f, .y = 0.2f, .z = 0.0f };

            if(vec_length(VEC_SUB_V(centre, p)) > 0.9f) {
                if (choose_mat < 0.8) {
                    // diffuse
                    random_lambertians[n_random_lambertians].albedo = VEC_MUL_V(vec_random(), vec_random());

                    random_materials[n_random_materials].object = &random_lambertians[n_random_lambertians];
                    random_materials[n_random_materials].scatter_func = lambertian_scatter;

                    random_spheres[n_random_spheres].centre = centre;
                    random_spheres[n_random_spheres].radius = 0.2f;
                    random_spheres[n_random_spheres].material = &random_materials[n_random_materials];

                    random_hittables[n_random_hittables].object = &random_spheres[n_random_spheres];
                    random_hittables[n_random_hittables].hit_func = sphere_hit;

                    hittable_list_add(&world, random_hittables[n_random_hittables]);

                    ++n_random_lambertians;
                    ++n_random_materials;
                    ++n_random_spheres;
                    ++n_random_hittables;
                }
                else if(choose_mat < 0.95f) {
                    // metal
                    random_metals[n_random_metals].albedo = vec_random_mm(0.5f, 1.0f);
                    random_metals[n_random_metals].fuzz = random_f_mm(0.0f, 0.5f);

                    random_materials[n_random_materials].object = &random_metals[n_random_metals];
                    random_materials[n_random_materials].scatter_func = metal_scatter;

                    random_spheres[n_random_spheres].centre = centre;
                    random_spheres[n_random_spheres].radius = 0.2f;
                    random_spheres[n_random_spheres].material = &random_materials[n_random_materials];

                    random_hittables[n_random_hittables].object = &random_spheres[n_random_spheres];
                    random_hittables[n_random_hittables].hit_func = sphere_hit;

                    hittable_list_add(&world, random_hittables[n_random_hittables]);

                    ++n_random_metals;
                    ++n_random_materials;
                    ++n_random_spheres;
                    ++n_random_hittables;
                }
                else {
                    // glass
                    random_dielectrics[n_random_dielectrics].ir = 1.5f;

                    random_materials[n_random_materials].object = &random_dielectrics[n_random_dielectrics];
                    random_materials[n_random_materials].scatter_func = dielectric_scatter;

                    random_spheres[n_random_spheres].centre = centre;
                    random_spheres[n_random_spheres].radius = 0.2f;
                    random_spheres[n_random_spheres].material = &random_materials[n_random_materials];

                    random_hittables[n_random_hittables].object = &random_spheres[n_random_spheres];
                    random_hittables[n_random_hittables].hit_func = sphere_hit;

                    hittable_list_add(&world, random_hittables[n_random_hittables]);

                    ++n_random_dielectrics;
                    ++n_random_materials;
                    ++n_random_spheres;
                    ++n_random_hittables;
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
