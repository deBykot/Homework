#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int flag_eof = 0;
int flag_quot = 0;
int kol = 0;
int quot_pair = 0;
int new_line = 0;
int errors = 0;

const char single_symbols[5] = ";()<";
const char pair_symbols[4] = "&|>";

char *read_word(FILE *file_in) {
    int is_word = 0;
    int is_spec_sym = 0;
    char c = 0;
    int iword = 0;
    char *word = (char *)malloc(sizeof(char));
    word[iword] = 0;

    while ((c = getc(file_in)) != EOF) {
        if (flag_quot) {
            if (c == '\n') {
                new_line = 1;
                flag_quot = 0;
                fprintf(stderr,"Invalid quality of quotes\n");
                errors = 1;
                free(word);
                return NULL;
            } 
            if (c == '"') {
                flag_quot = 0;
            } 
            else {
                word = (char *)realloc(word, (iword + 2) * sizeof(char));
                word[iword] = c;
                iword++;
            }
        } 
        else if (is_spec_sym) {
            is_spec_sym = 0;
            if (c == word[iword - 1]) {
                word = (char *)realloc(word, (iword + 2) * sizeof(char));
                word[iword] = c;
                iword++;
                word[iword] = 0;
                return word;
            } 
            else {
                ungetc(c, file_in);
                word[iword] = 0;
                return word;
            }
        } 
        else if (isspace(c)) {
            if (is_word) { 
                if (c == '\n') {
                    new_line = 1;
                }
                word[iword] = 0;
                return word;
            } 
            else { 
                if (c == '\n') {
                    new_line = 1;
                    free(word);
                    word = NULL;
                    return NULL;
                }
                continue; 
            }
        } 
        else if (c == '"') {
            is_word = 1;
            flag_quot = 1;
        }
        else if (strchr(pair_symbols, c)) {
            if (is_word) {
                ungetc(c, file_in);
                word[iword] = 0;
                return word;
            } 
            else {
                is_spec_sym = 1;
                word = (char *)realloc(word, (iword + 2) * sizeof(char));
                word[iword] = c;
                iword++;
            }
        } 
        else if (strchr(single_symbols, c)) {
            if (is_word) {
                ungetc(c, file_in);
                word[iword] = 0;
                return word;
            } 
            else {
                word = (char *)realloc(word, (iword + 2) * sizeof(char));
                word[0] = c;
                word[1]= '\0';
                return word;
            }
        } 
        else {               
            is_word = 1;
            word = (char *)realloc(word, (iword + 2) * sizeof(char));
            word[iword] = c;
            iword++;
        }
    }
    flag_eof = 1;
    if (word != NULL) {
        free(word);
        return NULL;
    } else {
        word[iword] = 0;
        return word;
    }
}

char **add_to_array (char **array, char *word) {
    array = realloc(array, (kol + 2) * sizeof(char*));
    array[kol] = word;
    kol ++;
    array[kol] = NULL;
    return array;
}

void free_array(char** array) {
    while(*array != NULL) {
        free(*array);
        array++; 
    }
}

void print_array(char **array) {
    char** newarray = array;

    while (*newarray != NULL) {
        printf("%s\n",*newarray);
        newarray++;
    }
}

int main(int argc, char ** argv) {
    char *word;
    char **array;
    FILE *file_in;

    if (argc == 2) {
        file_in = fopen(argv[1], "r");
        if (!file_in) {
            fprintf(stderr, "Ошибка открытия файла\n");
            return 1;
        }
    } else {
        file_in = stdin;
    }


    while(!flag_eof) {
        kol = 0;
        array = malloc(sizeof(char*));
        new_line = 0;
        errors = 0;

        while(!flag_eof && !new_line && !errors) {
            word = read_word(file_in);
            array = add_to_array(array, word);
        }
        if (errors) {
            free_array(array);
            free(array);
            continue;
        }
        if (!flag_eof && (*array != NULL)) {
            print_array(array);
        } 
        free_array(array);
        free(array);
    }

    if (argc == 2) {
        fclose(file_in);
    } 
    printf("\n");
    return 0;
}