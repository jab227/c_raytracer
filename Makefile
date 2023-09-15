CFLAGS=-Wall -Wextra -Wconversion -Wpedantic -Werror -std=c11 -ggdb3 -O2
LDLIBS=-lm

objects = src/main.o src/vec3.o src/color.o src/ray.o src/camera.o src/spheres.o src/interval.o

all: src/main

src/main: $(objects)

src/main.o: src/main.c src/camera.h

src/ray.o: src/ray.c src/ray.h src/vec3.h

src/camera.o: src/camera.c src/camera.h src/vec3.h src/ray.h src/color.h src/spheres.h src/interval.h

src/vec3.o: src/vec3.c src/vec3.h

src/color.o: src/color.c src/color.h src/interval.h

src/spheres.o: src/spheres.c src/spheres.h src/ray.h src/vec3.h src/interval.h

src/interval.o: src/interval.c src/interval.h

.PHONY: clean 

clean:
	rm -rfv $(objects)
