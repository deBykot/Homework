#include <stdio.h>
#include <stdlib.h>

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


int main(void) {
    struct Node* root = NewNode(1);

    root->left = NewNode(1);
    root->right = NewNode(11);

    root->left->left = NewNode(2);
    root->left->right = NewNode(3);
    root->right->left = NewNode(12);
    root->right->right = NewNode(13);

    root->left->right->right = NewNode(0);
    root->left->right->left = NewNode(1);

    
    PrintTree(root);

    free(root);
}