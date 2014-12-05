#include <string.h>
#include <assert.h>

#include "brainfuck_asm.h"

#define TRANSPILER_STACK_SIZE 256

static FILE *output;
static FILE *input;
static output_t type;
static unsigned stack[TRANSPILER_STACK_SIZE];
static unsigned char ptr;

static void session_start() {
  switch (type) {
    case ARM: fprintf(output, ".text\n\t.globl _start\n"); break;
    case NASM: fprintf(output, "section .text\n\tglobal _start\n"); break;
    case NONE: break;
  }
}

static void define_put() {
  switch(type) {
    case ARM:
      fprintf(output,
        "_put:\t\t\t\t@ sys_write(stdout, r11, 1)\n"
        "\tmov\tr7,#4\t\t@ sys_write\n"
        "\tmov\tr0,#1\t\t@ stdout\n"
        "\tmov\tr1,r11\n"
        "\tswi\t#0\n"
        "\tbx\tlr\n");
      break;
    case NASM:
      fprintf(output,
        "_put:\t\t\t\t; sys_write(stdout, ebp, 1)\n"
        "\tmov\teax,4\t\t; sys_write\n"
        "\tmov\tebx,1\t\t; stdout\n"
        "\tmov\tecx,ebp\n"
        "\tint\t80h\n"
        "\tret\n");
      break;
    case NONE: break;
  }
}


static void define_get() {
  switch(type) {
    case ARM:
      fprintf(output,
        "_get:\t\t\t\t@ sys_read(stdin, r11, 1)\n"
        "\tmov\tr7,#3\t\t@ sys_read\n"
        "\tmov\tr0,#0\t\t@ stdin\n"
        "\tmov\tr1,r11\n"
        "\tswi\t#0\n"
        "\tbx\tlr\n");
      break;
    case NASM:
      fprintf(output,
        "_get:\t\t\t\t; sys_read(stdin, ebp, 1)\n"
        "\tmov\teax,3\t\t; sys_read\n"
        "\tmov\tebx,0\t\t; stdin\n"
        "\tmov\tecx,ebp\n"
        "\tint\t80h\n"
        "\tret\n");
      break;
    case NONE: break;
  }
}


static void define_start(size_t stacksize) {
  (void)stacksize;
  switch(type) {
    case ARM:
      fprintf(output,
        "_start:\n"
        "\tmov\tr0,#1\t\t@ Set stack size to 8192\n"
        "\tlsl\tr0,#13\n"
        "\tsub\tsp,r0\t\t@ Create stack\n"
        "\tmov\tr11,sp\t\t@ Set ptr to start of stack\n"
        "\tmov\tr2,#1\t\t@ r2 is always 1\n");
      break;
    case NASM:
      fprintf(output,
        "_start:\n"
        "\tsub\tesp,%d\t; Init stack\n"
        "\tmov\tebp,esp\t\t; Set ptr to start of stack\n"
        "\tmov\tedx,1\t\t; edx (for _put and _get) is always 1\n", stacksize);
      break;
    case NONE: break;
  }
}

static void define_exit() {
  switch(type) {
    case ARM:
      fprintf(output,
        "\tmov\tr7,#1\t\t@ sys_exit(0)\n"
        "\tmov\tr0,#0\n"
        "\tswi\t#0\n");
      break;
    case NASM:
      fprintf(output,
        "\tmov\teax,1\t\t; sys_exit(0)\n"
        "\tmov\tebx,0\n"
        "\tint\t80h\n");
      break;
    case NONE: break;
  }
}

static void op_put() {
  switch(type) {
    case ARM: fprintf(output, "\tbl\t_put\n"); break;
    case NASM: fprintf(output, "\tcall\t_put\n"); break;
    case NONE: break;
  }
}

static void op_get() {
  switch(type) {
    case ARM: fprintf(output, "\tbl\t_get\n"); break;
    case NASM: fprintf(output, "\tcall\t_get\n"); break;
    case NONE: break;
  }
}

static void op_inc(int N) {
  switch(type) {
    case ARM:
      N %= 256; /* 256 rotations will loop a byte anyways */
      fprintf(output,
        "\tldrb\tr0,[r11]\n"
        "\tadd\tr0,#%d\n"
        "\tstrb\tr0,[r11]\n", N);
      break;
    case NASM:
      if (N == 1) fprintf(output, "\tinc\tbyte[ebp]\n");
      else        fprintf(output, "\tadd\tbyte[ebp],%d\n", N);
      break;
    case NONE: break;
  }
}

