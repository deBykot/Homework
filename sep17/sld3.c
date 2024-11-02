#include <stdio.h>
#include <string.h>

void cdelete(char *s, char c) {
	int len = strlen(s);
	int k, m;
	for(k = 0, m = 0; k < len; k++) {
		if (*(s+k) != c) 
			s[m++] = s[k];
   }
	s[m] = '\0';
}

int main(void){
	char str[20];
	fgets(str, 20, stdin);
	cdelete(str, 'c');
	printf("%s\n", str);
	return 0;
}
