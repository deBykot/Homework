#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

void handle_error(char *str) {
    fprintf(stderr, "%s", str);
    exit(1);
}

void create_and_write_bin(char *name) {
    int FileDescriptor = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (FileDescriptor == -1) {
        fprintf(stderr, "Ошибка создания");
        close(FileDescriptor);
        exit(1);
    }

    int numbers[] = {10, -8};
    ssize_t bytes_written = write(FileDescriptor, numbers, sizeof(numbers));

    if (bytes_written == -1 || bytes_written < sizeof(numbers)) {
        fprintf(stderr, "Неверная запись");
        close(FileDescriptor);
        exit(1);
    }

    close(FileDescriptor);
}

void rearrange(const char *filename) {
    int fd = open(filename, O_RDWR);
    if (fd == -1) 
        handle_error("Ошибка открытия файла\n");

    int buffer[BUFFER_SIZE];
    int negative[BUFFER_SIZE], non_negative[BUFFER_SIZE];
    int neg_count = 0, non_neg_count = 0;
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        int num_elements = bytesRead / sizeof(int);

        for (int i = 0; i < num_elements; i++) {
            if (buffer[i] < 0)
                negative[neg_count++] = buffer[i];
            else
                non_negative[non_neg_count++] = buffer[i];
        }
    }

    if (bytesRead == -1) handle_error("Ошибка чтения файла\n");

    if (lseek(fd, 0, SEEK_SET) == -1) handle_error("Ошибка с позицией\n");

    if (write(fd, negative, neg_count * sizeof(int)) == -1 ||
        write(fd, non_negative, non_neg_count * sizeof(int)) == -1)
        handle_error("Ошибка записи в файл\n");

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        handle_error("Неверное количество аргументов");
    }
    //create_and_write_bin(argv[1]);
    rearrange(argv[1]);

    return 0;
}