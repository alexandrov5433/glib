#include <stdlib.h>
#include "../include/data_structures/stack.h"

// ###############################     Stack     ###############################

Stack *newStack(const int capacity)
{
    if (capacity < 1)
    {
        return NULL;
    }

    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL)
    {
        return NULL;
    }

    void **arr = calloc(capacity, sizeof(void *));
    if (arr == NULL)
    {
        return NULL;
    }

    stack->arr = arr;
    stack->capacity = capacity;
    stack->currentIndex = -1;

    return stack;
}

int isFullS(const Stack *stack)
{
    if (stack == NULL)
    {
        return 2;
    }
    if (stack->currentIndex >= stack->capacity - 1)
    {
        return 1;
    }
    return 0;
}

int isEmptyS(const Stack *stack)
{
    if (stack == NULL)
    {
        return 2;
    }
    if (stack->currentIndex == -1)
    {
        return 1;
    }
    return 0;
}

void freeS(Stack *stack)
{
    if (stack == NULL)
    {
        return;
    }
    free(stack->arr);
    free(stack);
}

int pushS(Stack *stack, void *const ptr)
{
    if (stack == NULL || ptr == NULL)
    {
        return 2;
    }
    if (isFullS(stack) == 1)
    {
        return 1;
    }
    stack->arr[stack->currentIndex + 1] = ptr;
    (stack->currentIndex)++;
    return 0;
}

void *popS(Stack *stack)
{
    if (stack == NULL || isEmptyS(stack) == 1)
    {
        return NULL;
    }
    void *const element = stack->arr[stack->currentIndex];
    stack->arr[stack->currentIndex] = NULL;
    (stack->currentIndex)--;
    return element;
}

void *peekS(const Stack *stack)
{
    if (stack == NULL || isEmptyS(stack) == 1)
    {
        return NULL;
    }
    return stack->arr[stack->currentIndex];
}

int processS(const Stack *stack, void (*processor)(void *const ptr))
{
    if (stack == NULL)
    {
        return 1;
    }
    if (isEmptyS(stack) == 1)
    {
        return 0;
    }
    for (int i = 0; i <= stack->currentIndex; i++)
    {
        processor(stack->arr[i]);
    }
    return 0;
}

// ###############################      DynamicStack     ###############################

DynamicStack *newDynamicStack()
{
    DynamicStack *stack = (DynamicStack *)malloc(sizeof(DynamicStack));
    if (stack == NULL)
    {
        return NULL;
    }

    void **arr = calloc(STACK_DYNAMIC_INIT_CAPACITY, sizeof(void *));
    if (arr == NULL)
    {
        return NULL;
    }

    stack->arr = arr;
    stack->capacity = STACK_DYNAMIC_INIT_CAPACITY;
    stack->currentIndex = -1;

    return stack;
}

static int _extendDS(DynamicStack *stack)
{
    if (stack == NULL)
    {
        return 1;
    }

    if (stack->currentIndex >= stack->capacity - 1 - 10)
    {
        const int newCapacity = stack->capacity + STACK_DYNAMIC_INIT_CAPACITY;
        void **newArr = realloc(stack->arr, newCapacity * sizeof(void *));
        if (newArr == NULL)
        {
            return 2;
        }

        stack->arr = newArr;
        stack->capacity = newCapacity;
    }

    return 0;
}

static int _shrinkDS(DynamicStack *stack)
{
    if (stack == NULL)
    {
        return 1;
    }

    const int emptySpace = stack->capacity - 1 - stack->currentIndex;
    if (emptySpace >= STACK_DYNAMIC_INIT_CAPACITY * 1.5)
    {
        const int newCapacity = stack->capacity - STACK_DYNAMIC_INIT_CAPACITY;
        void **newArr = realloc(stack->arr, newCapacity * sizeof(void *));
        if (newArr == NULL)
        {
            return 2;
        }

        stack->arr = newArr;
        stack->capacity = newCapacity;
    }

    return 0;
}

int isEmptyDS(const DynamicStack *stack)
{
    if (stack == NULL)
    {
        return 2;
    }
    if (stack->currentIndex == -1)
    {
        return 1;
    }
    return 0;
}

void freeDS(DynamicStack *stack)
{
    if (stack == NULL)
    {
        return;
    }
    free(stack->arr);
    free(stack);
}

int pushDS(DynamicStack *stack, void *const ptr)
{
    if (ptr == NULL)
    {
        return 3;
    }

    const int extentionResult = _extendDS(stack); // NULL-check for stack is done inside
    if (extentionResult >= 1)
    {
        return extentionResult;
    }

    (stack->arr)[stack->currentIndex + 1] = ptr;
    (stack->currentIndex)++;

    return 0;
}

void *popDS(DynamicStack *stack)
{
    if (stack == NULL || isEmptyDS(stack))
    {
        return NULL;
    }

    void *const element = (stack->arr)[stack->currentIndex];
    (stack->arr)[stack->currentIndex] = NULL;
    (stack->currentIndex)--;

    _shrinkDS(stack);

    return element;
}

void *peekDS(const DynamicStack *stack)
{
    if (stack == NULL || isEmptyDS(stack))
    {
        return NULL;
    }
    return (stack->arr)[stack->currentIndex];
}

int processDS(const DynamicStack *stack, void (*processor)(void *const ptr))
{
    if (stack == NULL)
    {
        return 1;
    }
    if (isEmptyDS(stack))
    {
        return 0;
    }

    for (int i = 0; i <= stack->currentIndex; i++)
    {
        processor((stack->arr)[i]);
    }

    return 0;
}