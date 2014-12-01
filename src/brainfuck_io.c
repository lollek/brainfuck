#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

const char *progname = NULL; /* Set in main() */

#include "brainfuck_io.h"

char *read_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "%s: cannot access %s: %s\n",
        progname, filename, strerror(errno));
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) == -1) {
    fprintf(stderr, "%s: failed to seek in %s: %s\n",
        progname, filename, strerror(errno));
    fclose(file);
    return NULL;
  }

  long linelen = ftell(file);
  if (linelen == -1) {
    fprintf(stderr, "%s: cannot get size of %s: %s\n",
        progname, filename, strerror(errno));
    fclose(file);
    return NULL;
  }

  if (fseek(file, 0, SEEK_SET) == -1) {
    fprintf(stderr, "%s: failed to rewind %s: %s\n",
        progname, filename, strerror(errno));
    fclose(file);
    return NULL;
  }

  char *line = malloc(sizeof *line * (linelen +1));
  if (line == NULL) {
    fprintf(stderr, "%s: virtual memory exceeded!\n", progname);
    fclose(file);
    return NULL;
  }

  fread(line, sizeof *line, linelen, file);
  line[linelen] = '\0';
  fclose(file);
  return line;
}

FILE *open_file_for_reading(const char *filename) {
  FILE *return_val = fopen(filename, "r");
  if (return_val == NULL)
    fprintf(stderr, "%s: cannot read from file '%s': %s\n",
        progname, filename, strerror(errno));
  return return_val;
}


FILE *open_file_for_writing(const char *filename) {
  FILE *return_val = fopen(filename, "w");
  if (return_val == NULL)
    fprintf(stderr, "%s: cannot write to file '%s': %s\n",
        progname, filename, strerror(errno));
  return return_val;
}
