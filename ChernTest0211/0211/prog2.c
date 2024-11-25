#include <stdio.h>

int main(void) {
	long long c;
	int N = 0;
	int K = 0;
	int i, j;
	int maxx = 100000;
	long long buf[maxx];
	
	while (scanf("%lld", &c) != EOF) {
		buf[N] = c;
		N += 1;
	}
	
	for (i = 0; i < N; i++) {
		for (j = i + 1; j < N; j++) {
			if (buf[i] > buf[j]) {
				long long temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	
	K = N * 9 / 10;
	
	printf("%lld\n", buf[K]);
	
	return 0;
}