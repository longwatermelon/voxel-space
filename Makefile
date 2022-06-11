CC=gcc
CFLAGS=-std=gnu17 -ggdb -Wall -Werror -Wpedantic
LIBS=-lm -lSDL2 -lpng

SRC=$(wildcard src/*.c)
OBJS=$(addprefix obj/, $(SRC:.c=.o))

all:
	mkdir -p obj/src
	$(MAKE) main

main: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS)

obj/src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean:
	-rm -rf obj a.out

