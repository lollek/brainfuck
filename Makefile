CC = cc
CFLAGS = -Wall -Wextra -pedantic -Werror -O3 -g -ansi -std=c99
LDFLAGS = -lreadline
OBJS = $(addsuffix .o, $(basename $(wildcard src/*.c)))

bin/brainfuck: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(OBJS) brainfuck
