#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


struct TreeNode {
    char *word;
    int count;
    struct TreeNode *left;
    struct TreeNode *right;
};


struct TreeNode *create_node(const char *word) {
    struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    node->word = strdup(word); 
    node->count = 1;
    node->left = NULL;
    node->right = NULL; 
    return node;
}

void PrintT(struct TreeNode* root, int space) {
    if (root == NULL) {
        return;
    }

    space += 10; 

    PrintT(root->right, space);

    for (int i = 10; i < space; i++) {
        printf(" ");
    }

    printf("%s\n", root->word);
    PrintT(root->left, space);
}

void PrintTree(struct TreeNode* root) {
    return PrintT(root, 0);
}

struct TreeNode *insert_word(struct TreeNode *root, const char *word) {
    if (root == NULL) {
        return create_node(word);
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


int total_words(struct TreeNode *root) {
    if (root == NULL) return 0;
    return root->count + total_words(root->left) + total_words(root->right);
}


void tree_to_array(struct TreeNode *root, struct TreeNode **array, int *index) {
    if (root == NULL) return;
    tree_to_array(root->left, array, index);
    array[(*index)++] = root;
    tree_to_array(root->right, array, index);
}


int compare(const void *a, const void *b) {
    struct TreeNode *nodeA = *(struct TreeNode **)a;
    struct TreeNode *nodeB = *(struct TreeNode **)b;
    return nodeB->count - nodeA->count;
}


void print_dictionary(struct TreeNode **array, int size, int total_count) {
    for (int i = 0; i < size; i++) {
        struct TreeNode *node = array[i];
        printf("%s %d %.6f\n", node->word, node->count, (double)node->count / total_count);
    }
}


void process_input(struct TreeNode **root) {
    int size = 128;
    char *word = (char *)malloc(size * sizeof(char));  
    int index = 0;
    char c;
    
    while ((c = getchar()) != EOF) {
        if (ispunct(c)) {  
            if (index > 0) { 
                word[index] = '\0';
                *root = insert_word(*root, word);
                index = 0;
            }
            char punct[2] = {c, '\0'}; 
            *root = insert_word(*root, punct);
        } 
        else if (isspace(c)) {
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
    
    if (index > 0) {
        word[index] = '\0';
        *root = insert_word(*root, word);
    }
    
    free(word); 
}

void freetree(struct TreeNode* node) {
    if (node != NULL) {
        freetree(node->left);
        freetree(node->right);
        free(node->word); 
        free(node);
    }
}

int main() {
    struct TreeNode *root = NULL;
    process_input(&root);
    
    PrintTree(root);
    freetree(root);
    
    return 0;
}
