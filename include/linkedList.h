#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

// _WIN32 is a predefined compiler macro when compiling for Windows.
// Must NOT be defined manually! More notes in CMakeLists.txt
#ifdef _WIN32

#ifdef GALXLIB_SHARED

#ifdef GALXLIB_EXPORTS
#define GALXLIB_API __declspec(dllexport)
#else
#define GALXLIB_API __declspec(dllimport)
#endif

#else
#define GALXLIB_API
#endif

#endif

typedef struct Node
{
    void *value;
    struct Node *next;
} Node;

typedef struct LinkedList
{
    struct Node *start;
    struct Node *end;
    size_t nodesCount;
} LinkedList;

GALXLIB_API LinkedList *newLinkedList();

GALXLIB_API void freeNodeLL(Node *node);

GALXLIB_API void freeLinkedList(LinkedList *ll);

GALXLIB_API int appendToLL(void *value, LinkedList *ll);

GALXLIB_API int prependToLL(void *value, LinkedList *ll);

GALXLIB_API int incertBeforeLL(void *newValue, int (*selector)(void *value), LinkedList *ll);

GALXLIB_API int incertAfterLL(void *newValue, void *after, LinkedList *ll);

GALXLIB_API Node *findLL(int (*selector)(void *value), LinkedList *ll);

GALXLIB_API Node *removeNodeLL(int (*selector)(void *value), LinkedList *ll);

#endif