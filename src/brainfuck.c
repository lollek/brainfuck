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
  if ((new_stack = realloc(stack, new_stack_size)) == NULL) {
    fprintf(stderr, "Error: Virtual memory exceeded!\n");
    return 1;
  }
  stack_size = new_stack_size;
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
  if (c == NULL) {
    return 0;
  }

  while (*c != '\0') {
    size_t sublen;
    long num_brackets;

    switch (*c++) {
      case '.': putchar(stack[stack_ptr]); break;
      case ',': stack[stack_ptr] = getchar(); break;
      case '+': ++stack[stack_ptr]; break;
      case '-': --stack[stack_ptr]; break;
      case '>': if (stack_ptr +1 < stack_size) { ++stack_ptr; } break;
      case '<': if (stack_ptr > 0)             { --stack_ptr; } break;
      case '?': printf("@%d[%d]\n", stack_ptr, stack[stack_ptr]); break;

      case '[':
        /* Find the matching closing bracket */
        sublen = 0;
        num_brackets = 1;
        for (char *sub = c; *sub != '\0'; ++sub, ++sublen) {
          if      (*sub == '[') { ++num_brackets; }
          else if (*sub == ']') { --num_brackets; }
          if (num_brackets == 0) {
            break;
          }
        }

        if (stack[stack_ptr] != 0) {
          /* Change closing bracket to a 0, so we can use it as a subfunction */
          char stop_symbol = c[sublen];
          c[sublen] = '\0';

          /* Execute subfunction until pointer points to 0 */
          while (stack[stack_ptr] != 0) {
            _brainfuck(c);
          }

          /* Restore the closing bracket and continue */
          c[sublen] = stop_symbol;
        }

        /* Afterwards, move to the end of the subfunction */
        c += sublen +1;
        break;

      case ']': return 0; break;
    }
  }
  return 0;
}

int brainfuck(char *line) {
  long num_brackets = 0;

  if (stack == NULL) {
    fprintf(stderr, "Error: There is no stack!\n");
    return 1;
  } else if (line == NULL) {
    fprintf(stderr, "Error: NULL data received!\n");
    return 1;
  }

  for (char *c = line; *c != '\0'; ++c) {
    if      (*c == '[') { ++num_brackets; }
    else if (*c == ']') { --num_brackets; }
    if (num_brackets < 0) {
      fprintf(stderr, "Error: Unmatched bracket(s)\n");
      return 1;
    }
  }
  if (num_brackets != 0) {
    fprintf(stderr, "Error: Unmatched bracket(s)\n");
    return 1;
  } else {
    return _brainfuck(line);
  }
}
