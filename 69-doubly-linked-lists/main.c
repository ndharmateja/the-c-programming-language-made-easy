#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "utils.h"

void test_create_and_destroy()
{
    printf("Running test_create_and_destroy...\n");
    DLL *list = dll_create();
    if (list == NULL)
    {
        printf("FAIL: dll_create returned NULL\n");
        return;
    }
    if (dll_size(list) != 0)
    {
        printf("FAIL: dll_create should initialize size to 0\n");
    }
    else
    {
        printf("PASS: dll_create\n");
    }

    dll_destroy(list);
    printf("PASS: dll_destroy (check valgrind for leaks)\n\n");
}

void test_push_and_pop_front()
{
    printf("Running test_push_and_pop_front...\n");
    DLL *list = dll_create();

    dll_push_front(list, 10);
    dll_push_front(list, 20);
    dll_push_front(list, 30); // List: 30, 20, 10

    if (dll_size(list) != 3)
    {
        printf("FAIL: dll_size should be 3 after three pushes\n");
    }

    if (dll_front(list) != 30)
    {
        printf("FAIL: dll_front should be 30\n");
    }

    int val1 = dll_pop_front(list); // Should remove 30
    int val2 = dll_pop_front(list); // Should remove 20
    int val3 = dll_pop_front(list); // Should remove 10

    if (val1 != 30 || val2 != 20 || val3 != 10)
    {
        printf("FAIL: pop_front did not return expected values\n");
    }
    else
    {
        printf("PASS: push_front & pop_front\n");
    }

    dll_destroy(list);
    printf("\n");
}

void test_push_and_pop_back()
{
    printf("Running test_push_and_pop_back...\n");
    DLL *list = dll_create();

    dll_push_back(list, 100);
    dll_push_back(list, 200);
    dll_push_back(list, 300); // List: 100, 200, 300

    if (dll_back(list) != 300)
    {
        printf("FAIL: dll_back should be 300\n");
    }

    int val1 = dll_pop_back(list); // Should remove 300
    int val2 = dll_pop_back(list); // Should remove 200
    int val3 = dll_pop_back(list); // Should remove 100

    if (val1 != 300 || val2 != 200 || val3 != 100)
    {
        printf("FAIL: pop_back did not return expected values\n");
    }
    else
    {
        printf("PASS: push_back & pop_back\n");
    }

    dll_destroy(list);
    printf("\n");
}

void test_mixed_operations()
{
    printf("Running test_mixed_operations...\n");
    DLL *list = dll_create();

    dll_push_front(list, 1); // List: 1
    dll_push_back(list, 2);  // List: 1, 2
    dll_push_front(list, 3); // List: 3, 1, 2

    if (dll_front(list) != 3 || dll_back(list) != 2)
    {
        printf("FAIL: mixed push operations incorrect\n");
    }

    int val1 = dll_pop_back(list);  // removes 2
    int val2 = dll_pop_front(list); // removes 3
    int val3 = dll_pop_back(list);  // removes 1

    if (val1 != 2 || val2 != 3 || val3 != 1 || dll_size(list) != 0)
    {
        printf("FAIL: mixed pop operations incorrect\n");
    }
    else
    {
        printf("PASS: mixed operations\n");
    }

    dll_destroy(list);
    printf("\n");
}

void test_edge_cases()
{
    printf("Running test_edge_cases...\n");

    DLL *null_list = NULL;
    printf("Should handle NULL inputs gracefully:\n");
    dll_push_front(null_list, 1);
    dll_push_back(null_list, 1);
    dll_pop_front(null_list);
    dll_pop_back(null_list);
    dll_front(null_list);
    dll_back(null_list);
    dll_is_empty(null_list);
    dll_size(null_list);
    dll_print_forward(null_list);
    dll_print_backward(null_list);

    DLL *list = dll_create();
    printf("Should return proper values when list is empty:\n");
    if (dll_pop_front(list) != -1)
        printf("FAIL: pop_front from empty should return -1\n");
    if (dll_pop_back(list) != -1)
        printf("FAIL: pop_back from empty should return -1\n");
    if (dll_front(list) != -1)
        printf("FAIL: front on empty should return -1\n");
    if (dll_back(list) != -1)
        printf("FAIL: back on empty should return -1\n");
    if (!dll_is_empty(list))
        printf("FAIL: is_empty should return true on empty list\n");

    dll_destroy(list);
    printf("PASS: edge cases\n\n");
}

void test_print_functions()
{
    printf("Running test_print_functions...\n");
    DLL *list = dll_create();

    printf("Expected forward: [Size: 0]\nActual: ");
    dll_print_forward(list);

    dll_push_back(list, 1);
    dll_push_back(list, 2);
    dll_push_back(list, 3);

    printf("Forward\nExpected: [Size: 3] 1 -> 2 -> 3 -> \nActual:   ");
    dll_print_forward(list);

    printf("Backward\nExpected: [Size: 3] 3 -> 2 -> 1 -> \nActual:   ");
    dll_print_backward(list);

    dll_destroy(list);
    printf("PASS: print functions (visually verify)\n\n");
}

void test_reverse()
{
    printf("Running reverse dll...\n");
    DLL *list = dll_create();

    dll_push_back(list, 1);
    dll_push_back(list, 2);
    dll_push_back(list, 3);

    printf("Before:\n");
    dll_print_forward(list);

    // Reverse
    reverse_linked_list(list);

    printf("After:\n");
    dll_print_forward(list);

    dll_destroy(list);
}

int main()
{
    test_create_and_destroy();
    test_push_and_pop_front();
    test_push_and_pop_back();
    test_mixed_operations();
    test_edge_cases();
    test_print_functions();
    test_reverse();

    printf("All tests finished.\n");
    return 0;
}
