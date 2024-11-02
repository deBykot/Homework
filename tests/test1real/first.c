#include <stdio.h>
#include <stdlib.h>

// Функция для подсчета количества единиц в двоичном представлении числа
int count_ones(int num) {
    int count = 0;
    while (num) {
        count += num & 1;
        num >>= 1;
    }
    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

    int num = atoi(argv[1]);
    if (num <= 0) {
        fprintf(stderr, "Error: The number must be a positive integer.\n");
        return 1;
    }

    int original_num = num;
    int ones_count = count_ones(num);

    // Устанавливаем старший бит (самый левый бит) в 1 или 0
    int highest_bit = 1 << (sizeof(int) * 8 - 1);
    if (ones_count % 2 == 0) {
        num |= highest_bit;  // Устанавливаем старший бит в 1
    } else {
        num &= ~highest_bit; // Устанавливаем старший бит в 0
    }

    printf("Original number: 0x%x\n", original_num);
    printf("Modified number: 0x%x\n", num);

    return 0;
}