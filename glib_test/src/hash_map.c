#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../../include/data_structures/hash_map.h"
#include "../include/hash_map.h"
#include "../../include/gstring.h"


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
void hashMapValueDestructor(const Entry *const entry)
{
    free(entry->value);
}
void hashMapTest()
{
    puts("################## Test: HashMap ##################");
    int VALUES = 10000;
    HashMap *map = new_hash_map();
    if (!map)
    {
        puts("HashMap was not initialized.");
        goto _test_failure;
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
            goto _test_failure;
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
            goto _test_failure;
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
            goto _test_failure;
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
            goto _test_failure;
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
        goto _test_failure;
    }

    free_hash_map(map);

    // filter_hm test

    puts("Testing: filter_hm");
    HashMap *mapFilter = new_hash_map();
    if (!mapFilter)
    {
        puts("HashMap was not initialized for the test of filter_hm.");
        goto _test_failure;
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
            goto _test_failure;
        }
    }
    puts("\nHashMap ready. Invoking filter_hm");
    clock_t cf1 = clock();
    int filterErr = filter_hm(hashMapFilter, mapFilter);
    if (filterErr)
    {
        printf("Function filter_hm returned with Error: %d\n", filterErr);
        goto _test_failure;
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
            goto _test_failure;
        }
        if (*res != i)
        {
            printf("\nFunction get_hm returned (%d), but expected (%d) after filter_hm.\n", *res, i + 1);
            goto _test_failure;
        }
    }
    printf("done.\n");
    free_hash_map(mapFilter);

    puts("Testing: put action for identicle keys. Value must be replaced.");
    HashMap *identicleHM = new_hash_map();
    for (int i = 0; i < 3; ++i)
    {
        memset(strBuff, 0, 10);
        snprintf(strBuff, 10, "%d", i);
        int *num = (int *)malloc(sizeof(int));
        *num = i;

        printf("\rFunction put_hm adding: %d", i);
        int err = put_hm(strBuff, num, identicleHM);
        if (err)
        {
            printf("Function put_hm returned with Error: %d\n", err);
            goto _test_failure;
        }
    }
    for (int i = 0; i < 3; ++i)
    {
        memset(strBuff, 0, 10);
        snprintf(strBuff, 10, "%d", i);
        int *num = (int *)malloc(sizeof(int));
        *num = i * 10;

        printf("\rFunction put_hm adding: %d", i);
        int err = put_hm(strBuff, num, identicleHM);
        if (err)
        {
            printf("Function put_hm returned with Error: %d\n", err);
            goto _test_failure;
        }
    }
    printf("\nChecking results for putting identicle keys (value replacement)...");
    for (int i = 0; i < 3; ++i)
    {
        memset(strBuff, 0, 10);
        snprintf(strBuff, 10, "%d", i);
        int *res = get_hm(strBuff, identicleHM);
        if (*res != i * 10)
        {
            printf("\nFunction get_hm returned (%d), but expected (%d).\n", *res, i * 10);
            goto _test_failure;
        }
    }
    if (identicleHM->elements != 3)
    {
        printf("\nIncorrect elements count of %d in identicleHM. Expected 3.\n", identicleHM->elements);
        goto _test_failure;
    }
    free_hash_map(identicleHM);
    printf("done.\n");

    puts("Testing: value_destructor.");
    HashMap *destructorHM = new_hash_map();
    int destAddErr = add_destructor_hm(hashMapValueDestructor, destructorHM);
    if (destAddErr)
    {
        printf("Failed to add value_destructor to HashMap. add_destructor_hm returned %d\n", destAddErr);
        goto _test_failure;
    }
    for (int i = 0; i < VALUES; ++i)
    {
        memset(strBuff, 0, 10);
        snprintf(strBuff, 10, "%d", i);
        int *num = (int *)malloc(sizeof(int));
        *num = i;

        int err = put_hm(strBuff, num, destructorHM);
        if (err)
        {
            printf("Function put_hm returned with Error: %d\n", err);
            goto _test_failure;
        }
    }
    int errDestRemove = remove_hm("0", destructorHM);
    if (errDestRemove)
    {
        printf("Failed to remove value fromm HashMap. remove_hm returned %d\n", errDestRemove);
        goto _test_failure;
    }
    int filterDestErr = filter_hm(hashMapFilter, destructorHM);
    if (filterDestErr)
    {
        printf("Function filter_hm returned with Error: %d\n", filterDestErr);
        goto _test_failure;
    }
    if (destructorHM->elements != 49)
    {
        printf("Incorrect elements count after filtration. Expected 49, received %d\n", destructorHM->elements);
        goto _test_failure;
    }
    free_hash_map(destructorHM);

    puts(ANSI_COLOR_GREEN "Result: Success" ANSI_COLOR_RESET);
    puts("################## Test: HashMap ##################");
    return;
_test_failure:
    puts(ANSI_COLOR_RED "Result: Failure" ANSI_COLOR_RESET);
    puts("################## Test: HashMap ##################");
}
