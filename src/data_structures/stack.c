#include <stdlib.h>
#include "../include/data_structures/stack.h"

Stack *newStack(int capacity)
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

int pushS(Stack *stack, const void *ptr)
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
    void *element = stack->arr[stack->currentIndex];
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

int processS(const Stack *stack, void (*processor)(void *ptr))
{
    if (stack == NULL)
    {
        return 1;
    }
    if (isEmptyS(stack) == 1)
    {
        return;
    }
    for (int i = 0; i <= stack->currentIndex; i++)
    {
        processor(stack->arr[i]);
    }
    return 0;
}