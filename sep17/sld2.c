#include <stdio.h>
void reverse (char *s) {
	char c;
	int len = 0;
	while (*(s+len) != '\0') 
		len++;
	for (int i = 0; i < len/2; i++) {
		c = *(s+i);
		*(s+i) = *(s + len-i-1);
		*(s + len-i-1) = c;
   }
}

int main(void) {
	char str[20];
	fgets(str, 20, stdin);
	reverse(str);
	printf("%s\n", str);
	return 0;
}
