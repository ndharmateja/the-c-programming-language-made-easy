#ifndef DLL_H
#define DLL_H

// Node structure
typedef struct Node
{
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

// Doubly Linked List structure
typedef struct DLL
{
    Node *head;
    Node *tail;
    int size;
} DLL;

// DLL function declarations

// Initialization
DLL *dll_create();           // Create and initialize a new DLL
void dll_destroy(DLL *list); // Free all nodes and the list itself

// Insertion
void dll_push_front(DLL *list, int data); // Insert at head
void dll_push_back(DLL *list, int data);  // Insert at tail

// Deletion
int dll_pop_front(DLL *list); // Remove from head and return data
int dll_pop_back(DLL *list);  // Remove from tail and return data

// Access
int dll_front(DLL *list);    // Get data from head
int dll_back(DLL *list);     // Get data from tail
int dll_is_empty(DLL *list); // Check if list is empty
int dll_size(DLL *list);     // Return current size of list

// Utilities
void dll_print_forward(DLL *list);  // Print list from head to tail
void dll_print_backward(DLL *list); // Print list from tail to head

#endif // DLL_H
