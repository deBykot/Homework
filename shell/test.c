#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
	for (int i = 0; i < 10; ++i) {
		printf("\ni = %d", i);
		sleep(1);
	}
	return 0;
}