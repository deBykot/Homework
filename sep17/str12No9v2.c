#include <stdio.h>

int main() {
    int x;
    
    x = 0b11111111111111111111111110001111; 
	printf("%08x\n", x);
	
    x = x ^ 0b1110000;

    printf("Result: %d\n", x); 
    printf("Result (binary): %08x\n", x); 
    
    return 0;
}

