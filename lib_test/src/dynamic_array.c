#include <stdio.h>
#include <stdlib.h>
#include "../../include/dynamic_array.h"
#include "../../include/gstring.h"
#include "../include/dynamic_array.h"

void intProcessorDA(void *ptr)
{
    *(int *)ptr += 1;
}
int intFilterDA(void *ptr)
{
    return *(int *)ptr <= 5 ? 1 : 0;
}
void dynamicArrayTest()
{
    puts("################## Test: DynamicArray ##################");

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

    PersonDA p1 = {"Alice", 25};
    PersonDA p2 = {"Bob", 42};
    PersonDA p3 = {"Charlie", 30};
    PersonDA p4 = {"Diana", 19};
    PersonDA p5 = {"Eve", 35};
    PersonDA p6 = {"Frank", 28};
    PersonDA p7 = {"Grace", 50};
    PersonDA p8 = {"Hank", 22};
    PersonDA p9 = {"Ivy", 42};
    PersonDA p10 = {"Jack", 27};

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
    PersonDA *person_five = NULL;
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

    PersonDA *person = NULL;
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

    PersonDA *shiftPerson = NULL;
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

    puts(ANSI_COLOR_GREEN "Result: Success" ANSI_COLOR_RESET);
    puts("################## Test: DynamicArray ##################");
}
