#ifndef BRAINFUCK_MAIN_H
#define BRAINFUCK_MAIN_H

typedef enum output_t {
  REPL,
  NASM,
  ARM
} output_t;

/* Read all contents of a file to the stack
 *
 * Returns a pointer to the data on success (which will need to be freed)
 * Returns NULL on error
 */
char *read_file(const char *filename);

/* Switches to Read-Eval-Print-Loop mode
 *
 * Returns 0 on success, 1 on error */
int mode_repl(int argc, char **argv, size_t stack_size, int optind);

/* Switches to NASM mode
 *
 * Returns 0 on success, 1 on error */
int mode_compile(int argc, char **argv, int optind, char *outfile_name,
                 output_t asm_type);

#endif /* BRAINFUCK_MAIN_H */
