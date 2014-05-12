CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Werror -O3 -g -std=c99
LDFLAGS = -lreadline
OBJS = $(addsuffix .o, $(basename $(wildcard src/*.c)))
PROG_NAMES = brainfuck
PROGS = $(addprefix bin/, $(PROG_NAMES))

all: $(OBJS) $(PROG_NAMES)

%: src/%.o
	$(CC) $(LDFLAGS) -o bin/$@ src/$@*.o

clean:
	$(RM) $(OBJS) $(PROGS)