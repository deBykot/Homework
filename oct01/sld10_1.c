#include <stdio.h>
#include <stdlib.h>
// Структура узла списк
struct Node {
    int data;
    struct Node* next;
};


struct Node* createNode(int data) {
    struct Node * newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void swapFirstLast(struct Node** head) {
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }

    struct Node *prev = NULL, *last = *head, *first = *head;

    while (last->next != NULL) {
        prev = last;
        last = last->next;
    }

    if (first != last) {
        prev->next = first; 
        struct Node* temp = first->next; 

        first->next = NULL; 
        last->next = temp; 

        *head = last;
    }
}


void printList(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

int main() {

    struct Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);

    printf("Исходный список:\n");
    printList(head);

    // Меняем местами первое и последнее звено
    swapFirstLast(&head);

    printf("Список после перестановки:\n");
    printList(head);

    struct Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}