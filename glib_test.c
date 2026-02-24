#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "include/data_structures/linkedList.h"
#include "include/data_structures/hash_map.h"
#include "include/sorting.h"
#include "include/data_structures/stack.h"
#include "include/data_structures/dynamic_array.h"

int firstSelector(void *arg)
{
    if (strcmp((char *)arg, "first") == 0)
    {
        return 1;
    }
    return 0;
}

int thirdSelector(void *arg)
{
    if (strcmp((char *)arg, "third") == 0)
    {
        return 1;
    }
    return 0;
}

int nonExistendSelector(void *arg)
{
    if (strcmp((char *)arg, "nonExistent") == 0)
    {
        return 1;
    }
    return 0;
}

void linkedListTest()
{
    char first[6] = "first";
    char second[7] = "second";
    char third[6] = "third";
    char fourth[7] = "fourth";

    LinkedList *ll = newLinkedList();
    appendToLL(&first, ll);
    appendToLL(&second, ll);
    appendToLL(&third, ll);
    appendToLL(&fourth, ll);

    Node *firstNode = findLL(firstSelector, ll);
    Node *thirdNode = findLL(thirdSelector, ll);
    Node *nonExistentNode = findLL(nonExistendSelector, ll);

    if (firstNode == NULL)
    {
        puts("Node not found: firstNode.");
    }
    else
    {
        printf("firstNode: %s\n", firstNode->value);
    }

    if (thirdNode == NULL)
    {
        puts("Node not found: thirdNode.");
    }
    else
    {
        printf("thirdNode: %s\n", thirdNode->value);
    }

    if (nonExistentNode == NULL)
    {
        puts("Node not found: nonExistentNode.");
    }
    else
    {
        printf("firstNode: %s\n", nonExistentNode->value);
    }

    Node *removedFirstNode = removeNodeLL(firstSelector, ll);
    if (removedFirstNode == NULL)
    {
        puts("Could not remove first node.");
    }
    else
    {
        printf("removedFirstNode: %s\n", removedFirstNode->value);
    }

    freeLinkedList(ll);
}

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

typedef struct Person
{
    char *name;
    int age;
} Person;

int qscComparator(const void *a, const void *b)
{
    return ((Person *)a)->age - ((Person *)b)->age;
}

