cmake_minimum_required(VERSION 3.19)

if (NOT DEFINED CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 11)
endif()

if (NOT DEFINED CMAKE_C_EXTENSIONS)
  set(CMAKE_C_EXTENSIONS OFF)
endif()

if (NOT DEFINED CMAKE_EXPORT_COMPILE_COMMANDS) 
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

project(raytracer VERSION 0.1 LANGUAGES C)

add_compile_options(-Wall -Werror -Wconversion -Wextra)

add_library(vec3 STATIC src/vec3.c src/vec3.h)
target_link_libraries(vec3 PRIVATE m)

add_library(interval STATIC src/interval.c src/interval.h)

add_library(color STATIC src/color.c src/color.h)
target_link_libraries(color PRIVATE interval m)

add_library(ray STATIC src/ray.c src/ray.h)
target_link_libraries(ray PRIVATE vec3)

add_library(spheres STATIC src/spheres.c src/spheres.h)
target_link_libraries(spheres PRIVATE vec3 ray interval m)

add_library(camera STATIC src/camera.c src/camera.h)
target_link_libraries(camera PRIVATE vec3 ray interval spheres color)

add_executable(main src/main.c)
target_link_libraries(main PUBLIC camera)
