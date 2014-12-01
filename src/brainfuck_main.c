#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <readline/readline.h>

#include "brainfuck_io.h"
#include "brainfuck_repl.h"
#include "brainfuck_nasm.h"
#include "brainfuck_arm.h"

extern const char *progname;

typedef enum output_t {
  NONE,
  NASM,
  ARM
} output_t;

static int mode_shell(size_t stack_size) {
  resize_brainfuck_repl_stack(stack_size);
  printf("Welcome to brainfuck! Use Ctrl-D to exit\n");

  char *line = NULL;
  while ((line = readline(">> ")) != NULL) {
    brainfuck_repl_eval(line);
    free(line);
  }

  free_brainfuck_repl_stack();
  return 0;
}

static int mode_interpret(size_t stack_size, const char *infile) {
  resize_brainfuck_repl_stack(stack_size);

  char *data = read_file(infile);
  if (data != NULL) {
    brainfuck_repl_eval(data);
    free(data);
  }

  free_brainfuck_repl_stack();
  return 0;
}

static int mode_compile(char *infile_name, char *outfile_name,
                        output_t asm_type) {
  FILE *infile = open_file_for_reading(infile_name);
  if (infile == NULL)
    return 1;

  /* If no given outfile_name, set infile_name minus extension, plus .o */
  bool free_outfile_name = false;
  if (outfile_name == NULL) {
    free_outfile_name = true;

    if ((outfile_name = malloc(strlen(infile_name) + 5 + 1)) == NULL) {
      fprintf(stderr, "%s: virtual memory exceeded!\n", progname);
      fclose(infile);
      return 1;
    }

    strcpy(outfile_name, infile_name);
    char *ext = strchr(outfile_name, '.');
    if (ext != NULL) strcpy(ext, ".nasm");
    else             strcat(outfile_name, ".nasm");
  }

  FILE *outfile = open_file_for_writing(outfile_name);
  if (outfile == NULL) {
    fclose(infile);
    if (free_outfile_name)
      free(outfile_name);
    return 1;
  }

  switch (asm_type) {
    case NASM: brainfuck_nasm_write(outfile, infile); break;
    case ARM: brainfuck_arm_write(outfile, infile); break;
    default: break;
  }

  fclose(infile);
  fclose(outfile);
  if (free_outfile_name)
    free(outfile_name);
  return 0;
}

int main(int argc, char **argv) {
  int option_index = 0;
  struct option long_options[] = {
    {"arm",         no_argument,       0, 'a'},
    {"nasm",        no_argument,       0, 'n'},
    {"output",      required_argument, 0, 'o'},
    {"stack-size",  required_argument, 0, 's'},
    {"help",        no_argument,       0, '0'},
    {0,             0,                 0,  0}
  };

  size_t starting_stack_size = 30000;
  progname = argv[0];
  char *outfile_name = NULL;
  char *infile_name = NULL;
  output_t output = NONE;

  while (1) {
    int c = getopt_long(argc, argv, "ano:s:", long_options, &option_index);
    if (c == -1) {
      break;
    }
    switch (c) {
      case 'a': output = ARM; break;
      case 'n': output = NASM; break;
      case 'o': outfile_name = optarg;
      case 's': starting_stack_size = atoi(optarg); break;
      case '0':
        fprintf(stdout,
                "Usage: %s [OPTIONS] FILE\n"
                "Execute or create opcode for brainfuck with FILE as source.\n\n"
                "  -a, --arm                output ARM assembly code\n"
                "  -n, --nasm               output NASM assembly code\n"
                "  -o, --output=FILE        name of output file\n"
                "  -s, --stack-size=N       set stack size (default 30000)\n"
                "      --help               display this help and exit\n\n"
                "With no FILE, a repl is started\n"
                ,progname);
        return 0;
      default:
        fprintf(stderr, "Try '%s --help' for more information\n", progname);
        return 1;
    }
  }

  int num_files = argc - optind;
  if (num_files > 1) {
    fprintf(stderr, "%s: Can only take one file\n", progname);
    return 1;

  } else if (num_files != 1)
    return mode_shell(starting_stack_size);

  infile_name = argv[optind];
  switch (output) {
    case NONE: return mode_interpret(starting_stack_size, infile_name);
    case ARM:  /* FALLTHROUGH */
    case NASM: return mode_compile(infile_name, outfile_name, output);
  }
}
