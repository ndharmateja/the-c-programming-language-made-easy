#include <stdlib.h>
#include "utils.h"

void reverse_linked_list(DLL *list)
{
    if (list == NULL || dll_is_empty(list))
        return;

    Node *curr = list->head;
    while (curr != NULL)
    {
        Node *prev = curr->prev;
        Node *next = curr->next;
        curr->prev = next;
        curr->next = prev;
        curr = next;
    }

    Node *temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}