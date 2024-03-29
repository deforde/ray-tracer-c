#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <omp.h>

#include "camera.h"
#include "dielectric.h"
#include "hit_record.h"
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
#define TOTAL_NUM_OBJECTS MAX_NUM_RANDOM_SPHERES + 4

#define IMAGE_WIDTH 1200
#define IMAGE_HEIGHT (IMAGE_WIDTH * 2 / 3)

static colour_t pixels[IMAGE_WIDTH * IMAGE_HEIGHT] = {0};

static colour_t ray_colour(ray_t *r, hittable_list_t *world, int32_t depth) {
  colour_t zero_colour = {.x = 0, .y = 0, .z = 0};
  if (depth <= 0)
    return zero_colour;

  hit_record_t rec;
  if (hittable_list_hit(world, r, 0.001f, FLT_MAX, &rec)) {
    ray_t scattered = {.orig = {0, 0, 0}, .dir = {0, 0, 0}};
    colour_t attenuation = {.x = 0, .y = 0, .z = 0};

    if (material_scatter(rec.material, r, &rec, &attenuation, &scattered)) {
      return VEC_MUL_V(attenuation, ray_colour(&scattered, world, depth - 1));
    }

    return zero_colour;
  }

  vec_t unit_dir = vec_unit(&r->dir);
  const float t = 0.5f * (unit_dir.y + 1.0f);
  colour_t a = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
  colour_t b = {.x = 0.5f, .y = 0.7f, .z = 1.0f};
  return VEC_ADD_V(vec_mul_f(&a, 1.0f - t), vec_mul_f(&b, t));
}

