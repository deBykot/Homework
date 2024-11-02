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


int PrintLayer(struct Node* root, int n) {
    if (!root) {
        return 0;
    }

    if (n == 0) {
        printf("%d ", root->data);
        return root->left || root->right;
    } else {
        int l = PrintLayer(root->left, n - 1);
        int r = PrintLayer(root->right, n - 1);
        return l || r;
    }
}


void PrintTree(struct Node* root) {
    int n = 0;

    
    while (PrintLayer(root, n)) {
        printf("\n");  
        ++n;
    }
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
    printf("\n");
    return 0;
}
