add_executable(main main.c
  vec3.c vec3.h
  interval.c interval.h
  color.c color.h
  ray.c ray.h
  spheres.c spheres.h
  camera.c camera.h
  prng.c prng.h)
  
target_link_libraries(main PRIVATE m)
target_link_options(main PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address,undefined>")
target_compile_options(main PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address,undefined>")
