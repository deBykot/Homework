#include <stdio.h>
#include <string.h>

char f(char *s, int *n, char c) {
    int counts[256] = {0}; 
    int max_count = 0;
    char max_char = '\0';

    for (int i = 0; s[i] != '\0'; i++) {
        counts[s[i]]++;
    }

    for (int i = 0; i < 256; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            max_char = i;
        }
    }

    *n = counts[c];

    return max_char;
}

int main() {
    char str[30];
    fgets(str, 30, stdin);
    int count;
    char most_frequent = f(str, &count, 'c');

    printf("Символ 'c' встречается %d раз\n", count);
    printf("Самый частый: %c\n", most_frequent);

    return 0;
}