void quickSortCompTest()
{
    Person p1 = {"Alice", 25};
    Person p2 = {"Bob", 42};
    Person p3 = {"Charlie", 30};
    Person p4 = {"Diana", 19};
    Person p5 = {"Eve", 35};
    Person p6 = {"Frank", 28};
    Person p7 = {"Grace", 50};
    Person p8 = {"Hank", 22};
    Person p9 = {"Ivy", 42};
    Person p10 = {"Jack", 27};

    Person *people[10] = {
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

void hashMapProcessor(Entry *const ptr)
{
    (*(int *)(ptr->value))++;
}
int hashMapFilter(Entry *const ptr)
{
    if (*(int *)(ptr->value) >= 50)
        return 0;

    return 1;
}
void hashMapTest()
{
    int VALUES = 100000;
    puts("##########################");
    puts("Testing: HashMap");
    HashMap *map = new_hash_map();
    if (!map)
    {
        puts("HashMap was not initialized.");
        puts("Test: Failed.");
        puts("##########################");
        return;
    }

    puts("Testing: put_hm");
    clock_t c11 = clock();
    char strBuff[10] = {0};
    for (int i = 0; i < VALUES; ++i)
    {
        memset(strBuff, 0, 10);
        snprintf(strBuff, 10, "%d", i);
        int *num = (int *)malloc(sizeof(int));
        *num = i;

        printf("\rFunction put_hm adding: %d", i);
        int err = put_hm(strBuff, num, map);
        if (err)
        {
            printf("Function put_hm returned with Error: %d\n", err);
            puts("Test: Failed.");
            puts("##########################");
            return;
        }
    }
    clock_t c12 = clock();
    double dur1 = 1000.0 * (c12 - c11) / CLOCKS_PER_SEC;
    printf("\nCPU time used from put_hm (per clock()): %.2f ms\n", dur1);
    printf("CPU time used from put_hm (per clock()) per value: %.6f ms\n", dur1 / VALUES);

    puts("Testing: get_hm");
    clock_t c21 = clock();
    for (int i = 0; i < VALUES; ++i)
    {
        memset(strBuff, 0, 10);
        snprintf(strBuff, 10, "%d", i);
        int *res = get_hm(strBuff, map);
        if (*res != i)
        {
            printf("\nFunction get_hm returned (%d), but expected (%d).\n", *res, i);
            puts("Test: Failed.");
            puts("##########################");
            return;
        }
        else
        {
            printf("\rFunction get_hm returned (%d), at index(%d).", *res, i);
        }
    }
    clock_t c22 = clock();
    double dur2 = 1000.0 * (c22 - c21) / CLOCKS_PER_SEC;
    printf("\nCPU time used from get_hm (per clock()): %.2f ms\n", dur2);
    printf("CPU time used from get_hm (per clock()) per value: %.6f ms\n", dur2 / VALUES);

    puts("Testing: process_hm");
    clock_t cp1 = clock();
    int processingErr = process_hm(hashMapProcessor, map);
    clock_t cp2 = clock();
    double durp = 1000.0 * (cp2 - cp1) / CLOCKS_PER_SEC;
    printf("CPU time used from process_hm (per clock()): %.2f ms\n", durp);
    printf("CPU time used from process_hm (per clock()) per value: %.6f ms\n", durp / VALUES);
    printf("Checking results of process_hm...");
    for (int i = 0; i < VALUES; ++i)
    {
        memset(strBuff, 0, 10);
        snprintf(strBuff, 10, "%d", i);
        int *res = get_hm(strBuff, map);
        if (*res != i + 1)
        {
            printf("\nFunction get_hm returned (%d), but expected (%d) after process_hm.\n", *res, i + 1);
            puts("Test: Failed.");
            puts("##########################");
            return;
        }
    }
    printf("done.\n");

    puts("Testing: remove_hm");
    clock_t c31 = clock();
    for (int i = 0; i < VALUES; ++i)
    {
        memset(strBuff, 0, 10);
        snprintf(strBuff, 10, "%d", i);
        int err = remove_hm(strBuff, map);
        if (err)
        {
            printf("\nFunction remove_hm returned with Error: %d\n", err);
            puts("Test: Failed.");
            puts("##########################");
            return;
        }
        printf("\rFunction remove_hm removed key: %s", strBuff);
    }
    clock_t c32 = clock();
    double dur3 = 1000.0 * (c32 - c31) / CLOCKS_PER_SEC;
    printf("\nCPU time used from remove_hm (per clock()): %.2f ms\n", dur3);
    printf("CPU time used from remove_hm (per clock()) per value: %.6f ms\n", dur3 / VALUES);

    if (map->elements != 0)
    {
        printf("HashMap elements are %d, but expected 0.\n", map->elements);
        puts("Test: Failed.");
        puts("##########################");
        return;
    }

    free_hash_map(map);

    // filter_hm test

    puts("Testing: filter_hm");
    HashMap *mapFilter = new_hash_map();
    if (!mapFilter)
    {
        puts("HashMap was not initialized for the test of filter_hm.");
        puts("Test: Failed.");
        puts("##########################");
        return;
    }
    for (int i = 0; i < VALUES; ++i)
    {
        memset(strBuff, 0, 10);
        snprintf(strBuff, 10, "%d", i);
        int *num = (int *)malloc(sizeof(int));
        *num = i;

        printf("\rFunction put_hm adding: %d", i);
        int err = put_hm(strBuff, num, mapFilter);
        if (err)
        {
            printf("\nFunction put_hm returned with Error: %d\n", err);
            puts("Test: Failed.");
            puts("##########################");
            return;
        }
    }
    puts("\nHashMap ready. Invoking filter_hm");
    clock_t cf1 = clock();
    int filterErr = filter_hm(hashMapFilter, mapFilter);
    if (filterErr)
    {
        printf("Function filter_hm returned with Error: %d\n", filterErr);
        puts("Test: Failed.");
        puts("##########################");
        return;
    }
    clock_t cf2 = clock();
    double durf = 1000.0 * (c32 - c31) / CLOCKS_PER_SEC;
    printf("CPU time used from filter_hm (per clock()): %.2f ms\n", durf);
    printf("CPU time used from filter_hm (per clock()) per value: %.6f ms\n", durf / VALUES);
    printf("Checking results of filter_hm...");
    for (int i = 0; i < fmin(50, VALUES); ++i)
    {
        memset(strBuff, 0, 10);
        snprintf(strBuff, 10, "%d", i);
        int *res = get_hm(strBuff, mapFilter);
        if (res == NULL)
        {
            printf("\nFunction get_hm returned NULL, but expected (%d) after filter_hm.\n", i + 1);
            puts("Test: Failed.");
            puts("##########################");
            return;
        }
        if (*res != i)
        {
            printf("\nFunction get_hm returned (%d), but expected (%d) after filter_hm.\n", *res, i + 1);
            puts("Test: Failed.");
            puts("##########################");
            return;
        }
    }
    printf("done.\n");
    free_hash_map(mapFilter);

    puts("Test: Success.");
    puts("##########################");
    return;
}

void stackProcessor(void *const ptr)
{
    *(int *)ptr = 0;
}
void stackTest()
{
    puts("#################    Test: Stack    #################");
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    int five = 5;
    Stack *stack = newStack(5);

    if (isEmptyS(stack))
    {
        puts("OK: Stack is empty.");
    }
    else
    {
        puts("Eror: Stack must be empty.");
    }

    pushS(stack, &one);
    pushS(stack, &two);
    pushS(stack, &three);
    pushS(stack, &four);
    pushS(stack, &five);

    if (isFullS(stack))
    {
        puts("OK: Stack is full.");
    }
    else
    {
        puts("Eror: Stack must be full.");
    }

    for (int i = 1; i <= 5; i++)
    {
        printf("Poped element: %d\n", *(int *)(popS(stack)));
    }

    puts("Repopulating Stack...");
    pushS(stack, &one);
    pushS(stack, &two);
    pushS(stack, &three);
    pushS(stack, &four);
    pushS(stack, &five);

    puts("Processing Stack...");
    processS(stack, stackProcessor);

    puts("Elements post-processing:");
    for (int i = 1; i <= 5; i++)
    {
        printf("Poped element: %d\n", *(int *)(popS(stack)));
    }

    if (isEmptyS(stack))
    {
        puts("OK: Stack is empty.");
    }
    else
    {
        puts("Eror: Stack must be empty.");
    }
    freeS(stack);
    puts("#################    Test Done    #################");
}

void printIntProcessor(void *const ptr)
{
    if (ptr == NULL)
    {
        printf("NULL ");
        return;
    }
    printf("%d ", *(int *)ptr);
}
int dynamicStackTest()
{
    printf("=== DynamicStack Tests ===\n\n");

    /* Create DynamicStack */
    DynamicStack *ds = newDynamicStack();
    if (!ds)
    {
        printf("FAIL: newDynamicStack returned NULL\n");
        return 1;
    }
    printf("PASS: newDynamicStack created\n");

    /* Test empty state */
    printf("isEmptyDS (expected 1): %d\n", isEmptyDS(ds));

    /* Push elements */
    const int NUM_ELEMENTS = 300;
    int *values[NUM_ELEMENTS];

    for (int i = 0; i < NUM_ELEMENTS; i++)
    {
        values[i] = malloc(sizeof(int));
        *values[i] = i * 10;

        int res = pushDS(ds, values[i]);
        if (res != 0)
        {
            printf("FAIL: pushDS failed at %d (code %d)\n", i, res);
            return 1;
        }
    }
    printf("PASS: pushDS pushed %d elements\n", NUM_ELEMENTS);

    /* Stack should not be empty */
    printf("isEmptyDS (expected 0): %d\n", isEmptyDS(ds));

    /* Peek test */
    int *peeked = (int *)peekDS(ds);
    if (!peeked || *peeked != (NUM_ELEMENTS - 1) * 10)
    {
        printf("FAIL: peekDS returned wrong value\n");
        return 1;
    }
    printf("PASS: peekDS returned %d\n", *peeked);

    /* Process stack */
    printf("processDS output (bottom -> top): ");
    processDS(ds, printIntProcessor);
    printf("\nPASS: processDS executed\n");

    /* Pop elements and test LIFO order */
    printf("Popping elements:\n");
    for (int i = NUM_ELEMENTS - 1; i >= 0; i--)
    {
        int *popped = (int *)popDS(ds);
        if (!popped || *popped != i * 10)
        {
            printf("FAIL: popDS expected %d, got %d\n",
                   i * 10,
                   popped ? *popped : -1);
            return 1;
        }
        printf("  popped %d\n", *popped);
        free(popped);
    }
    printf("PASS: popDS returned correct LIFO order\n");

    /* Stack should be empty again */
    printf("isEmptyDS (expected 1): %d\n", isEmptyDS(ds));

    /* Pop on empty stack */
    if (popDS(ds) != NULL)
    {
        printf("FAIL: popDS on empty stack should return NULL\n");
        return 1;
    }
    printf("PASS: popDS on empty stack returned NULL\n");

    /* Peek on empty stack */
    if (peekDS(ds) != NULL)
    {
        printf("FAIL: peekDS on empty stack should return NULL\n");
        return 1;
    }
    printf("PASS: peekDS on empty stack returned NULL\n");

    /* Cleanup */
    freeDS(ds);
    printf("PASS: freeDS executed\n");

    printf("\n=== ALL DynamicStack TESTS PASSED ===\n");
    return 0;
}

void intProcessorDA(void *ptr, int *loopBreaker)
{
    *(int *)ptr += 1;
}
int intFilterDA(void *ptr)
{
    return *(int *)ptr <= 5 ? 1 : 0;
}
void dynamicArrayTest()
{
    puts("####### Test: DynamicArray");

    printf("Testing: Initialization...");
    DynamicArray *da = new_dynamic_array(INT);
    if (!da)
    {
        printf("failed.\n");
        return;
    }
    printf("success.\n");

    printf("Testing function: push_da...");
    for (size_t i = 0; i < 5000; ++i)
    {
        int err = push_da(da, &i);
        if (err)
        {
            printf("failed with code: %d\n", err);
            return;
        }
    }
    printf("success.\n");

    printf("Testing function: popInt...");
    int *outputInt = (int *)malloc(sizeof(int));
    for (int i = 4999; i >= 0; --i)
    {

        int err = pop_da(da, outputInt);
        if (err)
        {
            printf("failed with code: %d\n", err);
            return;
        }
        if (*outputInt != i)
        {
            printf("failed. Expected output (%d), received (%d).\n", i, *outputInt);
            return;
        }
    }
    free(outputInt);
    printf("success.\n");

    printf("Testing array count:...");
    if (da->count != 0)
    {
        printf("failed. Expected (0), received (%d).\n", da->count);
        return;
    }
    printf("success.\n");

    printf("Testing function: unshift_da...");
    for (int i = 99; i >= 0; --i)
    {
        int err = unshift_da(da, &i);
        if (err)
        {
            printf("failed with code: %d\n", err);
            return;
        }
    }
    printf("success.\n");

    printf("Testing function: shiftInt...");
    int *o = (int *)malloc(sizeof(int));
    for (int i = 0; i <= 99; ++i)
    {

        int err = shift_da(da, o);
        if (err)
        {
            printf("failed with code: %d\n", err);
            return;
        }
        if (*o != i)
        {
            printf("failed. Expected output (%d), received (%d).\n", i, *o);
            return;
        }
    }
    free(o);
    printf("success.\n");

    printf("Testing array count:...");
    if (da->count != 0)
    {
        printf("failed. Expected (0), received (%d).\n", da->count);
        return;
    }
    printf("success.\n");

    printf("Testing function: processDA:...");
    for (size_t i = 0; i < 10; ++i)
    {
        int err = push_da(da, &i);
        if (err)
        {
            printf("failed, while fillig array, with code: %d\n", err);
            return;
        }
    }
    printf("array filled. Starting processor...");
    int err = process_da(da, intProcessorDA);
    if (err)
    {
        printf("failed with code: %d\n", err);
        return;
    }
    printf("processing done. Checking values...");
    int *intOut = NULL;
    for (size_t i = 0; i < 10; ++i)
    {
        int err = at_da(da, i, (void **)&intOut);
        if (err)
        {
            printf("failed, while getting value from array with atDA, with code: %d\n", err);
            return;
        }
        if (*intOut != i + 1)
        {
            printf("failed. Expected value (%d), received (%d).\n", i + 1, *intOut);
            return;
        }
    }
    printf("success.\n");

    printf("Testing function: filterDA:...");
    int filterErr = filter_da(da, intFilterDA);
    if (filterErr)
    {
        printf("failed with code: %d\n", err);
        return;
    }
    printf("filtering done. Checking array count...");
    if (da->count != 5)
    {
        printf("failed. Expected array count (5), received (%d).\n", da->count);
        return;
    }
    printf("done. Checking values...");
    for (int i = 1; i <= 5; ++i)
    {
        int err = at_da(da, i - 1, (void **)&intOut);
        if (err)
        {
            printf("failed, while getting value from array with atDA, with code: %d\n", err);
            return;
        }
        if (*intOut != i)
        {
            printf("failed. Expected (%d) at index (%d), received (%d).\n", i, i - 1, *intOut);
            return;
        }
    }
    printf("success.\n");
    free_dynamic_array(da);

    printf("Testing void pointer array...");
    DynamicArray *vpDa = new_dynamic_array(VOID_PTR);

    Person p1 = {"Alice", 25};
    Person p2 = {"Bob", 42};
    Person p3 = {"Charlie", 30};
    Person p4 = {"Diana", 19};
    Person p5 = {"Eve", 35};
    Person p6 = {"Frank", 28};
    Person p7 = {"Grace", 50};
    Person p8 = {"Hank", 22};
    Person p9 = {"Ivy", 42};
    Person p10 = {"Jack", 27};

    printf("populating...");
    int errArr[10] = {
        push_da(vpDa, &p1),
        push_da(vpDa, &p2),
        push_da(vpDa, &p3),
        push_da(vpDa, &p4),
        push_da(vpDa, &p5),
        push_da(vpDa, &p6),
        push_da(vpDa, &p7),
        push_da(vpDa, &p8),
        push_da(vpDa, &p9),
        push_da(vpDa, &p10),
    };

    for (size_t i = 0; i < 10; ++i)
    {
        if (errArr[i])
        {
            printf("failed. One or more push_da calls returned more than 0.\n");
            return;
        }
    }
    Person *person_five = NULL;
    int pf_err = at_da(vpDa, 4, (void **)&person_five);
    if (!person_five)
    {
        printf("failed. Person not found in array\n");
        return;
    }
    if (!person_five || person_five->age != 35)
    {
        printf("failed. Expected age (35), received (%d).\n", person_five->age);
        return;
    }
    printf("success.\n");

    free_dynamic_array(vpDa);

    printf("Testing function: popDA for VOID_PTR...");
    DynamicArray *daPopTest = new_dynamic_array(VOID_PTR);
    push_da(daPopTest, &p1);
    push_da(daPopTest, &p2);

    Person *person = NULL;
    int daPopErr = pop_da(daPopTest, (void *)&person);
    if (daPopErr)
    {
        printf("failed. pop_da reuturned error (%d).\n", daPopErr);
        return;
    }
    if (person->age == 42)
    {
        printf("success.\n");
    }
    else
    {
        printf("failed. Expected age (42), received (%d).\n", person->age);
        return;
    }
    free_dynamic_array(daPopTest);

    printf("Testing function: pop_da for INT...");
    DynamicArray *daPopInt = new_dynamic_array(INT);
    for (int i = 1; i < 4; ++i)
    {
        push_da(daPopInt, &i);
    }
    int popIntOutput = 0;
    int popIntErr = pop_da(daPopInt, &popIntOutput);
    if (popIntErr)
    {
        printf("failed. popDA reuturned error (%d).\n", popIntErr);
        return;
    }
    if (popIntOutput == 3)
    {
        printf("success.\n");
    }
    else
    {
        printf("failed. Expected (3), received (%d).\n", popIntOutput);
        return;
    }
    free_dynamic_array(daPopInt);

    printf("Testing function: shift_da for VOID_PTR...");
    DynamicArray *daShiftVoid = new_dynamic_array(VOID_PTR);
    push_da(daShiftVoid, &p1);
    push_da(daShiftVoid, &p2);

    Person *shiftPerson = NULL;
    int daShiftErr = shift_da(daShiftVoid, (void *)&person);
    if (daShiftErr)
    {
        printf("failed. shift_da reuturned error (%d).\n", daShiftErr);
        return;
    }
    if (person->age == 25)
    {
        printf("success.\n");
    }
    else
    {
        printf("failed. Expected age (25), received (%d).\n", person->age);
        return;
    }
    free_dynamic_array(daShiftVoid);

    printf("Testing function: shift_da for INT...");
    DynamicArray *daShiftInt = new_dynamic_array(INT);
    for (int i = 1; i < 4; ++i)
    {
        push_da(daPopInt, &i);
    }
    int shiftIntOutput = 0;
    int shiftIntErr = shift_da(daShiftInt, &shiftIntOutput);
    if (shiftIntErr)
    {
        printf("failed. popDA reuturned error (%d).\n", shiftIntErr);
        return;
    }
    if (shiftIntOutput == 1)
    {
        printf("success.\n");
    }
    else
    {
        printf("failed. Expected (1), received (%d).\n", shiftIntOutput);
        return;
    }
    free_dynamic_array(daShiftInt);

    puts("####### Test done.");
}
int main()
{
    clock_t c1 = clock();

    hashMapTest();
    // linkedListTest();
    // quickSortTest();
    // quickSortCompTest();
    // bubbleSortTest();
    // bubbleSortCompTest();
    // stackTest();
    // dynamicStackTest();
    // dynamicArrayTest();
    clock_t c2 = clock();
    double dur = 1000.0 * (c2 - c1) / CLOCKS_PER_SEC;
    printf("CPU time used (per clock()): %.2f ms\n", dur);
    return EXIT_SUCCESS;
}