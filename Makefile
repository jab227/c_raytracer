CFLAGS=-Wall -Wextra -Wconversion -Wpedantic -Werror -std=c11 -ggdb3 -O2
LDLIBS=-lm

objects = main.o vec3.o color.o ray.o camera.o spheres.o

all: main

main: $(objects)

main.o: main.c color.o spheres.o camera.o vec3.o ray.o

ray.o: ray.c ray.h vec3.o

camera.o: camera.c camera.h vec3.o

vec3.o: vec3.c vec3.h

color.o: color.c color.h

spheres.o: spheres.c spheres.h ray.o vec3.o

.PHONY: clean 

clean:
	rm -rfv $(objects)
