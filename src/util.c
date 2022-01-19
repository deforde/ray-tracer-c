#include "util.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

void write_colour(FILE* image_file, colour_t pixel_color)
{
    uint32_t ir = (uint32_t)(255.999 * pixel_color.x);
    uint32_t ig = (uint32_t)(255.999 * pixel_color.y);
    uint32_t ib = (uint32_t)(255.999 * pixel_color.z);

    char buf[128] = {0};
    const size_t buf_len = snprintf(buf, sizeof(buf), "%u %u %u\n", ir, ig, ib);
    fwrite(buf, buf_len, 1, image_file);
}

float random_f_mm(float min, float max)
{
    const float f = (float)rand() / RAND_MAX;
    return (max - min) * f + min;
}

float random_f()
{
    return random_f_mm(0.0f, 1.0f);
}

float degrees_to_radians(float degrees)
{
    return degrees * M_PI / 180.0f;
}

float clamp(float x, float min, float max)
{
    if(x < min)
        x = min;
    else if(x > max)
        x = max;
    return x;
}

float reflectance(float cosine, float ref_idx)
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * powf((1 - cosine), 5);
}
