#ifndef __BRAINFUCK__H__
#define __BRAINFUCK__H__

#include <stddef.h>

/* Sets the brainfuck stack size.
 * This must happen before brainfuck() is called
 *
 * returns 0 on success
 * on error, the stack is unchanged */
int resize_brainfuck_stack(size_t new_stack_size);

/* Free's the brainfuck stack */
void free_brainfuck_stack();

/* Evaluates the code in line
 *
 * returns 0 on success */
int brainfuck(char *line);

#endif /* __BRAINFUCK__H__ */
