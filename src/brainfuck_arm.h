#ifndef BRAINFUCK_ARM_H
#define BRAINFUCK_ARM_H

#include <stdio.h>

/* Takes brainfuck in input and write arm assembly to output
 *
 * Return 0 on success */
int brainfuck_arm_write(FILE *output, FILE *input);

#endif /* BRAINFUCK_ARM_H */
