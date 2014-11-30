#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <readline/readline.h>

#include "brainfuck_repl.h"
#include "brainfuck_nasm.h"

#include "brainfuck_main.h"

static const char *progname = NULL;

char *read_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
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

  long linelen = ftell(file);
  if (linelen == -1) {
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

  char *line = malloc(sizeof *line * (linelen +1));
  if (line == NULL) {
    fprintf(stderr, "%s: virtual memory exceeded!\n", progname);
    fclose(file);
    return NULL;
  }

  fread(line, sizeof *line, linelen, file);
  line[linelen] = '\0';
  fclose(file);
  return line;
}

int mode_repl(int argc, char **argv, size_t stack_size, int optind) {
  resize_brainfuck_repl_stack(stack_size);

  /* file arg */
  if (optind < argc) {
    for (int i = optind; i < argc; ++i) {
      char *data = read_file(argv[i]);
      if (data == NULL)
        continue;

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

int mode_nasm(int argc, char **argv, int optind, char *output_name) {
  bool free_output_name = false;
  for (int i = optind; i < argc; ++i) {
    FILE *input = fopen(argv[i], "r");
    if (input == NULL) {
      fprintf(stderr, "%s: cannot access %s: %s\n",
          progname, argv[i], strerror(errno));
      return 1;
    }

    if (output_name == NULL) {
      output_name = malloc(strlen(argv[i]) + 5 + 1);
      free_output_name = true;

      if (output_name == NULL) {
        fprintf(stderr, "%s: virtual memory exceeded!\n", progname);
        fclose(input);
        return 1;
      }

      strcpy(output_name, argv[i]);
      char *extension_ptr = strrchr(output_name, '.');
      if (extension_ptr == NULL)
        strcat(output_name, ".nasm");
      else
        strcpy(extension_ptr, ".nasm");
    }

    FILE *output = fopen(output_name, "w");
    if (output == NULL) {
      fprintf(stderr, "%s: cannot access %s: %s\n",
          progname, output_name, strerror(errno));
      fclose(input);
      if (free_output_name)
        free(output_name);
      return 1;
    }

    brainfuck_nasm_write(output, input);
    fclose(output);
    if (free_output_name)
      free(output_name);
    fclose(input);
  }
  return 0;
}

int main(int argc, char **argv) {
  char *outfile_name = NULL;
  output_t output = REPL;
  int option_index = 0;
  size_t starting_stack_size = 30000;
  struct option long_options[] = {
    {"nasm",        no_argument,       0, 'n'},
    {"output",      required_argument, 0, 'o'},
    {"stack-size",  required_argument, 0, 's'},
    {"help",        no_argument,       0, '0'},
    {0,             0,                 0,  0}
  };

  progname = argv[0];

  while (1) {
    int c = getopt_long(argc, argv, "no:s:", long_options, &option_index);
    if (c == -1) {
      break;
    }
    switch (c) {
      case 'n': output = NASM; break;
      case 'o': outfile_name = optarg;
      case 's': starting_stack_size = atoi(optarg); break;
      case '0':
        fprintf(stdout,
                "Usage: %s [OPTIONS] [FILE]\n"
                "Execute brainfuck with FILE(s) as source.\n\n"
                "  -n, --nasm               output nasm assembly code\n"
                "  -o, --output=FILE        name of output file\n"
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

  switch (output) {
    case REPL: return mode_repl(argc, argv, starting_stack_size, optind);
    case NASM: return mode_nasm(argc, argv, optind, outfile_name);
  }
}
