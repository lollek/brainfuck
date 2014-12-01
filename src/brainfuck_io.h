#ifndef BRAINFUCK_IO_H
#define BRAINFUCK_IO_H

/* Read all contents of a file to the stack
 *
 * Returns a pointer to the data on success (which will need to be freed)
 * Returns NULL on error
 */
char *read_file(const char *filename);

FILE *open_file_for_reading(const char *filename);
FILE *open_file_for_writing(const char *filename);

#endif /* BRAINFUCK_IO_H */
