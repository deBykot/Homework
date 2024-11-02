#include <stdio.h>

int main() {
    int x;
    
    x = 0b11111111111111111111111110001111; 
	printf("%08x\n", x);
	
    int mask = 0b111 << 4; 

    x = x ^ mask;

    printf("Result: %d\n", x); 
    printf("Result (binary): %08x\n", x); 
    
    return 0;
}

