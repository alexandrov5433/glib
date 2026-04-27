#ifndef SORTING_H
#define SORTING_H

#include <stdlib.h>
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

GALXLIB_API void quickSort(int *nums, size_t length);

GALXLIB_API void quickSortComp(void **elements, size_t length, int (*comparator)(const void *a, const void *b));

GALXLIB_API void bubbleSort(int *nums, size_t length);

GALXLIB_API void bubbleSortComp(void **array, size_t length, int (*comparator)(void *a, void *b));

#endif