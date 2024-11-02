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


int maxD(struct Node* root) {
    if (root == NULL)
        return 0;

    int l = maxD(root->left);
    int r = maxD(root->right);

    if (l > r) {
        return l + 1;
    }
    else {
        return r + 1;
    }
}

void PrintLevel(struct Node* root, int l, int cD, int mD) {
    if (root == NULL) {
        int spaces = (1 << (mD - cD));  
        for (int i = 0; i < spaces; i++) {
            printf(" ");
        }
        return;
    }

    if (l == 1) {
        int spaces = (1 << (mD - cD));  
        for (int i = 0; i < spaces / 2; i++) {
            printf(" ");
        }
        printf("%d ", root->data);
        for (int i = 0; i < spaces / 2; i++) {
            printf(" ");
        }

    } else if (l > 1) {
        PrintLevel(root->left, l - 1, cD + 1, mD);
        PrintLevel(root->right, l - 1, cD + 1, mD);
    }
}

void PrintTree(struct Node* root) {
    int mD = maxD(root);

    for (int i = 1; i <= mD; i++) {
        PrintLevel(root, i, 1, mD);
        printf("\n");
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

    return 0;
}
