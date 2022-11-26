#ifndef VEC_H
#define VEC_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    float x;
    float y;
    float z;
} vec_t;


vec_t vec_add_v(const vec_t *r, size_t n, const vec_t* x[]);
vec_t vec_sub_v(const vec_t *r, size_t n, const vec_t* x[]);
vec_t vec_mul_v(const vec_t *r, size_t n, const vec_t* x[]);
vec_t vec_div_v(const vec_t *r, size_t n, const vec_t* x[]);

vec_t vec_add_f(const vec_t *u, float f);
vec_t vec_sub_f(const vec_t *u, float f);
vec_t vec_mul_f(const vec_t *u, float f);
vec_t vec_div_f(const vec_t *u, float f);

float vec_length(const vec_t *v);
float vec_length_squared(const vec_t *v);

vec_t vec_unit(const vec_t *v);

float vec_dot(const vec_t *u, const vec_t *v);
vec_t vec_cross(const vec_t *u, const vec_t *v);

vec_t vec_random_mm(double min, double max);
vec_t vec_random();
vec_t vec_random_in_unit_sphere();
vec_t vec_random_unit();
vec_t vec_random_in_hemisphere(const vec_t *normal);
vec_t vec_random_in_unit_disk();

bool vec_near_zero(const vec_t *v);

vec_t vec_reflect(const vec_t *v, const vec_t *n);
vec_t vec_refract(const vec_t *uv, const vec_t *n, float etai_over_etat);

typedef vec_t point_t;
typedef vec_t colour_t;

#endif //VEC_H
