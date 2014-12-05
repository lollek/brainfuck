#ifndef BRAINFUCK_ASM_H
#define BRAINFUCK_ASM_H

#include <stdio.h>

typedef enum output_t {
  NONE,
  NASM,
  ARM
} output_t;

/* Takes brainfuck in input and writes assembly to output
 *
 * Return 0 on success */
int brainfuck_make_asm(FILE *output, FILE *input, output_t type,
                       size_t stacksize);

#endif /* BRAINFUCK_ASM_H */
