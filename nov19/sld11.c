#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int i = 1;

void increment() {
    printf("\n");
    i++;
}

void By() {
    printf("\nBy-by!\n");
    exit(0);
}

int main(void) {
    signal(SIGINT, increment);
    signal(SIGQUIT, By);

    while(1) {
        printf("%d\n", i);
        sleep(1);
    }
    return 0;
}