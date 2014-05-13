#ifndef __BRAINFUCK__H__
#define __BRAINFUCK__H__

#include <stddef.h>

/* Sets the brainfuck stack size. 
 * This must happen before brainfuck() is called */
void resize_stack(size_t new_stack_size);

/* Evaluates the code in line */
void brainfuck(char *line);

#endif /* __BRAINFUCK__H__ */
