#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// _WIN32 is a predefined compiler macro when compiling for Windows.
// Must NOT be defined manually! More notes in CMakeLists.txt
#ifdef _WIN32
#ifdef GLIB_EXPORTS
#define GLIB_API __declspec(dllexport)
#else
#define GLIB_API __declspec(dllimport)
#endif
#else
#define GLIB_API
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

GLIB_API LinkedList *newLinkedList();

GLIB_API void freeNodeLL(Node *node);

GLIB_API void freeLinkedList(LinkedList *ll);

GLIB_API int appendToLL(void *value, LinkedList *ll);

GLIB_API int prependToLL(void *value, LinkedList *ll);

GLIB_API int incertBeforeLL(void *newValue, int (*selector)(void *value), LinkedList *ll);

GLIB_API int incertAfterLL(void *newValue, void *after, LinkedList *ll);

GLIB_API Node *findLL(int (*selector)(void *value), LinkedList *ll);

GLIB_API Node *removeNodeLL(int (*selector)(void *value), LinkedList *ll);

#endif