#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int last_time = 0;

void Hi() {
    time_t current_time = time(NULL);
    // printf("%ld", current_time);
    // printf("\n%ld", current_time - last_time);
    if (current_time - last_time < 1) {
        printf("\nBy-by!\n");
        exit(0);
    } else {
        printf("\nHi!\n");
    }

    last_time = current_time;
}

int main(void) {
    signal(SIGINT, Hi);

    while (1) {

    }

    return 0;
}