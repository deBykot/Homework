#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1024
#define ALLOWED_LEN 5

void delete_string_less20(char *name) {
    int Temp_FD = open("temp", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    int FD = open(name, O_RDONLY, 0666);
    
    if (FD == -1) {
        fprintf(stderr, "Ошибка открытия файла\n");
        exit(1);
    }

    if (Temp_FD == -1) {
        fprintf(stderr, "Ошибка создания временного файла\n");
        exit(1);
    }

    char buf[MAX_LEN];
    int LineLen = 0;
    int BytesRead;
    char c;

    while((BytesRead = read(FD, &c, sizeof(char))) > 0) {
        if (c != '\n') {    
            buf[LineLen++] = c;
        } else {
            buf[LineLen] = '\0';

            if (LineLen <= ALLOWED_LEN) {
                if (write(Temp_FD, buf, LineLen) == -1 || write(Temp_FD, "\n", sizeof(char)) == -1) {
                    fprintf(stderr, "Ошибка записи в временный файл\n");
                    exit(1);
                }
            }
            LineLen = 0;
        }
    }

    if (BytesRead == -1) {
        fprintf(stderr, "Ошибка чтения из файл\n");
        exit(1);
    }

    close(Temp_FD);
    close(FD);

    Temp_FD = open("temp", O_RDONLY);
    FD = open(name, O_WRONLY | O_TRUNC);

    while ((BytesRead = read(Temp_FD, &c, sizeof(char))) > 0) {
        if (write(FD, &c, sizeof(char)) == -1) {
            fprintf(stderr, "Ошибка записи в исходный файл\n");
            close(FD);
            close(Temp_FD);
            exit(1);
        }
    }

    close(Temp_FD);
    close(FD);

    unlink("temp");

}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Неверное количество аргументов\n");
        exit(1);
    }

    delete_string_less20(argv[1]);

    return 0;
}
