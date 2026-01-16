#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#ifdef _WIN32
#ifdef GLIB_EXPORTS
#define GLIB_API __declspec(dllexport)
#else
#define GLIB_API __declspec(dllimport)
#endif
#else
#define GLIB_API
#endif

typedef struct Stack
{
    void **arr;
    int capacity;
    int currentIndex;
} Stack;

typedef struct DynamicStack
{
    void **arr;
    int currentIndex;
} DynamicStack;

/**
 * Creates a new Stack with a fixed capacity.
 * @param capacity An integer, bigger than 0, which indicates the capacity. If capacity is 0 or less, NULL is returned.
 * @returns The pointer to the newly created Stack. NULL is returned on memory allocation failure.
 */
GLIB_API Stack *newStack(int capacity);

/**
 * Checks if the Stack is full.
 * @param stack A pointer to the Stack. If NULL, 2 is returned.
 * @returns 0 if there is free space (one or more), and 1 otherwise.
 */
GLIB_API int isFullS(const Stack *stack);

/**
 * Checks if the Stack is empty.
 * @param stack A pointer to the Stack. If NULL, 2 is returned.
 * @returns 1 if the Stack is empty, and 0 otherwise.
 */
GLIB_API int isEmptyS(const Stack *stack);

/**
 * Adds a new element on top of the Stack.
 * @param stack A pointer ot the Stack, to which the new element must be added. If NULL, 2 is returned.
 * @param ptr The pointer, which will be added as a new element on top of the Stack, If NULL, 2 is returned.
 * @returns 0 on success; 1 if the Stack is full (isFullS function).
 */
GLIB_API int pushS(Stack *stack, const void *ptr);

/**
 * Retrieves the topmost element of the Stack and removes it from the Stack.
 * @param stack A pointer ot the Stack, from which the top element must be retrieved. If NULL, NULL is returned.
 * @returns The pointer, which was the topmost element. If the Stack is empty (isEmptyS function), returns NULL.
 */
GLIB_API void *popS(Stack *stack);

/**
 * Returns the topmost element of the Stack, without modifying the Stack.
 * @param stack A pointer ot the Stack, from which the top element must be retrieved. If NULL, NULL is returned.
 * @returns The pointer, which was the topmost element. If the Stack is empty (isEmptyS function), returns NULL.
 */
GLIB_API void *peekS(const Stack *stack);

/**
 * Applies a processor function to every element of the Stack, from bottom to top.
 * @param stack A pointer ot Stack, the elements of which must be processed. If NULL, 1 is returned. 
 * If the Stack is empty, nothing is done.
 * @param processor The function, which will be applied to each element.
 * @returns 0 on success.
 */
GLIB_API int processS(const Stack *stack, void (*processor)(void *ptr));

#endif