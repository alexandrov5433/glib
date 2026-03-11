#ifndef TEST_SORTING
#define TEST_SORTING

typedef struct PersonSorting
{
    char *name;
    int age;
} PersonSorting;

void printIntArray(int *nums, int length);
void quickSortTest();
int qscComparator(const void *a, const void *b);
void quickSortCompTest();
void bubbleSortTest();
int comparator(void *x, void *y);
void bubbleSortCompTest();

#endif