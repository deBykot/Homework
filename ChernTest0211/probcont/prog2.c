#include <stdio.h>

int main(void) {
	long long s;
	char c;
	s = 0;
	while (scanf("%c", &c) != EOF) {
		if ((c >= '0') && (c <= '9')) {
			s += (c - '0');
		}

		else if ((c >= 'a') && (c <= 'f')) {
			s += 10 + c - 'a';
		}		
		
		else if ((c >= 'A') && (c <= 'F')) {
			s += 10 + c - 'A';
		}
	}

	printf("%lld\n", s);
}
		
