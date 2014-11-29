#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <readline/readline.h>

#include "brainfuck_repl.h"

static const char *progname = NULL;

char *read_file(const char *filename) {
  FILE *file;
  char *line;
  long linelen;

  if ((file = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "%s: cannot access %s: %s\n",
        progname, filename, strerror(errno));
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) == -1) {
    fprintf(stderr, "%s: failed to seek in %s: %s\n",
        progname, filename, strerror(errno));
    fclose(file);
    return NULL;
  }

  if ((linelen = ftell(file)) == -1) {
    fprintf(stderr, "%s: cannot get size of %s: %s\n",
        progname, filename, strerror(errno));
    fclose(file);
    return NULL;
  }

  if (fseek(file, 0, SEEK_SET) == -1) {
    fprintf(stderr, "%s: failed to rewind %s: %s\n",
        progname, filename, strerror(errno));
    fclose(file);
    return NULL;
  }

  if ((line = malloc(sizeof *line * (linelen +1))) == NULL) {
    fprintf(stderr, "%s: virtual memory exceeded!\n", progname);
    fclose(file);
    return NULL;
  }

  fread(line, sizeof *line, linelen, file);
  line[linelen] = '\0';
  fclose(file);
  return line;
}

int main(int argc, char **argv) {
  int option_index = 0;
  size_t starting_stack_size = 30000;
  struct option long_options[] = {
    {"stack-size",  required_argument, 0, 's'},
    {"help",        no_argument,       0, '0'},
    {0,             0,                 0,  0}
  };

  progname = argv[0];

  while (1) {
    int c = getopt_long(argc, argv, "s:", long_options, &option_index);
    if (c == -1) {
      break;
    }
    switch (c) {
      case 's': starting_stack_size = atoi(optarg); break;
      case '0':
        fprintf(stdout,
                "Usage: %s [OPTIONS] [FILE]\n"
                "Execute brainfuck with FILE(s) as source.\n\n"
                "  -s, --stack-size=N       set stack size (default 30000)\n"
                "      --help               display this help and exit\n\n"
                "With no FILE, a repl is started\n"
                ,argv[0]);
        return 0;
      default: fprintf(stderr, "Try '%s --help' for more information\n",
                        argv[0]);
               return 1;
    }
  }

  resize_brainfuck_repl_stack(starting_stack_size);

  /* file arg */
  if (optind < argc) {
    int i;
    for (i = optind; i < argc; ++i) {
      char *data = read_file(argv[i]);
      brainfuck_repl_eval(data);
      free(data);
    }

  /* repl */
  } else {
    char *line = NULL;
    printf("Welcome to brainfuck! Use Ctrl-D to exit\n");
    while ((line = readline(">> ")) != NULL) {
      brainfuck_repl_eval(line);
      free(line);
    }
  }
  free_brainfuck_repl_stack();
  return 0;
}
