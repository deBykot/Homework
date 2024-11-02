#include <stdio.h>
#include <stdlib.h>


struct Node {
    int data;
    struct Node *next;
    struct Node *prev;
};

struct Node *create_Node(int data) {
    struct Node *NewNode = (struct Node*)malloc(sizeof(struct Node));
    NewNode->data = data;
    return NewNode;
}

void print_list(struct Node* node) {
    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}

void double_node(struct Node *root) {
    while (root != NULL) {
        struct Node *TempNode = create_Node(root->data);
        
        TempNode->next = root->next;
        TempNode->prev = root;
        
        if (root->next != NULL) {
            root->next->prev = TempNode;
        }
        root->next = TempNode;
        root = TempNode->next;
    }
}

void freelist(struct Node* node) {
    struct Node* temp;
    while (node != NULL) {
        temp = node;
        node = node->next;
        free(temp);
    }
}

int main(void) {
    struct Node* root = create_Node(1);
    root->next = create_Node(2);
    root->next->prev = root;
    root->next->next = create_Node(3);
    root->next->next->prev = root->next;
    root->next->next->next = create_Node(4);
    root->next->next->next->prev = root->next->next;
    root->next->next->next->next = create_Node(5);
    root->next->next->next->next->prev = root->next->next->next;
    root->next->next->next->next->next = create_Node(6);
    root->next->next->next->next->next->prev = root->next->next->next->next;

    
    print_list(root);
    double_node(root);
    print_list(root);
    freelist(root);

    return 0;
}