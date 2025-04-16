#include <stdio.h>
#include <stdlib.h>
#include "dll.h"

// Initialization of DLL with sentinel nodes
DLL *dll_create()
{
    DLL *list = (DLL *)malloc(sizeof(DLL));
    list->size = 0;
    list->head = (Node *)malloc(sizeof(Node));
    list->tail = (Node *)malloc(sizeof(Node));

    list->head->next = list->tail;
    list->tail->prev = list->head;
    return list;
}

// Create and initialize a new DLL
void dll_destroy(DLL *list)
{
    if (list == NULL)
        return;

    Node *curr = list->head;
    while (curr != NULL)
    {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

// Insertion
void dll_push_front(DLL *list, int data)
{
    if (list == NULL)
        return;

    // Create new node
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = data;

    // Insert new node after the head sentinel
    new_node->next = list->head->next;
    list->head->next->prev = new_node;
    new_node->prev = list->head;
    list->head->next = new_node;

    // Increment size
    list->size++;
}

void dll_push_back(DLL *list, int data)
{
    if (list == NULL)
        return;

    // Create new node
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = data;

    // Insert new node before the tail sentinel
    new_node->prev = list->tail->prev;
    list->tail->prev->next = new_node;
    new_node->next = list->tail;
    list->tail->prev = new_node;

    // Increment size
    list->size++;
}

// Deletion
int dll_pop_front(DLL *list)
{
    if (list == NULL)
        return -1;

    if (dll_is_empty(list))
        return -1;

    Node *node_to_remove = list->head->next;
    int data = node_to_remove->data;

    node_to_remove->next->prev = list->head;
    list->head->next = node_to_remove->next;

    free(node_to_remove);
    list->size--;
    return data;
}

int dll_pop_back(DLL *list)
{
    if (list == NULL)
        return -1;

    if (dll_is_empty(list))
        return -1;

    Node *node_to_remove = list->tail->prev;
    int data = node_to_remove->data;

    node_to_remove->prev->next = list->tail;
    list->tail->prev = node_to_remove->prev;

    free(node_to_remove);
    list->size--;
    return data;
}

// Access
int dll_front(DLL *list)
{
    if (list == NULL)
        return -1;

    if (dll_is_empty(list))
        return -1;

    return list->head->next->data;
}

int dll_back(DLL *list)
{
    if (list == NULL)
        return -1;

    if (dll_is_empty(list))
        return -1;

    return list->tail->prev->data;
}

int dll_is_empty(DLL *list)
{
    if (list == NULL)
        return 1;
    return list->size == 0;
}

int dll_size(DLL *list)
{
    if (list == NULL)
        return 0;
    return list->size;
}

// Utilities
void dll_print_forward(DLL *list)
{
    if (list == NULL)
        return;
    printf("[Size: %d] ", list->size);
    Node *curr = list->head->next;
    for (int i = 0; i < list->size; i++)
    {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }
    printf("\n");
}

void dll_print_backward(DLL *list)
{
    if (list == NULL)
        return;
    printf("[Size: %d] ", list->size);
    Node *curr = list->tail->prev;
    for (int i = 0; i < list->size; i++)
    {
        printf("%d -> ", curr->data);
        curr = curr->prev;
    }
    printf("\n");
}
