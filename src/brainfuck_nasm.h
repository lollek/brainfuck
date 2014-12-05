#ifndef BRAINFUCK_NASM_H
#define BRAINFUCK_NASM_H

#include <stdio.h>

/* Takes brainfuck in input and write nasm aseembly to output
 *
 * Return 0 on success */
int brainfuck_nasm_write(FILE *output, FILE *input, size_t stacksize);

#endif /* BRAINFUCK_NASM_H */
