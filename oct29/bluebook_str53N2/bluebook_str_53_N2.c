#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* create_Node(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

int count_Nodes(struct Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + count_Nodes(root->left) + count_Nodes(root->right);
}

int main(void) {
    struct Node* root = create_Node(1);
    root->left = create_Node(2);
    root->right = create_Node(3);
    root->left->left = create_Node(4);
    root->left->right = create_Node(5);
    root->right->left = create_Node(6);
    root->right->right = create_Node(7);

    int totalNodes = count_Nodes(root);
    printf("Число узлов: %d\n", totalNodes);

    return 0;
}