int main() {
  // Image
  const float aspect_ratio = 3.f / 2.f;
  const size_t image_width = IMAGE_WIDTH;
  const size_t image_height = IMAGE_HEIGHT;
  const size_t samples_per_pixel = 100;
  const int32_t max_depth = 50;

  // World
  hittable_list_t world = {0};
  // hittable_list_init(&world);

  lambertian_t lambertians[TOTAL_NUM_OBJECTS];
  size_t n_lambertians = 0;
  metal_t metals[TOTAL_NUM_OBJECTS];
  size_t n_metals = 0;
  dielectric_t dielectrics[TOTAL_NUM_OBJECTS];
  size_t n_dielectrics = 0;
  sphere_t spheres[TOTAL_NUM_OBJECTS];
  size_t n_spheres = 0;

  colour_t ground_albedo = {0.5f, 0.5f, 0.5f};
  lambertian_init(&lambertians[n_lambertians], ground_albedo);
  point_t ground_centre = {0.0f, -1000.0f, 0.0f};
  sphere_init(&spheres[n_spheres], ground_centre, 1000.0f,
              (material_t *)&lambertians[n_lambertians]);
  ++n_lambertians;
  ++n_spheres;

  dielectric_init(&dielectrics[n_dielectrics], 1.5f);
  point_t obj_1_centre = {0.0f, 1.0f, 0.0f};
  sphere_init(&spheres[n_spheres], obj_1_centre, 1.0f,
              (material_t *)&dielectrics[n_dielectrics]);
  ++n_dielectrics;
  ++n_spheres;

  colour_t obj_2_albedo = {0.4f, 0.2f, 0.1f};
  lambertian_init(&lambertians[n_lambertians], obj_2_albedo);
  point_t obj_2_centre = {-4.0f, 1.0f, 0.0f};
  sphere_init(&spheres[n_spheres], obj_2_centre, 1.0f,
              (material_t *)&lambertians[n_lambertians]);
  ++n_lambertians;
  ++n_spheres;

  colour_t obj_3_albedo = {0.7f, 0.6f, 0.5f};
  metal_init(&metals[n_metals], obj_3_albedo, 0.0f);
  point_t obj_3_centre = {4.0f, 1.0f, 0.0f};
  sphere_init(&spheres[n_spheres], obj_3_centre, 1.0f,
              (material_t *)&metals[n_metals]);
  ++n_metals;
  ++n_spheres;

  const int32_t random_sphere_idx_max =
      ((int32_t)sqrtf(MAX_NUM_RANDOM_SPHERES) - 1) / 2;
  const int32_t random_sphere_idx_min = -random_sphere_idx_max;
  for (int32_t a = random_sphere_idx_min; a < random_sphere_idx_max; a++) {
    for (int32_t b = random_sphere_idx_min; b < random_sphere_idx_max; b++) {
      const float choose_mat = random_f();
      const point_t centre = {
          .x = a + 0.9f * random_f(), .y = 0.2f, .z = b + 0.9f * random_f()};
      const point_t p = {.x = 4.0f, .y = 0.2f, .z = 0.0f};

      const vec_t x = VEC_SUB_V(centre, p);
      if (vec_length(&x) > 0.9f) {
        if (choose_mat < 0.8) {
          // diffuse
          lambertian_init(&lambertians[n_lambertians],
                          VEC_MUL_V(vec_random(), vec_random()));
          sphere_init(&spheres[n_spheres], centre, 0.2f,
                      (material_t *)&lambertians[n_lambertians]);
          ++n_lambertians;
          ++n_spheres;
        } else if (choose_mat < 0.95f) {
          // metal
          metal_init(&metals[n_metals], vec_random_mm(0.5f, 1.0f),
                     random_f_mm(0.0f, 0.5f));
          sphere_init(&spheres[n_spheres], centre, 0.2f,
                      (material_t *)&metals[n_metals]);
          ++n_metals;
          ++n_spheres;
        } else {
          // glass
          dielectric_init(&dielectrics[n_dielectrics], 1.5f);
          sphere_init(&spheres[n_spheres], centre, 0.2f,
                      (material_t *)&dielectrics[n_dielectrics]);
          ++n_dielectrics;
          ++n_spheres;
        }
      }
    }
  }

  for (size_t n = 0; n < n_spheres; n++) {
    hittable_list_add(&world, (hittable_t *)&spheres[n]);
  }

  // Camera
  const point_t lookfrom = {13, 2, 3};
  const point_t lookat = {0, 0, 0};
  const vec_t vup = {0, 1, 0};
  const float vfov = 20.0f;
  const float dist_to_focus = 10.0f;
  const float aperture = 0.1f;
  camera_t cam;
  camera_init(&cam, lookfrom, lookat, vup, vfov, aspect_ratio, aperture,
              dist_to_focus);

  // Render
  printf("Rendering...\n");
#pragma omp parallel for
  for (size_t n = 0; n < (size_t)(image_height * image_width); ++n) {
    const int thread_id = omp_get_thread_num();
    const int num_threads = omp_get_num_threads();
    if (thread_id == num_threads / 2) {
      fprintf(stderr, "%3u%% (thread: %i/%i)\r",
              (uint32_t)(100.0f *
                         (n - thread_id *
                                  (image_height * image_width / num_threads)) /
                         (image_height * image_width / num_threads)),
              thread_id, num_threads);
    }
    size_t i = n / image_width;
    size_t j = n % image_width;
    colour_t pixel_colour = {.x = 0, .y = 0, .z = 0};
    for (size_t s = 0; s < samples_per_pixel; ++s) {
      const float u = (j + random_f()) / (image_width - 1);
      const float v =
          ((image_height - 1 - i) + random_f()) / (image_height - 1);
      ray_t r = camera_get_ray(&cam, u, v);
      pixel_colour = VEC_ADD_V(pixel_colour, ray_colour(&r, &world, max_depth));
    }
    pixels[n] = pixel_colour;
  }
  printf("100%%\n");

  // Write
  FILE *image_file = fopen("image.ppm", "w");
  char buf[128] = {0};
  const size_t buf_len = snprintf(buf, sizeof(buf), "P3\n%zu %zu\n255\n",
                                  image_width, image_height);
  fwrite(buf, buf_len, 1, image_file);
  memset(buf, 0, sizeof(buf));
  for (size_t n = 0; n < (size_t)(image_height * image_width); ++n) {
    colour_t pixel_colour = pixels[n];
    write_colour(image_file, &pixel_colour, samples_per_pixel);
  }
  fclose(image_file);

  return EXIT_SUCCESS;
}
