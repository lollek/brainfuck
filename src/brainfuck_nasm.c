#include "brainfuck_nasm.h"

static int counter = 0;

static inline void op_put(FILE *output) {
  fprintf(output, "\tmov\teax,4\n"
                  "\tmov\tebx,1\n"
                  "\tmov\tecx,ebp\n"
                  "\tmov\tedx,1\n"
                  "\tint\t80h\n");
}

static inline void op_get(FILE *output) {
  fprintf(output, "\tmov\teax,3\n"
                  "\tmov\tebx,0\n"
                  "\tmov\tecx,ebp\n"
                  "\tmov\tedx,1\n"
                  "\tint\t80h\n");
}

static inline void op_inc(FILE *output) {
  fprintf(output, "\tinc\tbyte[ebp]\n");
}

static inline void op_dec(FILE *output) {
  fprintf(output, "\tdec\tbyte[ebp]\n");
}

static inline void op_mvr(FILE *output) {
  fprintf(output, "\tinc\tebp\n");
}

static inline void op_mvl(FILE *output) {
  fprintf(output, "\tdec\tebp\n");
}

static void op_tag(FILE *output) {
  fprintf(output, "tag%d:\n"
                  "\tpush tag%d\n",
                  counter, counter);
  ++counter;
}

static void op_jmp(FILE *output) {
  fprintf(output, "\tpop\teax\n"
                  "\tcmp\tbyte[ebp],0\n"
                  "\tjz\tnojmp%d\n"
                  "\tjmp\teax\n"
                  "nojmp%d:\n",
                  counter, counter);
  ++counter;
}

int brainfuck_nasm_write(FILE *output, FILE *input) {
  fprintf(output, "section .text\n"
                  "\tglobal _start\n"
                  "_start:\n"
                  "\tsub\tesp,7000\n"
                  "\tmov\tebp,esp\n");

  int op;
  while ((op = fgetc(input)) != EOF) {
    switch (op) {
      case '.': op_put(output); break;
      case ',': op_get(output); break;
      case '+': op_inc(output); break;
      case '-': op_dec(output); break;
      case '>': op_mvr(output); break;
      case '<': op_mvl(output); break;
      case '[': op_tag(output); break;
      case ']': op_jmp(output); break;
    }
  }

  fprintf(output, "\tmov\teax,1\n"
                  "\tmov\tebx,0\n"
                  "\tint\t80h\n");
  return 0;
}
