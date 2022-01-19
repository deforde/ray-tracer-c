#ifndef __VEC_H___
#define __VEC_H___

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    float x;
    float y;
    float z;
} vec_t;

vec_t vec_add_v(size_t n, ...);
vec_t vec_sub_v(size_t n, ...);
vec_t vec_mul_v(size_t n, ...);
vec_t vec_div_v(size_t n, ...);

vec_t vec_add_f(vec_t u, size_t n, ...);
vec_t vec_sub_f(vec_t u, size_t n, ...);
vec_t vec_mul_f(vec_t u, size_t n, ...);
vec_t vec_div_f(vec_t u, size_t n, ...);

#define NUM_ARGS(ARG_TYPE, ...) (sizeof((ARG_TYPE[]){__VA_ARGS__})/sizeof(ARG_TYPE))

#define VEC_FUNC_V(FUNC_NAME, ...) FUNC_NAME(NUM_ARGS(vec_t, __VA_ARGS__), __VA_ARGS__)

#define VEC_ADD_V(...) VEC_FUNC_V(vec_add_v, __VA_ARGS__)
#define VEC_SUB_V(...) VEC_FUNC_V(vec_sub_v, __VA_ARGS__)
#define VEC_MUL_V(...) VEC_FUNC_V(vec_mul_v, __VA_ARGS__)
#define VEC_DIV_V(...) VEC_FUNC_V(vec_div_v, __VA_ARGS__)

#define VEC_FUNC_F(FUNC_NAME, VEC, ...) FUNC_NAME(VEC, NUM_ARGS(float, __VA_ARGS__), __VA_ARGS__)

#define VEC_ADD_F(VEC, ...) VEC_FUNC_F(vec_add_f, VEC, __VA_ARGS__)
#define VEC_SUB_F(VEC, ...) VEC_FUNC_F(vec_sub_f, VEC, __VA_ARGS__)
#define VEC_MUL_F(VEC, ...) VEC_FUNC_F(vec_mul_f, VEC, __VA_ARGS__)
#define VEC_DIV_F(VEC, ...) VEC_FUNC_F(vec_div_f, VEC, __VA_ARGS__)

float vec_length(vec_t v);
float vec_length_squared(vec_t v);

vec_t vec_unit(vec_t v);

float vec_dot(vec_t u, vec_t v);
vec_t vec_cross(vec_t u, vec_t v);

vec_t vec_random_mm(double min, double max);
vec_t vec_random();
vec_t vec_random_in_unit_sphere();
vec_t vec_random_unit();
vec_t vec_random_in_hemisphere(vec_t normal);
vec_t vec_random_in_unit_disk();

bool vec_near_zero(vec_t v);

vec_t vec_reflect(vec_t v, vec_t n);
vec_t vec_refract(vec_t uv, vec_t n, float etai_over_etat);

typedef vec_t point_t;
typedef vec_t colour_t;

void write_colour(FILE* image_file, colour_t pixel_colour, size_t samples_per_pixel);

#endif //__VEC_H___
