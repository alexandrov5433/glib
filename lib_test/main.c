#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "./include/dynamic_array.h"
#include "./include/gstring.h"
#include "./include/hash_map.h"
#include "./include/linked_list.h"
#include "./include/sorting.h"
#include "./include/stack.h"
#include "./include/gregex.h"

int main()
{
    clock_t c1 = clock();

    stringTest();
    hashMapTest();
    // linkedListTest();
    // quickSortTest();
    // quickSortCompTest();
    // bubbleSortTest();
    // bubbleSortCompTest();
    // stackTest();
    // dynamicStackTest();
    dynamicArrayTest();
    test_gregex();
    clock_t c2 = clock();
    double dur = 1000.0 * (c2 - c1) / CLOCKS_PER_SEC;
    printf("CPU time used (per clock()): %.2f ms\n", dur);
    return EXIT_SUCCESS;
}