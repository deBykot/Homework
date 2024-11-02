#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void swap_pairs (char * str){

	if (str == NULL || *str == '\0')
		return;

	while (*str != '\0') {	
		char * tp = str, l;
		int flag = 0, i = 0, j = 0, k = 0, p = 0;
		while (*str != ','){
			i++;
			str++;
		}
		str++; i++; // ,
		while (*str != ',' && *str != '\0'){
			j++;
			str++;
		}
		if (*str == ','){
			j++;
			str++;
		}
		else {
			*str++ = ',';
			*str = '\0';
			flag = 1;
			j++;
		}
		for (k = 0; k < j; k++){
			l = *(str-1);
			for (p = i+j-1; p > 0; p--){
				*(tp+p) = *(tp+p-1);
			}
			*tp = l;
		}
		if (flag)
			*(str-1) = '\0';		
	}
}

int main (int argc, char * argv[]) {

	char str [1024] = "abc,DX567,674q,MAMA,aaa,ddd";
	printf("Input: %s\n", str);
	swap_pairs(str);
	printf("Output: %s\n", str);
	return 0;
}