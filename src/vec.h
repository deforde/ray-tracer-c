#ifndef VEC_H
#define VEC_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    float x;
    float y;
    float z;
} vec_t;

vec_t vec_add_v(size_t n, ...);
vec_t vec_sub_v(size_t n, ...);
vec_t vec_mul_v(size_t n, ...);
vec_t vec_div_v(size_t n, ...);

vec_t vec_add_f(const vec_t *u, size_t n, ...);
vec_t vec_sub_f(const vec_t *u, size_t n, ...);
vec_t vec_mul_f(const vec_t *u, size_t n, ...);
vec_t vec_div_f(const vec_t *u, size_t n, ...);

#define PP_NARG(...) \
         PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
         PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

#define VEC_FUNC_V(FUNC_NAME, ...) FUNC_NAME(PP_NARG(__VA_ARGS__), __VA_ARGS__)

#define VEC_ADD_V(...) VEC_FUNC_V(vec_add_v, __VA_ARGS__)
#define VEC_SUB_V(...) VEC_FUNC_V(vec_sub_v, __VA_ARGS__)
#define VEC_MUL_V(...) VEC_FUNC_V(vec_mul_v, __VA_ARGS__)
#define VEC_DIV_V(...) VEC_FUNC_V(vec_div_v, __VA_ARGS__)

#define VEC_FUNC_F(FUNC_NAME, VEC, ...) FUNC_NAME(VEC, PP_NARG(__VA_ARGS__), __VA_ARGS__)

#define VEC_ADD_F(VEC, ...) VEC_FUNC_F(vec_add_f, VEC, __VA_ARGS__)
#define VEC_SUB_F(VEC, ...) VEC_FUNC_F(vec_sub_f, VEC, __VA_ARGS__)
#define VEC_MUL_F(VEC, ...) VEC_FUNC_F(vec_mul_f, VEC, __VA_ARGS__)
#define VEC_DIV_F(VEC, ...) VEC_FUNC_F(vec_div_f, VEC, __VA_ARGS__)

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
