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
    if (nums[i + 1] == nums[endIndex]) {
        _quickSort(nums, i + 2, endIndex);
        return;
    }
    _quickSort(nums, i + 1, endIndex);
}

void quickSort(int *nums, size_t length)
{
    _quickSort(nums, 0, length - 1);
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

void bubbleSortComp(void **array, size_t length, int (*comparator)(void *first, void *second))
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