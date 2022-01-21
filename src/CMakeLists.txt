file(GLOB SOURCES *.c)

target_sources(ray-tracer-c
PRIVATE
    ${SOURCES}
)
