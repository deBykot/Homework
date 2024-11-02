#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>


void create_and_write_bin(char *name) {
    int FileDescriptor = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (FileDescriptor == -1) {
        fprintf(stderr, "Ошибка создания");
        close(FileDescriptor);
        exit(1);
    }

    long long numbers[] = {10, 8, 16, 3, 2, 15};
    ssize_t bytes_written = write(FileDescriptor, numbers, sizeof(numbers));

    if (bytes_written == -1 || bytes_written < sizeof(numbers)) {
        fprintf(stderr, "Неверная запись");
        close(FileDescriptor);
        exit(1);
    }

    close(FileDescriptor);
}

void find_min_and_double(char *name) {
    int FileDescriptor = open(name, O_RDWR);

    if (FileDescriptor == -1) {
        fprintf(stderr, "Ошибка открытия");
        close(FileDescriptor);
        exit(1);
    }

    long long min;
    long long cur;
    size_t l_size = sizeof(long long);
    off_t min_pos = 0;
    ssize_t bytes_read = read(FileDescriptor, &min, l_size);

    if (bytes_read == -1) {
        fprintf(stderr, "Ошибка чтения");
        close(FileDescriptor);
        exit(1);
    }

    off_t pos = l_size;
    off_t end_pos;
    while(read(FileDescriptor, &cur, l_size) == l_size) {
        if (cur < min) {
            min = cur;
            min_pos = pos;
        }
        pos += l_size;
    }

    lseek(FileDescriptor, min_pos + l_size, SEEK_SET);
    ftruncate(FileDescriptor, pos + l_size);

    end_pos = pos;
    while(end_pos > min_pos + l_size) {
        end_pos -= l_size;
        lseek(FileDescriptor, end_pos, SEEK_SET);
        read(FileDescriptor, &cur, l_size);
        lseek(FileDescriptor, end_pos + l_size, SEEK_SET);
        write(FileDescriptor, &cur, l_size);

    }
    lseek(FileDescriptor, min_pos + l_size, SEEK_SET);
    write(FileDescriptor, &min, l_size);

    close(FileDescriptor);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Неверные аргументы");
        return 1;
    }

    //create_and_write_bin(argv[1]);
    find_min_and_double(argv[1]);

    return 0;

}