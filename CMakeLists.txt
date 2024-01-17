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
add_subdirectory(src)
