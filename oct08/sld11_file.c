#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    FILE* f1 = fopen(argv[1], "r");
    FILE* f2 = fopen(argv[2], "w");

    if (!f1 || !f2) {
        printf("error");
        return 1;
    }
    
    char line[1024];
    while (fgets(line, sizeof(line), f1) != NULL) {
        fputs(line, f2);
        fputs(line, f2);
    }

    fclose(f1);
    fclose(f2);

    return 0;
}
