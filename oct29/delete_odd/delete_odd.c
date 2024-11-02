#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

void DeleteFirst(Node **head) {
    if (*head == NULL) return;

    Node *temp = *head;
    if ((*head)->next == *head) {
        *head = NULL;
    } else {
        Node *current = *head;
        while (current->next != *head) {
            current = current->next;
        }
        current->next = (*head)->next;
        *head = (*head)->next;
    }
    free(temp);
}

void deleteNode(Node *head, Node *node) {
    if (head == NULL || node == NULL) return;

    Node *current = head;
    while (current->next != node) {
        current = current->next;
    }
    current->next = node->next;
    free(node);
}

void delete_odd(Node **head) {
    if (*head == NULL) return;

    Node *current = *head;
    Node *previous = NULL;
    int count = 0;
    int len = 0;

    do {
        len++;
        current = current->next;
    } while (current != *head);

    current = *head;

    if (len == 1) {
        DeleteFirst(head);
        return;
    }

    while (len > 0) {
        Node *next = current->next;

        if (count == 0) {
            DeleteFirst(head);
            current = *head;
        } else if (count % 2 == 0) {
            deleteNode(*head, current);
        } else {
            previous = current;
        }

        current = next;
        len--;
        count++;
    }

    if (previous != NULL) {
        previous->next = *head;
    }
}

Node* create_node(int data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = new_node; 
    return new_node;
}

void append(Node **head, int data) {
    Node *new_node = create_node(data);
    if (*head == NULL) {
        *head = new_node;
        return;
    }

    Node *current = *head;
    while (current->next != *head) {
        current = current->next;
    }
    current->next = new_node;
    new_node->next = *head;
}

void print_list(Node *head) {
    if (head == NULL) return;

    Node *current = head;
    do {
        printf("%d ", current->data);
        current = current->next;
    } while (current != head);
    printf("\n");
}

int main() {
    Node *head = NULL;

    append(&head, 1);
    append(&head, 2);
    append(&head, 3);
    append(&head, 4);
    append(&head, 5);

    print_list(head);
    delete_odd(&head);
    print_list(head);

    return 0;
}