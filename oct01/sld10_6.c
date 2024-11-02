#include <stdio.h>
#include <stdlib.h>


typedef struct Node_str {
    int value;
    struct Node_str *next;
} Node;

void push(Node **head, int value) {
    Node *tmp = malloc(sizeof(Node));
    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
}


int pop(Node **head) {
    Node *out;
    int value;
    out = *head;
    *head = (*head)->next;
    value = out->value;
    free(out);
    return value;
}


int peek(Node *head) {
    if(head == NULL) {
        exit(0);
    }
    return head->value;
}


void printStack(const Node *head) {
    while (head) {
        printf("%d ", head->value);
        head = head->next;
    }
    printf("\n");
}

unsigned int getSize(const Node *head) {
    unsigned int size = 0;
    while (head) {
        size++;
        head = head->next;
    }
    return size;
}

int main(void) {
    Node *head = NULL;
    for (int i = 0; i < 10; i++) {
        push(&head, i);
    }
    printf("%d\n", getSize(head));
    printStack(head);
    while (head) {
        printf("%d ", peek(head));
        printf("%d\n", pop(&head));
    }
    return 0;
}