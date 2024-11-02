#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    struct Node *prev, *next;
    char *elem;
};

struct List {
    struct Node *first, *last;
};


void process(struct List *pl, const char *str) {
    struct Node *cur = pl->first;
    while (cur != NULL) {
        struct Node *next = cur->next;
        if (strcmp(cur->elem, str) == 0) {
            // Удаление звена
            if (cur->prev) {
                cur->prev->next = cur->next;
            } else {
                pl->first = cur->next;
            }
            if (cur->next) {
                cur->next->prev = cur->prev;
            } else {
                pl->last = cur->prev;
            }
            free(cur->elem);
            free(cur);
        } else if (strcmp(cur->elem, str) > 0) {
            // Перемещение звена в конец списка
            if (cur->prev) {
                cur->prev->next = cur->next;
            } else {
                pl->first = cur->next;
            }
            if (cur->next) {
                cur->next->prev = cur->prev;
            } else {
                pl->last = cur->prev;
            }
            if (pl->last) {
                pl->last->next = cur;
            }
            cur->prev = pl->last;
            cur->next = NULL;
            pl->last = cur;
            if (pl->first == NULL) {
                pl->first = cur;
            }
        }
        cur = next;
    }
}


