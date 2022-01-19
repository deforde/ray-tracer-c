#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "util.h"
#include "vec.h"

int main()
{
    // Image
    const int32_t image_width = 256;
    const int32_t image_height = 256;

    FILE* image_file = fopen("image.ppm", "w");
    char buf[128] = {0};
    const size_t buf_len = snprintf(buf, sizeof(buf), "P3\n%u %u\n255\n", image_width, image_height);
    fwrite(buf, buf_len, 1, image_file);
    memset(buf, 0, sizeof(buf));

    // Render
    printf("Rendering...\n");
    for(int32_t i = (image_height - 1); i >= 0; --i) {
        printf("%3u%%\r", (uint32_t)(100.0f * (1.0f - (float)i / image_height)));
        fflush(stdout);
        for(int32_t j = 0; j < image_width; ++j) {
            const float r = (float)j / (image_width - 1);
            const float g = (float)i / (image_height - 1);
            const float b = 0.25f;

            uint32_t ir = (uint32_t)(255.999 * r);
            uint32_t ig = (uint32_t)(255.999 * g);
            uint32_t ib = (uint32_t)(255.999 * b);

            const size_t buf_len = snprintf(buf, sizeof(buf), "%u %u %u\n", ir, ig, ib);
            fwrite(buf, buf_len, 1, image_file);
            memset(buf, 0, sizeof(buf));
        }
    }
    printf("100%%\n");

    fclose(image_file);

    return EXIT_SUCCESS;
}
