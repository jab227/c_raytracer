CFLAGS=-Wall -Wextra -Wconversion -Wpedantic -Werror -std=c11 -ggdb3 -O2
LDLIBS=-lm

objects = main.o vec3.o color.o ray.o

all: main

main: $(objects)

main.o: main.c color.h

vec3.o: vec3.c vec3.h

color.o: color.c color.h

ray.o: ray.c ray.h

.PHONY: clean 

clean:
	rm -rfv $(objects)
