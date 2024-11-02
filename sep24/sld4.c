#include <stdio.h>
#include <string.h>


const char *keywords[] = {"break", "char", "continue", "do", "double", "else", "float", "for", "if", 
    "int", "auto", "short", "struct", "unsigned", "long", "signed", "switch", "void", "case", "default", 
    "goto", "register", "sizeof", "typedef", "volatile", "extern", "return", "static", "union", "while",
    "enum", "const", NULL};


const char *ptrkey(const char *str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return keywords[i];
        }
    }
    return NULL;
}   


int main(void) {
    const char *test1 = "double";
    const char *test2 = "Abay";

    const char *result1 = ptrkey(test1);
    const char *result2 = ptrkey(test2);

    if (result1) {
        printf("\"%s\" is a keyword.\n", result1);
    } else { 
        printf("\"%s\" is not a keyword.\n", test1);
    }
    
    if (result2) {
        printf("\"%s\" is a keyword.\n", result2);
    } else { 
        printf("\"%s\" is not a keyword.\n", test2);
    }
    
    return 0;
}