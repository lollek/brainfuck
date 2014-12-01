#include <string.h>

#include "brainfuck_arm.h"

#define op_put() fprintf(output, "\tbl\t_put\n");
#define op_get() fprintf(output, "\tbl\t_get\n");

static void op_inc(FILE *output, int N) {
  N %= 256; /* 256 rotations will loop a byte anyways */
  fprintf(output, "\tldrb\tr0,[r11]\n"
                  "\tadd\tr0,#%d\n"
                  "\tstrb\tr0,[r11]\n", N);
}

static void op_dec(FILE *output, int N) {
  N %= 256; /* 256 rotations will loop a byte anyways */
  fprintf(output, "\tldrb\tr0,[r11]\n"
                  "\tsub\tr0,#%d\n"
                  "\tstrb\tr0,[r11]\n", N);
}

static void op_mvr(FILE *output, int N) {
  if (N < 256) fprintf(output, "\tadd\tr11,#%d\n", N);
  else         fprintf(output, "\tldr\tr0,=%d\n"
                               "\tadd\tr11,r0\n", N);
}

static void op_mvl(FILE *output, int N) {
  if (N < 256) fprintf(output, "\tsub\tr11,#%d\n", N);
  else         fprintf(output, "\tldr\tr0,=%d\n"
                               "\tsub\tr11,r0\n", N);
}

#define op_tag()                             \
  fprintf(output, "\tldrb\tr0,[r11]\n"       \
                  "\tcmp\tr0,#0\n"           \
                  "\tbeq\tend%d_%d\n"        \
                  "tag%d_%d:",               \
          ptr, stack[ptr], ptr, stack[ptr]); \
  ++ptr;

#define op_jmp()                                                 \
  fprintf(output, "\tldrb\tr0,[r11]\n"                           \
                  "\tcmp\tr0,#0\n"                               \
                  "\tbne\ttag%d_%d\n"                            \
                  "end%d_%d:",                                   \
                  ptr -1, stack[ptr -1], ptr -1, stack[ptr -1]); \
  stack[--ptr]++;

int brainfuck_arm_write(FILE *output, FILE *input) {
  fprintf(output, ".text\n"
                  "\t.globl _start\n"
  /* PUT */       "_put:\t\t\t\t@ sys_write(stdout, r11, 1)\n"
                  "\tmov\tr7,#4\t\t@ sys_write\n"
                  "\tmov\tr0,#1\t\t@ stdout\n"
                  "\tmov\tr1,r11\n"
                  "\tswi\t#0\n"
                  "\tbx\tlr\n"
  /* GET */       "_get:\t\t\t\t@ sys_read(stdin, r11, 1)\n"
                  "\tmov\tr7,#3\t\t@ sys_read\n"
                  "\tmov\tr0,#0\t\t@ stdin\n"
                  "\tmov\tr1,r11\n"
                  "\tswi\t#0\n"
                  "\tbx\tlr\n"
  /* START */     "_start:\n"
                  "\tmov\tr0,#1\t\t@ Set stack size to 8192\n"
                  "\tlsl\tr0,#13\n"
                  "\tsub\tsp,r0\t\t@ Create stack\n"
                  "\tmov\tr11,sp\t\t@ Set ptr to start of stack\n"
                  "\tmov\tr2,#1\t\t@ r2 (for _put and _get) is always 1\n");

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

  fprintf(output, "\tmov\tr7,#1\t\t@ sys_exit(0)\n"
                  "\tmov\tr0,#0\n"
                  "\tswi\t#0\n");
  return 0;
}
