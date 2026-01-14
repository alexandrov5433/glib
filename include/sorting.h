#include <stdlib.h>
#ifndef SORTING_H
#define SORTING_H

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

GLIB_API void quickSort(int *nums, size_t length);

GLIB_API void quickSortComp(void **elements, size_t length, int (*comparator)(const void *a, const void *b));

GLIB_API void bubbleSort(int *nums, size_t length);

GLIB_API void bubbleSortComp(void **array, size_t length, int (*comparator)(void *a, void *b));

#endif