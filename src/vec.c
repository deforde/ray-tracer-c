#include "vec.h"

#include <math.h>
#include <stdarg.h>

#include "util.h"

#define NEAR_ZERO_APPROX 1e-8f

vec_t vec_add_v(size_t n, vec_t x[]) {
  vec_t *v = &x[0];

  for (size_t i = 1; i < n; ++i) {
    const vec_t *u = &x[i];
    v->x += u->x;
    v->y += u->y;
    v->z += u->z;
  }

  return *v;
}

vec_t vec_sub_v(size_t n, vec_t x[]) {
  vec_t *v = &x[0];

  for (size_t i = 1; i < n; ++i) {
    const vec_t *u = &x[i];
    v->x -= u->x;
    v->y -= u->y;
    v->z -= u->z;
  }

  return *v;
}

vec_t vec_mul_v(size_t n, vec_t x[]) {
  vec_t *v = &x[0];

  for (size_t i = 1; i < n; ++i) {
    const vec_t *u = &x[i];
    v->x *= u->x;
    v->y *= u->y;
    v->z *= u->z;
  }

  return *v;
}

vec_t vec_div_v(size_t n, vec_t x[]) {
  vec_t *v = &x[0];

  for (size_t i = 1; i < n; ++i) {
    const vec_t *u = &x[i];
    v->x /= u->x;
    v->y /= u->y;
    v->z /= u->z;
  }

  return *v;
}

vec_t vec_add_f(const vec_t *u, float f) {
  vec_t v = *u;

  v.x += f;
  v.y += f;
  v.z += f;

  return v;
}

vec_t vec_sub_f(const vec_t *u, float f) {
  vec_t v = *u;

  v.x -= f;
  v.y -= f;
  v.z -= f;

  return v;
}

vec_t vec_mul_f(const vec_t *u, float f) {
  vec_t v = *u;

  v.x *= f;
  v.y *= f;
  v.z *= f;

  return v;
}

vec_t vec_div_f(const vec_t *u, float f) {
  vec_t v = *u;

  v.x /= f;
  v.y /= f;
  v.z /= f;

  return v;
}

float vec_length(const vec_t *v) { return sqrtf(vec_length_squared(v)); }

float vec_length_squared(const vec_t *v) {
  return v->x * v->x + v->y * v->y + v->z * v->z;
}

vec_t vec_unit(const vec_t *v) { return vec_div_f(v, vec_length(v)); }

float vec_dot(const vec_t *u, const vec_t *v) {
  return u->x * v->x + u->y * v->y + u->z * v->z;
}

vec_t vec_cross(const vec_t *u, const vec_t *v) {
  vec_t r = {.x = u->y * v->z - u->z * v->y,
             .y = u->z * v->x - u->x * v->z,
             .z = u->x * v->y - u->y * v->x};
  return r;
}

vec_t vec_random_mm(double min, double max) {
  vec_t v = {
      .x = random_f_mm(min, max),
      .y = random_f_mm(min, max),
      .z = random_f_mm(min, max),
  };
  return v;
}

vec_t vec_random() { return vec_random_mm(0.0f, 1.0f); }

vec_t vec_random_in_unit_sphere() {
  for (;;) {
    const vec_t v = vec_random_mm(-1.0f, 1.0f);
    if (vec_length_squared(&v) < 1) {
      return v;
    }
  }
}

vec_t vec_random_unit() {
  const vec_t v = vec_random_mm(-1.0f, 1.0f);
  return vec_unit(&v); // TODO: Check this
}

vec_t vec_random_in_hemisphere(const vec_t *normal) {
  const vec_t v = vec_random_in_unit_sphere();
  if (vec_dot(&v, normal) > 0.0f) {
    return v;
  }
  return vec_mul_f(&v, -1.0f);
}

vec_t vec_random_in_unit_disk() {
  for (;;) {
    const vec_t v = {
        .x = random_f_mm(-1.0f, 1.0f), .y = random_f_mm(-1.0f, 1.0f), .z = 0};
    if (vec_length_squared(&v) < 1) {
      return v;
    }
  }
}

bool vec_near_zero(const vec_t *v) {
  return fabsf(v->x) < NEAR_ZERO_APPROX && fabsf(v->y) < NEAR_ZERO_APPROX &&
         fabsf(v->z) < NEAR_ZERO_APPROX;
}

vec_t vec_reflect(const vec_t *v, const vec_t *n) {
  return VEC_SUB_V(*v, vec_mul_f(n, 2.0f * vec_dot(v, n)));
}

vec_t vec_refract(const vec_t *uv, const vec_t *n, float etai_over_etat) {
  const vec_t x = vec_mul_f(uv, -1.0f);
  const float dot_prod = vec_dot(&x, n);
  const float cos_theta = dot_prod < 1.0f ? dot_prod : 1.0f;

  vec_t b = VEC_ADD_V(*uv, vec_mul_f(n, cos_theta));

  const vec_t r_out_perp = vec_mul_f(&b, etai_over_etat);

  return VEC_ADD_V(
      r_out_perp,
      vec_mul_f(n, -sqrtf(fabsf(1.0f - vec_length_squared(&r_out_perp)))));
}
