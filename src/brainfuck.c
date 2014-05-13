#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "brainfuck.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#define debug_print(fmt, ...) \
          do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

static char *stack = NULL;
static size_t stack_size = 0;
static size_t stack_ptr = 0;

int resize_brainfuck_stack(size_t new_stack_size) {
  char *new_stack;
  if (new_stack_size == 0) {
    fprintf(stderr, "Error: Unable to set stack size to 0\n");
    return 1;
  }

  debug_print("Stack size: %d -> %d\n", stack_size, new_stack_size);
  stack_size = new_stack_size;
  if ((new_stack = realloc(stack, stack_size)) == NULL) {
    fprintf(stderr, "Error: Virtual memory exceeded!\n");
    return 1;
  }
  stack = new_stack;
  memset(stack, 0, stack_size);
  return 0;
}

void free_brainfuck_stack() {
  free(stack);
  stack = NULL;
}

static int _brainfuck(char *line) {
  char *c = line;
  char *tmpline = NULL;
  if (c == NULL) {
    return 0;
  }

  while (*c) {
    switch (*c++) {
      case '.': putchar(stack[stack_ptr]); break;
      case ',': stack[stack_ptr] = getchar(); break;
      case '+': ++stack[stack_ptr]; break;
      case '-': --stack[stack_ptr]; break;
      case '>': if (stack_ptr +1 < stack_size) { ++stack_ptr; } break;
      case '<': if (stack_ptr > 0)             { --stack_ptr; } break;
      case '?': printf("@%d[%d]\n", stack_ptr, stack[stack_ptr]); break;

      case '[':
        /* Make sure there's a closing bracket before executing */
        if (strchr(c, ']') == NULL) {
          fprintf(stderr, "Warning: No closing bracket found. "
                          "The following code will not be executed:\n%s\n",
                          &c[-1]);
          return 1;
        }

        if ((tmpline = malloc(sizeof *tmpline * (strlen(c) +1))) == NULL) {
          fprintf(stderr, "Virtual memory exceeded!\n");
          abort();
        }
        strcpy(tmpline, c);
        _brainfuck(tmpline);
        free(tmpline);
        tmpline = NULL;
        for (; *c != ']' && *c != '\0'; ++c);
        for (; *c == ']'; ++c);
        break;

      case ']':
        if (stack[stack_ptr] != 0) {
          c = line;
        } else {
          return 0;
        } break;
    }
  }
  return 0;
}

int brainfuck(char *line) {
  if (stack == NULL) {
    fprintf(stderr, "Error: There is no stack!\n");
  } else if (line == NULL) {
    fprintf(stderr, "Error: NULL data received!\n");

  } else {
    long num_brackets = 0;
    for (char *c = line; *c != '\0'; ++c) {
      if (*c == '[') {
        ++num_brackets;
      } else if (*c == ']') {
        --num_brackets;
      }
    }
    if (num_brackets != 0) {
      fprintf(stderr, "Error: Unmatched bracket(s)\n");
    } else {
      return _brainfuck(line);
    }
  }
  return 1;
}
