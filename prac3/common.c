#include "common.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


FILE* open_file(const char* filename, const char* mode) {
    FILE* file = fopen(filename, mode);
    if (!file) {
        fprintf(stderr, "Ошибка открытия файла: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

void handle_bom(FILE* file, int* is_le) {
    unsigned char bom[2];
    if (fread(bom, 1, 2, file) == 2) {
        if (bom[0] == 0xFF && bom[1] == 0xFE) {
            *is_le = 1;
        } else if (bom[0] == 0xFE && bom[1] == 0xFF) {
            *is_le = 0;
        } else {
            fseek(file, 0, SEEK_SET);
        }
    } else {
        fseek(file, 0, SEEK_SET);
    }
}

void convert_utf16_to_utf8(FILE* input, FILE* output, int is_le) {
    unsigned short utf16_char;
    size_t read_bytes;
    while ((read_bytes = fread(&utf16_char, sizeof(char), 2, input)) == 2) {
        if (!is_le) {
            utf16_char = (utf16_char << 8) | (utf16_char >> 8);
        }
        if (utf16_char < 0x80) {
            fputc(utf16_char, output);
        } else if (utf16_char < 0x800) {
            fputc(0xC0 | (utf16_char >> 6), output);
            fputc(0x80 | (utf16_char & 0x3F), output);
        } else {
            fputc(0xE0 | (utf16_char >> 12), output);
            fputc(0x80 | ((utf16_char >> 6) & 0x3F), output);
            fputc(0x80 | (utf16_char & 0x3F), output);
        }
    }

    if (read_bytes == 1) {
        fprintf(stderr, "Нечетное количество байтов в UTF-16\n");
    }
}

void convert_utf8_to_utf16(FILE* input, FILE* output, int is_le) {
    int c;
    long offset = 0;
    while ((c = fgetc(input)) != EOF) {
        unsigned short utf16_char;
        if (c < 0x80) {
            utf16_char = c;
        } else if ((c & 0xE0) == 0xC0) {
            int next_byte = fgetc(input);
            if (next_byte == EOF || (next_byte & 0xC0) != 0x80) {
                fprintf(stderr, "Недопустимая последовательность при смещении %ld: 0x%X\n", offset, c);
                offset++;
                continue;
            }
            utf16_char = (c & 0x1F) << 6;
            utf16_char |= (next_byte & 0x3F);
            offset++;
        } else if ((c & 0xF0) == 0xE0) {
            int next_byte1 = fgetc(input);
            int next_byte2 = fgetc(input);
            if (next_byte1 == EOF || next_byte2 == EOF || 
                (next_byte1 & 0xC0) != 0x80 || (next_byte2 & 0xC0) != 0x80) {
                fprintf(stderr, "Недопустимая последовательность при смещении %ld: 0x%X\n", offset, c);
                offset += 2;
                continue;
            }
            utf16_char = (c & 0x0F) << 12;
            utf16_char |= (next_byte1 & 0x3F) << 6;
            utf16_char |= (next_byte2 & 0x3F);
            offset += 2;
        } else {
            fprintf(stderr, "Недопустимая последовательность при смещении %ld: 0x%X\n", offset, c);
            offset++;
            continue;
        }
        if (is_le) {
            unsigned char bytes[2];
            bytes[0] = utf16_char & 0xFF;
            bytes[1] = (utf16_char >> 8) & 0xFF;
            fwrite(bytes, sizeof(unsigned char), 2, output);
        } else {
            fwrite(&utf16_char, sizeof(unsigned short), 1, output);
        }
        offset++;
    }
}

Arguments parse_arguments(int argc, char* argv[]) {
    Arguments args = {NULL, NULL, 1};

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            args.input_filename = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            args.output_filename = argv[++i];
        } else if (strcmp(argv[i], "-le") == 0) {
            args.is_le = 1;
        } else if (strcmp(argv[i], "-be") == 0) {
            args.is_le = 0;
        } else {
            fprintf(stderr, "Неверный аргумент: %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }

    return args;
}

void fileclose(FILE* input, FILE* output) {
    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);
}