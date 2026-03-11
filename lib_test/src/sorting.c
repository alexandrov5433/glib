#include <stdio.h>
#include <stdlib.h>
#include "../../include/sorting.h"
#include "../include/sorting.h"

void printIntArray(int *nums, int length)
{
    printf("[ ");
    for (int i = 0; i < length; i++)
    {
        printf("%d, ", nums[i]);
    }
    printf(" ]\n");
}

void quickSortTest()
{
    int nums[11] = {5, 2, 4, 4, 7, 123, 9, 0, 36, 22, 7};
    int numbers[7] = {0, 1, 2, 3, 4, 5, 6};
    int numbersOne[7] = {0, 0, 2, 0, 0, 0, 6};
    int numbersTwo[7] = {1, 1, 2, 0, 0, 1, 6};
    quickSort(nums, 11);
    quickSort(numbers, 7);
    quickSort(numbersOne, 7);
    quickSort(numbersTwo, 7);
    printIntArray(nums, 11);
    printIntArray(numbers, 7);
    printIntArray(numbersOne, 7);
    printIntArray(numbersTwo, 7);
}

int qscComparator(const void *a, const void *b)
{
    return ((PersonSorting *)a)->age - ((PersonSorting *)b)->age;
}

void quickSortCompTest()
{
    PersonSorting p1 = {"Alice", 25};
    PersonSorting p2 = {"Bob", 42};
    PersonSorting p3 = {"Charlie", 30};
    PersonSorting p4 = {"Diana", 19};
    PersonSorting p5 = {"Eve", 35};
    PersonSorting p6 = {"Frank", 28};
    PersonSorting p7 = {"Grace", 50};
    PersonSorting p8 = {"Hank", 22};
    PersonSorting p9 = {"Ivy", 42};
    PersonSorting p10 = {"Jack", 27};

    PersonSorting *people[10] = {
        &p1, &p2, &p3, &p4, &p5,
        &p6, &p7, &p8, &p9, &p10};

    quickSortComp((void **)people, 10, qscComparator);

    for (int i = 0; i < 10; i++)
    {
        printf("%s (%d)\n", people[i]->name, people[i]->age);
    }
}

void bubbleSortTest()
{
    int nums[11] = {5, 2, 4, 4, 7, 123, 9, 0, 36, 22, 7};
    int otherNums[7] = {64, 34, 25, 12, 22, 11, 90};
    bubbleSort(nums, 11);
    bubbleSort(otherNums, 7);
    printIntArray(nums, 11);
    printIntArray(otherNums, 7);
}

int comparator(void *x, void *y)
{
    if (*((int *)x) > *((int *)y))
        return 1;
    return 0;
}
void bubbleSortCompTest()
{
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    int five = 5;
    int *nums[5] = {&four, &one, &two, &three, &five};
    bubbleSortComp((void **)nums, 5, &comparator);
    printf("[ ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d, ", *(nums[i]));
    }
    printf(" ]\n");
}
