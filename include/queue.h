#ifndef QUEUE_H
#define QUEUE_H

#include "../include/linkedList.h"

#ifdef _WIN32
#ifdef GALXLIB_EXPORTS
#define GLIB_API __declspec(dllexport)
#else
#define GLIB_API __declspec(dllimport)
#endif
#else
#define GLIB_API
#endif

typedef struct Queue {
    struct LinkedList *list;
} Queue;

#endif