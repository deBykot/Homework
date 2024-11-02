#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *input_string() {
    char *str = NULL;
    int ch, len = 0;
    unsigned long long size = 0;

    while ((ch = getchar()) != EOF) {
        if (len + 1 >= size) {
            size = size ? size * 2 : 1;
            str = realloc(str, size);
        }
        str[len++] = ch;
    }
    if (str) {
        str[len] = '\0';
    }
    return str;
}


void sort_strings(char *arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (strcmp(arr[i], arr[j]) > 0) {
                char *temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}


void print_strings(char *arr[], int n) {
    for (int i = 0; i < n; ++i) {
        if (arr[i] && arr[i][0] != '\0') { 
            printf("%s\n", arr[i]);
        }
    }
}

int main() {
    char **arr = NULL;
    int n = 0, v = 0;
    char *input;


    while ((input = input_string()) != NULL) {
        if (input[0] != '\0') {
            if (v == n) {
                v = v ? v * 2 : 1;
                arr = realloc(arr, v * sizeof(char *));
            }
        }
            
        arr[n++] = input;
    }


    sort_strings(arr, n);
    print_strings(arr, n);

    for (int i = 0; i < n; ++i) {
        free(arr[i]);
    }
    free(arr);

    return 0;
}
