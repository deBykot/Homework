#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const char* input_filename;
    const char* output_filename;
    int is_le;
} Arguments;

FILE* open_file(const char* filename, const char* mode);

void handle_bom(FILE* file, int* is_le);

void convert_utf16_to_utf8(FILE* input, FILE* output, int is_le);

void convert_utf8_to_utf16(FILE* input, FILE* output, int is_le);

Arguments parse_arguments(int argc, char* argv[]);

void fileclose(FILE* input, FILE* output);

#endif // COMMON_H
