#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <readline/readline.h>

#ifndef DEBUG
#define DEBUG 0
#endif

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

void eval_bf(char *line) {
  char *c = line;
  char *tmpline = NULL;
  if (c == NULL) {
    return;
  }

  while (*c) {
    switch (*c++) {
      case '.': putchar(stack[stack_ptr]); break;
      case ',': stack[stack_ptr] = getchar(); break;
      case '+': ++stack[stack_ptr]; break;
      case '-': --stack[stack_ptr]; break;
      case '>': if (stack_ptr +1 < stack_size) {
                  ++stack_ptr;
                } break;
      case '<': if (stack_ptr > 0) {
                  --stack_ptr;
                } break;
      case '?':
        printf("cmd:%c @%d[%d]\n", c[-1], stack_ptr, stack[stack_ptr]);
        break;

      case '[':
        if ((tmpline = malloc(sizeof *tmpline * (strlen(c) +1))) == NULL) {
          fprintf(stderr, "Virtual memory exceeded!\n");
          abort();
        }
        strcpy(tmpline, c);
        eval_bf(tmpline);
        for (; *c != ']' && *c != '\0'; ++c);
        for (; *c == ']'; ++c);
        break;

      case ']':
        if (stack[stack_ptr] != 0) {
          c = line;
        } else {
          free(line);
          return;
        } break;
    }
  }
}

int main(int argc, char **argv) {
  int option_index = 0;
  size_t starting_stack_size = 30000;
  struct option long_options[] = {
    {"stack-size",  required_argument, 0, 's'},
    {"help",        no_argument,       0, '0'},
    {0,             0,                 0,  0}
  };

  while (1) {
    int c = getopt_long(argc, argv, "s:", long_options, &option_index);
    if (c == -1) {
      break;
    }
    switch (c) {
      case 's': starting_stack_size = atoi(optarg); break;
      case '0': 
        fprintf(stdout,
                "Usage: %s [OPTIONS]\n\n"
                "  -s, --stack-size=N       set stack size (default 30000)\n"
                "      --help               display this help and exit\n",
                argv[0]);
        return 0;
      default: fprintf(stderr, "Try '%s --help' for more information\n",
                        argv[0]);
               return 1;
    }
  }

  resize_stack(starting_stack_size);

  /* stdin pipe */
  if (!isatty(STDIN_FILENO)) {
    char buf[BUFSIZ];
    while (fgets(buf, BUFSIZ, stdin) != NULL) {
      eval_bf(buf);
    }

  /* file arg */
  } else if (optind < argc) {
    FILE *file;
    char buf[BUFSIZ];
    int i;
    for (i = optind; i < argc; ++i) {
      if ((file = fopen(argv[i], "r")) == NULL) {
        fprintf(stderr, "%s: unable to open %s: %s\n",
                argv[0], argv[i], strerror(errno));
        continue;
      }
      while (fgets(buf, BUFSIZ, file) != NULL) {
        eval_bf(buf);
      }
      fclose(file);
    }

  /* repl */
  } else {
    char *line = NULL;
    printf("Welcome to brainfuck! Use Ctrl-D to exit\n");
    while ((line = readline(">> ")) != NULL) {
      eval_bf(line);
      free(line);
    }
  }
  return 0;
}
