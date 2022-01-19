#include "vec.h"

#include <math.h>
#include <stdarg.h>
#include <stdint.h>

#include "util.h"

#define NEAR_ZERO_APPROX 1e-8f

vec_t vec_add_v(size_t n, ...)
{
    va_list ptr;
    va_start(ptr, n);

    vec_t v = va_arg(ptr, vec_t);

    for(size_t i = 1; i < n; ++i) {
        vec_t u = va_arg(ptr, vec_t);
        v.x += u.x;
        v.y += u.y;
        v.z += u.z;
    }
    va_end(ptr);

    return v;
}

vec_t vec_sub_v(size_t n, ...)
{
    va_list ptr;
    va_start(ptr, n);

    vec_t v = va_arg(ptr, vec_t);

    for(size_t i = 1; i < n; ++i) {
        vec_t u = va_arg(ptr, vec_t);
        v.x -= u.x;
        v.y -= u.y;
        v.z -= u.z;
    }
    va_end(ptr);

    return v;
}

vec_t vec_mul_v(size_t n, ...)
{
    va_list ptr;
    va_start(ptr, n);

    vec_t v = va_arg(ptr, vec_t);

    for(size_t i = 1; i < n; ++i) {
        vec_t u = va_arg(ptr, vec_t);
        v.x *= u.x;
        v.y *= u.y;
        v.z *= u.z;
    }
    va_end(ptr);

    return v;
}

vec_t vec_div_v(size_t n, ...)
{
    va_list ptr;
    va_start(ptr, n);

    vec_t v = va_arg(ptr, vec_t);

    for(size_t i = 1; i < n; ++i) {
        vec_t u = va_arg(ptr, vec_t);
        v.x /= u.x;
        v.y /= u.y;
        v.z /= u.z;
    }
    va_end(ptr);

    return v;
}

vec_t vec_add_f(vec_t u, size_t n, ...)
{
    vec_t v = u;

    va_list ptr;
    va_start(ptr, n);
    for(size_t i = 0; i < n; ++i) {
        float f = (float)va_arg(ptr, double);
        v.x += f;
        v.y += f;
        v.z += f;
    }
    va_end(ptr);

    return v;
}

vec_t vec_sub_f(vec_t u, size_t n, ...)
{
    vec_t v = u;

    va_list ptr;
    va_start(ptr, n);
    for(size_t i = 0; i < n; ++i) {
        float f = (float)va_arg(ptr, double);
        v.x -= f;
        v.y -= f;
        v.z -= f;
    }
    va_end(ptr);

    return v;
}

vec_t vec_mul_f(vec_t u, size_t n, ...)
{
    vec_t v = u;

    va_list ptr;
    va_start(ptr, n);
    for(size_t i = 0; i < n; ++i) {
        float f = (float)va_arg(ptr, double);
        v.x *= f;
        v.y *= f;
        v.z *= f;
    }
    va_end(ptr);

    return v;
}

vec_t vec_div_f(vec_t u, size_t n, ...)
{
    vec_t v = u;

    va_list ptr;
    va_start(ptr, n);
    for(size_t i = 0; i < n; ++i) {
        float f = (float)va_arg(ptr, double);
        v.x /= f;
        v.y /= f;
        v.z /= f;
    }
    va_end(ptr);

    return v;
}

float vec_length(vec_t v)
{
    return sqrt(vec_length_squared(v));
}

float vec_length_squared(vec_t v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

vec_t vec_unit(vec_t v)
{
    return VEC_DIV_F(v, vec_length(v));
}

float vec_dot(vec_t u, vec_t v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

vec_t vec_cross(vec_t u, vec_t v)
{
    vec_t r =  {
        .x = u.y * v.z - u.z * v.y,
        .y = u.z * v.x - u.x * v.z,
        .z = u.x * v.y - u.y * v.x
    };
    return r;
}

vec_t vec_random_mm(double min, double max)
{
    vec_t v = {
        .x = random_f_mm(min, max),
        .y = random_f_mm(min, max),
        .z = random_f_mm(min, max),
    };
    return v;
}

vec_t vec_random()
{
    return vec_random_mm(0.0f, 1.0f);
}

vec_t vec_random_in_unit_sphere()
{
    for(;;) {
        const vec_t v = vec_random_mm(-1.0f, 1.0f);
        if(vec_length_squared(v) < 1) {
            return v;
        }
    }
}

vec_t vec_random_unit()
{
    return vec_unit(vec_random_mm(-1.0f, 1.0f)); // TODO: Check this
}

vec_t vec_random_in_hemisphere(vec_t normal)
{
    vec_t v = vec_random_in_unit_sphere();
    if(vec_dot(v, normal) > 0.0f) {
        return v;
    }
    return VEC_MUL_F(v, -1.0f);
}

vec_t vec_random_in_unit_disk()
{
    for(;;) {
        const vec_t v = {
            .x = random_f_mm(-1.0f, 1.0f),
            .y = random_f_mm(-1.0f, 1.0f),
            .z = 0
        };
        if(vec_length_squared(v) < 1) {
            return v;
        }
    }
}

bool vec_near_zero(vec_t v)
{
    return fabsf(v.x) < NEAR_ZERO_APPROX && fabsf(v.y) < NEAR_ZERO_APPROX && fabsf(v.z) < NEAR_ZERO_APPROX;
}

vec_t vec_reflect(vec_t v, vec_t n)
{
    vec_t a = VEC_MUL_F(n, 2, vec_dot(v, n));
    return VEC_SUB_V(v, a);
}

vec_t vec_refract(vec_t uv, vec_t n, float etai_over_etat)
{
    const float dot_prod = vec_dot(VEC_MUL_F(uv, -1.0f), n);
    const float cos_theta = dot_prod < 1.0f ? dot_prod : 1.0f;

    vec_t a = VEC_MUL_F(n, cos_theta);
    vec_t b = VEC_ADD_V(uv, a);

    const vec_t r_out_perp = VEC_MUL_F(b, etai_over_etat);
    const vec_t r_out_parallel = VEC_MUL_F(n, -sqrt(fabsf(1.0f - vec_length_squared(r_out_perp))));

    return VEC_ADD_V(r_out_perp, r_out_parallel);
}

void write_colour(FILE* image_file, colour_t pixel_colour, size_t samples_per_pixel)
{
    float r = pixel_colour.x;
    float g = pixel_colour.y;
    float b = pixel_colour.z;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    const float scale = 1.0f / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    const uint32_t ur = (uint32_t)(256 * clamp(r, 0.0f, 0.999f));
    const uint32_t ug = (uint32_t)(256 * clamp(g, 0.0f, 0.999f));
    const uint32_t ub = (uint32_t)(256 * clamp(b, 0.0f, 0.999f));

    // Write the translated [0,255] value of each color component.
    char buf[128] = {0};
    const size_t buf_len = snprintf(buf, sizeof(buf), "%u %u %u\n", ur, ug, ub);
    fwrite(buf, buf_len, 1, image_file);
}
