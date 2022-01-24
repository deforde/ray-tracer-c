cmake_minimum_required(VERSION 3.0.0)
project(ray-tracer-c C)

add_compile_options(-Wall -Wextra -pedantic -Werror -Werror=vla -fopenmp)

add_executable(ray-tracer-c "")

add_subdirectory(src)
target_link_libraries(ray-tracer-c m gomp)
