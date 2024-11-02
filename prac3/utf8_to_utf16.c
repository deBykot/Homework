#include <stdio.h>
#include "common.h"

int main(int argc, char* argv[]) {
    Arguments args = parse_arguments(argc, argv);

    FILE* input = args.input_filename ? open_file(args.input_filename, "rb") : stdin;
    FILE* output = args.output_filename ? open_file(args.output_filename, "wb") : stdout;

    convert_utf8_to_utf16(input, output, args.is_le);
    fileclose(input, output);

    return 0;
}