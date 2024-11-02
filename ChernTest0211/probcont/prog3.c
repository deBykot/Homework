#include <stdio.h>

int main(void) {
	int a, b, n, i, j;

	scanf("%d%d%d", &a, &b, &n);

	if (n <= 0) return 0;
	
	for (i = 0; i < n; i++) 
		printf(" ");

	for (i = a; i < b; i++) 
		printf("%*d", n + 1, i); 
	
	printf("\n");
	
	for (i = a; i < b; i++) {
		printf("%*d", n, i);
		for (j = a; j < b; j++) {
			printf("%*lld", n + 1, ((long long) i) * j);
		}
		printf("\n");	
	}
	
	return 0;

}
