#include <stdio.h>

int main(void) {
	long long a, b, c, d, razn;
	int n, i, j, er;
	er = -1;
	n = 4;
	scanf("%lld%lld%lld%lld",&a, &b, &c, &d);
	long long buf[4] = {a, b, c, d};

	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			if (buf[i] > buf[j]) {
				int temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	
	razn = buf[1] - buf[0];
	
	for (i = 0; i < n - 1; i++) {
		if ((buf[i + 1] - buf[i]) != razn) {
			printf("%lld %d\n", buf[0], er);
			return 0;
		}
	}
	

	printf("%lld %lld\n", buf[0], razn);
	
	return 0;
}