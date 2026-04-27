#ifndef QUEUE_H
#define QUEUE_H

#include "../include/linkedList.h"

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

typedef struct Queue {
    struct LinkedList *list;
} Queue;

#endif