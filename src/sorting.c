#include <stdlib.h>
#include <math.h>
#include "../include/sorting.h"

static void swap(int *nums, int x, int y)
{
    int temp = nums[x];
    nums[x] = nums[y];
    nums[y] = temp;
}

static void swapPointer(void **array, int x, int y)
{
    void *temp = array[x];
    array[x] = array[y];
    array[y] = temp;
}

static void pushLeft(int *nums, size_t length, int targetIndex)
{
    for (int i = targetIndex; i > 0; i--)
    {
        swap(nums, i, i - 1);
    }
}

static void pushRight(int *nums, size_t length, int targetIndex)
{
    for (int i = targetIndex; i < length - 1; i++)
    {
        swap(nums, i, i + 1);
    }
}

static void _quickSort(int *nums, int startIndex, int endIndex)
{
    if (startIndex >= endIndex)
    {
        // case: only 1 element in the array; sorting done;
        return;
    }
    const int pivot = nums[endIndex];
    int i = startIndex - 1;
    for (int j = startIndex; j < endIndex; j++)
    {
        if (nums[j] < pivot)
        {
            i++;
            swap(nums, i, j);
        }
    }
    swap(nums, i + 1, endIndex);
    if (startIndex == endIndex - 1 && nums[startIndex] <= nums[endIndex])
    {
        return;
    }
    _quickSort(nums, startIndex, i);
    if (nums[i + 1] == nums[endIndex])
    {
        _quickSort(nums, i + 2, endIndex);
        return;
    }
    _quickSort(nums, i + 1, endIndex);
}

void quickSort(int *nums, size_t length)
{
    _quickSort(nums, 0, length - 1);
}

static void _quickSortComp(void **elements, int startIndex, int endIndex, int (*comparator)(const void *a, const void *b))
{
    if (startIndex >= endIndex)
    {
        return;
    }
    const void *pivot = elements[endIndex];
    int i = startIndex - 1;
    for (int j = startIndex; j < endIndex; j++)
    {
        if (comparator(elements[j], pivot) < 0)
        {
            i++;
            swapPointer(elements, i, j);
        }
    }
    swapPointer(elements, i + 1, endIndex);
    if (startIndex == endIndex - 1 && comparator(elements[startIndex], elements[endIndex]) <= 0)
    {
        return;
    }
    _quickSortComp(elements, startIndex, i, comparator);
    if (comparator(elements[i + 1], elements[endIndex]) == 0)
    {
        _quickSortComp(elements, i + 2, endIndex, comparator);
        return;
    }
    _quickSortComp(elements, i + 1, endIndex, comparator);
}

/**
 * @param comparator (<0) a comes BEFORE b. (0) a and b are EQUAL. (>0) a comes AFTER b. 
 */
void quickSortComp(void **elements, size_t length, int (*comparator)(const void *a, const void *b))
{
    _quickSortComp(elements, 0, length - 1, comparator);
}

void bubbleSort(int *nums, size_t length)
{
    for (int i = 0; i < length - 1; i++)
    {
        for (int j = 1; j < length; j++)
        {
            int x = nums[j - 1];
            int y = nums[j];
            if (x > y)
            {
                swap(nums, j - 1, j);
            }
        }
    }
}

void bubbleSortComp(void **array, size_t length, int (*comparator)(void *a, void *b))
{
    for (int i = 0; i < length - 1; i++)
    {
        for (int j = 1; j < length; j++)
        {
            void *x = array[j - 1];
            void *y = array[j];
            if (comparator(x, y) == 1)
            {
                swapPointer(array, j - 1, j);
            }
        }
    }
}