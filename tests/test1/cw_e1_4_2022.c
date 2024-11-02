#include <stdio.h>

int main (int argc, char **argv){
	//if (argc != 2) {
	//	printf ("Input_error!\n");
	//	return 1;
	//}
	int size = sizeof(long long);
///////////////////////////////////////////////////////
	FILE* fp= fopen ("test_e1_4", "w");
	long long a[5] = {-1,7,-1,3,-2};
	for (int i =0; i < 5; i++)
		fwrite (&a[i], size, 1, fp);
	fclose (fp);
///////////////////////////////////////////////////////	

	fp = fopen("test_e1_4" /*argv[1]*/, "r+");
	if (fp == NULL){
		printf ("File_open_error!\n");
		return 2;
	}
	long long min, cur, pos_min, end, t;

	t = fread(&min, size, 1, fp);
	if (t)
		pos_min = 0;
	else
		return 0;

	while (fread (&cur, size, 1, fp)){
		if (cur <= min){ 
			min = cur;
			pos_min = ftell(fp) - size;
		} 
	}
	fseek (fp, -size, SEEK_END);
	end = ftell(fp);

	while (end >= pos_min){
		fread (&cur, size, 1, fp);
		fwrite (&cur, size, 1, fp);
		if(end == 0)
			break;
		fseek(fp, -3*size, SEEK_CUR);
		end = ftell(fp);
	}
	fseek(fp, 0, SEEK_SET);
	while (fread (&cur, size, 1, fp))
		printf ("%lld ", cur);
	printf ("\n");
	return 0;
}