#ifndef BRAINFUCK_REPL_H
#define BRAINFUCK_REPL_H

#include <stddef.h>

/* Sets the brainfuck stack size.
 * This must happen before brainfuck() is called
 *
 * returns 0 on success
 * on error, the stack is unchanged */
int resize_brainfuck_repl_stack(size_t new_stack_size);

/* Free's the brainfuck stack */
void free_brainfuck_repl_stack();

/* Evaluates the code in line
 *
 * returns 0 on success */
int brainfuck_repl_eval(char *line);

#endif /* BRAINFUCK_REPL_H */
