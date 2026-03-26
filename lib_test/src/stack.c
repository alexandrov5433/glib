#include <stdio.h>
#include <stdlib.h>
#include "../../include/stack.h"
#include "../include/stack.h"

void printIntProcessor(void *const ptr)
{
    if (ptr == NULL)
    {
        printf("NULL ");
        return;
    }
    printf("%d ", *(int *)ptr);
}
int dynamicStackTest()
{
    printf("=== DynamicStack Tests ===\n\n");

    /* Create DynamicStack */
    DynamicStack *ds = newDynamicStack();
    if (!ds)
    {
        printf("FAIL: newDynamicStack returned NULL\n");
        return 1;
    }
    printf("PASS: newDynamicStack created\n");

    /* Test empty state */
    printf("isEmptyDS (expected 1): %d\n", isEmptyDS(ds));

    /* Push elements */
    const int NUM_ELEMENTS = 300;
    int *values[NUM_ELEMENTS];

    for (int i = 0; i < NUM_ELEMENTS; i++)
    {
        values[i] = malloc(sizeof(int));
        *values[i] = i * 10;

        int res = pushDS(ds, values[i]);
        if (res != 0)
        {
            printf("FAIL: pushDS failed at %d (code %d)\n", i, res);
            return 1;
        }
    }
    printf("PASS: pushDS pushed %d elements\n", NUM_ELEMENTS);

    /* Stack should not be empty */
    printf("isEmptyDS (expected 0): %d\n", isEmptyDS(ds));

    /* Peek test */
    int *peeked = (int *)peekDS(ds);
    if (!peeked || *peeked != (NUM_ELEMENTS - 1) * 10)
    {
        printf("FAIL: peekDS returned wrong value\n");
        return 1;
    }
    printf("PASS: peekDS returned %d\n", *peeked);

    /* Process stack */
    printf("processDS output (bottom -> top): ");
    processDS(ds, printIntProcessor);
    printf("\nPASS: processDS executed\n");

    /* Pop elements and test LIFO order */
    printf("Popping elements:\n");
    for (int i = NUM_ELEMENTS - 1; i >= 0; i--)
    {
        int *popped = (int *)popDS(ds);
        if (!popped || *popped != i * 10)
        {
            printf("FAIL: popDS expected %d, got %d\n",
                   i * 10,
                   popped ? *popped : -1);
            return 1;
        }
        printf("  popped %d\n", *popped);
        free(popped);
    }
    printf("PASS: popDS returned correct LIFO order\n");

    /* Stack should be empty again */
    printf("isEmptyDS (expected 1): %d\n", isEmptyDS(ds));

    /* Pop on empty stack */
    if (popDS(ds) != NULL)
    {
        printf("FAIL: popDS on empty stack should return NULL\n");
        return 1;
    }
    printf("PASS: popDS on empty stack returned NULL\n");

    /* Peek on empty stack */
    if (peekDS(ds) != NULL)
    {
        printf("FAIL: peekDS on empty stack should return NULL\n");
        return 1;
    }
    printf("PASS: peekDS on empty stack returned NULL\n");

    /* Cleanup */
    freeDS(ds);
    printf("PASS: freeDS executed\n");

    printf("\n=== ALL DynamicStack TESTS PASSED ===\n");
    return 0;
}

void stackProcessor(void *const ptr)
{
    *(int *)ptr = 0;
}
void stackTest()
{
    puts("#################    Test: Stack    #################");
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    int five = 5;
    Stack *stack = newStack(5);

    if (isEmptyS(stack))
    {
        puts("OK: Stack is empty.");
    }
    else
    {
        puts("Eror: Stack must be empty.");
    }

    pushS(stack, &one);
    pushS(stack, &two);
    pushS(stack, &three);
    pushS(stack, &four);
    pushS(stack, &five);

    if (isFullS(stack))
    {
        puts("OK: Stack is full.");
    }
    else
    {
        puts("Eror: Stack must be full.");
    }

    for (int i = 1; i <= 5; i++)
    {
        printf("Poped element: %d\n", *(int *)(popS(stack)));
    }

    puts("Repopulating Stack...");
    pushS(stack, &one);
    pushS(stack, &two);
    pushS(stack, &three);
    pushS(stack, &four);
    pushS(stack, &five);

    puts("Processing Stack...");
    processS(stack, stackProcessor);

    puts("Elements post-processing:");
    for (int i = 1; i <= 5; i++)
    {
        printf("Poped element: %d\n", *(int *)(popS(stack)));
    }

    if (isEmptyS(stack))
    {
        puts("OK: Stack is empty.");
    }
    else
    {
        puts("Eror: Stack must be empty.");
    }
    freeS(stack);
    puts("#################    Test Done    #################");
}

