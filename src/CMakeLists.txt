file(GLOB SOURCES *.c)

target_sources(ray-tracer-c
PRIVATE
    ${SOURCES}
)

# target_sources(ray-tracer-c
# PRIVATE
#     src/main.c
#     src/util.c
#     src/vec.c
#     src/ray.c
#     src/hit_record.c
#     src/material.c
# )
