add_executable(main main.c
  vec3.c vec3.h
  interval.c interval.h
  color.c color.h
  ray.c ray.h
  spheres.c spheres.h
  camera.c camera.h
  prng.c prng.h)
target_link_libraries(main PRIVATE m)
