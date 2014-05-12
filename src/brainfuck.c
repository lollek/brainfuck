#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>

#define DEBUG 0

#define debug_print(fmt, ...) \
          do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

static char *stack = NULL;
static size_t stack_size = 0;
static size_t stack_ptr = 0;

void resize_stack(size_t new_stack_size) {
  size_t old_stack_size = stack_size;
  stack_size = new_stack_size;

  debug_print("Changing stack size from %d to %d\n",
              old_stack_size, stack_size);

  if ((stack = realloc(stack, stack_size)) == NULL) {
    fprintf(stderr, "Virtual memory exceeded!\n");
    abort();
  }
  memset(&stack[old_stack_size], 0, stack_size - old_stack_size);
}

int prompt(char *line) {
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
      case '>': stack_ptr = (stack_ptr + 1) % stack_size; break;
      case '<': stack_ptr = (stack_ptr - 1) % stack_size; break;
      case '?':
        printf("cmd:%c @%d[%d]\n", c[-1], stack_ptr, stack[stack_ptr]);
        break;

      case '[':
        if ((tmpline = malloc(sizeof *tmpline * (strlen(c) +1))) == NULL) {
          fprintf(stderr, "Virtual memory exceeded!\n");
          abort();
        }
        strcpy(tmpline, c);
        prompt(tmpline);
        while (*c != ']' && c[1] != '\0') {
          ++c;
        }
        ++c;
        break;

      case ']':
        if (stack[stack_ptr] != 0) {
          c = line;
        } else {
          free(line);
          return 0;
        } break;
    }
  }

  free(line);
  return prompt(readline(">> "));
}

int main(void) {
  resize_stack(30000);
  printf("Welcome to brainfuck! Use Ctrl-D to exit\n");
  return prompt(readline(">> "));
}
