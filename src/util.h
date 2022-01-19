#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stddef.h>

#include "vec.h"

void write_colour(FILE* image_file, colour_t pixel_color);

float random_f_mm(float min, float max);
float random_f();

float degrees_to_radians(float degrees);

float clamp(float x, float min, float max);

float reflectance(float cosine, float ref_idx);

#endif //__UTIL_H__
