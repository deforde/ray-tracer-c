#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

#include "vec.h"

void write_colour(FILE* image_file, colour_t *pixel_color, size_t samples_per_pixel);

float random_f_mm(float min, float max);
float random_f();

float degrees_to_radians(float degrees);

float clamp(float x, float min, float max);

float reflectance(float cosine, float ref_idx);

#endif //UTIL_H
