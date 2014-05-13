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

void resize_stack(size_t new_stack_size) {
  if (new_stack_size == 0) {
    fprintf(stderr, "Error: Unable to set stack size to 0\n");
    return;
  }

  debug_print("Stack size: %d -> %d\n", stack_size, new_stack_size);
  stack_size = new_stack_size;
  if ((stack = realloc(stack, stack_size)) == NULL) {
    fprintf(stderr, "Virtual memory exceeded!\n");
    abort();
  }
  memset(stack, 0, stack_size);
}

void brainfuck(char *line) {
  char *c = line;
  char *tmpline = NULL;
  if (stack == NULL || c == NULL) {
    return;
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
          return;
        }

        if ((tmpline = malloc(sizeof *tmpline * (strlen(c) +1))) == NULL) {
          fprintf(stderr, "Virtual memory exceeded!\n");
          abort();
        }
        strcpy(tmpline, c);
        brainfuck(tmpline);
        free(tmpline);
        tmpline = NULL;
        for (; *c != ']' && *c != '\0'; ++c);
        for (; *c == ']'; ++c);
        /*
        while (*c == ']') {
          c = strchr(c, ']');
        }
        */
        break;

      case ']':
        if (stack[stack_ptr] != 0) {
          c = line;
        } else {
          return;
        } break;
    }
  }
}
