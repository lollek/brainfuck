#include <string.h>

#include "brainfuck_nasm.h"

#define op_put() fprintf(output, "\tcall\t_put\n");
#define op_get() fprintf(output, "\tcall\t_get\n");

static void op_inc(FILE *output, int N) {
  if (N == 1) fprintf(output, "\tinc\tbyte[ebp]\n");
  else        fprintf(output, "\tadd\tbyte[ebp],%d\n", N);
}

static void op_dec(FILE *output, int N) {
  if (N == 1) fprintf(output, "\tdec\tbyte[ebp]\n");
  else        fprintf(output, "\tsub\tbyte[ebp],%d\n", N);
}

static void op_mvr(FILE *output, int N) {
  if (N == 1) fprintf(output, "\tinc\tebp\n");
  else        fprintf(output, "\tadd\tebp,%d\n", N);
}

static void op_mvl(FILE *output, int N) {
  if (N == 1) fprintf(output, "\tdec\tebp\n");
  else        fprintf(output, "\tsub\tebp,%d\n", N);
}

#define op_tag()                             \
  fprintf(output, "\tcmp\tbyte[ebp],0\n"     \
                  "\tjz\tend%d_%d\n"         \
                  "tag%d_%d:",               \
          ptr, stack[ptr], ptr, stack[ptr]); \
  ++ptr;

#define op_jmp()                                                 \
  fprintf(output, "\tcmp\tbyte[ebp],0\n"                         \
                  "\tjnz\ttag%d_%d\n"                            \
                  "end%d_%d:",                                   \
                  ptr -1, stack[ptr -1], ptr -1, stack[ptr -1]); \
  stack[--ptr]++;

int brainfuck_nasm_write(FILE *output, FILE *input, size_t stacksize) {
  fprintf(output, "section .text\n"
                  "\tglobal _start\n"
  /* PUT */       "_put:\t\t\t\t; sys_write(stdout, ebp, 1)\n"
                  "\tmov\teax,4\t\t; sys_write\n"
                  "\tmov\tebx,1\t\t; stdout\n"
                  "\tmov\tecx,ebp\n"
                  "\tint\t80h\n"
                  "\tret\n"
  /* GET */       "_get:\t\t\t\t; sys_read(stdin, ebp, 1)\n"
                  "\tmov\teax,3\t\t; sys_read\n"
                  "\tmov\tebx,0\t\t; stdin\n"
                  "\tmov\tecx,ebp\n"
                  "\tint\t80h\n"
                  "\tret\n"
  /* START */     "_start:\n"
                  "\tsub\tesp,%d\t; Init stack\n"
                  "\tmov\tebp,esp\t\t; Set ptr to start of stack\n"
                  "\tmov\tedx,1\t\t; edx (for _put and _get) is always 1\n",
                  stacksize);

  int stack[256] = { 0 };
  unsigned char ptr = 0;
  int op;

  int queued_op = 0;
  int num_queued = 0;
  while ((op = fgetc(input)) != EOF) {
    if (strchr(".,+-><[]", op) == NULL)
      continue;

    if (queued_op != 0) {
      if (queued_op == op) {
        ++num_queued;
        continue;
      }

      switch(queued_op) {
        case '+': op_inc(output, num_queued); break;
        case '-': op_dec(output, num_queued); break;
        case '>': op_mvr(output, num_queued); break;
        case '<': op_mvl(output, num_queued); break;
      }
      queued_op = 0;
      num_queued = 0;
    }

    switch (op) {
      case '.': op_put(); break;
      case ',': op_get(); break;
      case '+': /* FALLTHROUGH */
      case '-': /* FALLTHROUGH */
      case '>': /* FALLTHROUGH */
      case '<': queued_op = op; num_queued = 1; break;
      case '[': op_tag(); break;
      case ']': op_jmp(); break;
    }
  }

  fprintf(output, "\tmov\teax,1\t\t; sys_exit(0)\n"
                  "\tmov\tebx,0\n"
                  "\tint\t80h\n");
  return 0;
}