static void op_dec(int N) {
  switch(type) {
    case ARM:
      N %= 256; /* 256 rotations will loop a byte anyways */
      fprintf(output,
        "\tldrb\tr0,[r11]\n"
        "\tsub\tr0,#%d\n"
        "\tstrb\tr0,[r11]\n", N);
      break;
    case NASM:
      if (N == 1) fprintf(output, "\tdec\tbyte[ebp]\n");
      else        fprintf(output, "\tsub\tbyte[ebp],%d\n", N);
      break;
    case NONE: break;
  }
}

static void op_mvr(int N) {
  switch(type) {
    case ARM:
      if (N < 256) fprintf(output, "\tadd\tr11,#%d\n", N);
      else         fprintf(output, "\tldr\tr0,=%d\n"
                                   "\tadd\tr11,r0\n", N);
      break;
    case NASM:
      if (N == 1) fprintf(output, "\tinc\tebp\n");
      else        fprintf(output, "\tadd\tebp,%d\n", N);
      break;
    case NONE: break;
  }
}

static void op_mvl(int N) {
  switch(type) {
    case ARM:
      if (N < 256) fprintf(output, "\tsub\tr11,#%d\n", N);
      else         fprintf(output, "\tldr\tr0,=%d\n"
                                   "\tsub\tr11,r0\n", N);
      break;
    case NASM:
      if (N == 1) fprintf(output, "\tdec\tebp\n");
      else        fprintf(output, "\tsub\tebp,%d\n", N);
      break;
    case NONE: break;
  }
}

static void op_tag() {
  switch(type) {
    case ARM:
      fprintf(output,
        "\tldrb\tr0,[r11]\n"
        "\tcmp\tr0,#0\n"
        "\tbeq\tend%d_%d\n"
        "tag%d_%d:",
        ptr, stack[ptr], ptr, stack[ptr]);
      break;
    case NASM:
      fprintf(output,
        "\tcmp\tbyte[ebp],0\n"
        "\tjz\tend%d_%d\n"
        "tag%d_%d:",
        ptr, stack[ptr], ptr, stack[ptr]);
      break;
    case NONE: break;
  }
  ++ptr;
}

static void op_jmp() {
  switch(type) {
    case ARM:
      fprintf(output,
        "\tldrb\tr0,[r11]\n"
        "\tcmp\tr0,#0\n"
        "\tbne\ttag%d_%d\n"
        "end%d_%d:",
        ptr -1, stack[ptr -1], ptr -1, stack[ptr -1]);
      break;
    case NASM:
      fprintf(output,
        "\tcmp\tbyte[ebp],0\n"
        "\tjnz\ttag%d_%d\n"
        "end%d_%d:",
        ptr -1, stack[ptr -1], ptr -1, stack[ptr -1]);
      break;
    case NONE: break;
  }
  stack[--ptr]++;
}

int brainfuck_make_asm(FILE *_output, FILE *_input, output_t _type,
                       size_t stacksize) {
  output = _output;
  input = _input;
  type = _type;

  assert(type != NONE);

  /* Create header */
  session_start();
  define_put();
  define_get();
  define_start(stacksize);

  /* Transpile brainfuck to asm */
  memset(stack, 0, TRANSPILER_STACK_SIZE);
  ptr = 0;

  int queued_op = 0;
  int num_queued = 0;

  int op;
  while ((op = fgetc(input)) != EOF) {
    if (strchr(".,+-><[]", op) == NULL)
      continue;

    if (queued_op != 0) {
      if (queued_op == op) {
        ++num_queued;
        continue;
      }

      switch(queued_op) {
        case '+': op_inc(num_queued); break;
        case '-': op_dec(num_queued); break;
        case '>': op_mvr(num_queued); break;
        case '<': op_mvl(num_queued); break;
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
      case '[': op_tag(stack, ptr); break;
      case ']': op_jmp(stack, ptr); break;
    }
  }

  /* Create clean exit */
  define_exit();
  return 0;
}
