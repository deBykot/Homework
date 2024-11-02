#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


void open_files(char **mas, int c, FILE *files[], int is_eof[], int numbers[]) {
    int i;
    for (i = 0; i < c; i++) {
      files[i] = fopen(mas[i], "r");
      if (!files[i]) return;
      if (fscanf(files[i], "%d", &numbers[i]) == 1) {
         is_eof[i] = 0;
      }
      else is_eof[i] = 1; 
    }    
}
  
void process(int c, FILE *files[], int is_eof[], int numbers[] ) {
        int cnt;
        for (int i = 0; i < c; i++) {
           if (is_eof[i] == 0) cnt++;
        }
    do {
         int min = INT_MAX;
         int imin = -1;
         for (int i = 0; i < c; i++) {
            if (imin == -1 && is_eof[i] == 0) {
               min = numbers[i];
               imin = i;
            }
            if (numbers[i] <= min && is_eof[i] == 0) {
               min = numbers[i];
               imin = i;
            }
         }   
         if (imin != -1) {
             printf("%d", min);
             if (fscanf(files[imin], "%d", &numbers[imin]) == -1) {
                is_eof[imin] = 1;
                numbers[imin] = INT_MAX;
                cnt++;
             }
         }        
      } while (cnt<c);
}


  
int main(int argc, char **argv) {
     if (argc < 2) return 0;
     int file_count = argc - 1;
     FILE *files[file_count];
     int is_eof[file_count];
     int numbers[file_count];     
       open_files(&argv[1], file_count, files, is_eof, numbers); 
       for (int i = 0; i < file_count; i++) {
          printf("%d",is_eof[i]);
       }
       //process(file_count, files, is_eof, numbers);
       for (int i = 1; i <= file_count; i++) {
           if (files[i]) fclose(files[i]);
       }
             
       return 0;
}
