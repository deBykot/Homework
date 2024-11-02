#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* NewNode(int elem) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));

    temp->data = elem;
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}
struct Node *insert_word(struct Node *root, const char *word) {
    if (root == NULL) {
        return NewNode(word);
    }
    
    int cmp = strcmp(word, root->word);
    if (cmp == 0) {
        root->count++;  
    } else if (cmp < 0) {
        root->left = insert_word(root->left, word);
    } else {
        root->right = insert_word(root->right, word);
    }
    return root;
}

void PrintT(struct Node* root, int space) {
    if (root == NULL) {
        return;
    }
    
    space += 10; 

    PrintT(root->right, space);

    for (int i = 10; i < space; i++) {
        printf(" ");
    }

    printf("%d\n", root->data);
    PrintT(root->left, space);
}

void PrintTree(struct Node* root) {
    return PrintT(root, 0);
}

void process_input(struct Node **root) {
    int size = 128;
    char *word = (char *)malloc(size * sizeof(char));  
    int index = 0;
    char c;
    
    while ((c = getchar()) != EOF) {
        if (isspace(c)) {
            if (index > 0) {
                word[index] = '\0';
                *root = insert_word(*root, word); 
                index = 0;
            }
        } 
        else {
            if (index >= size - 1) {
                size *= 2;
                word = (char *)realloc(word, size * sizeof(char));
            }
            word[index++] = c;
        }
    }
}

int main(void) {
    struct Node *root = NULL;
    process_input(&root);
    PrintTree(root);

    free(root);
    return 0;
}