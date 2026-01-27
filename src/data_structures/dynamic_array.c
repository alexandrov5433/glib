#include <stdlib.h>
#include <string.h>
#include "../include/data_structures/dynamic_array.h"

// ##################   static   ##################

static int _enumCheck(enum DynamicArrayType const type)
{
    switch (type)
    {
    case 0:
        // int
    case 1:
        // char
    case 2:
        // float
    case 3:
        // double
    case 4:
        // void*
        return 0;
    default:
        return 1;
    }
}

static int _memRealloc(DynamicArray *const da, size_t newCapacity)
{
    switch (da->type)
    {
    case 0:
        // int
        int *intArr = realloc(da->intArr, newCapacity * da->singleItemSize);
        if (intArr == NULL)
            return 2;
        da->intArr = intArr;
        break;
    case 1:
        // char
        char *charArr = realloc(da->charArr, newCapacity * da->singleItemSize);
        if (charArr == NULL)
            return 2;
        da->charArr = charArr;
        break;
    case 2:
        // float
        float *floatArr = realloc(da->floatArr, newCapacity * da->singleItemSize);
        if (floatArr == NULL)
            return 2;
        da->floatArr = floatArr;
        break;
    case 3:
        // double
        double *doubleArr = realloc(da->doubleArr, newCapacity * da->singleItemSize);
        if (doubleArr == NULL)
            return 2;
        da->doubleArr = doubleArr;
        break;
    case 4:
        // void*
        void **voidArr = realloc(da->voidArr, newCapacity * da->singleItemSize);
        if (voidArr == NULL)
            return 2;
        da->voidArr = voidArr;
        break;
    default:
        return 1;
    }
    da->capacity = newCapacity;
    return 0;
}

static int _expandDA(DynamicArray *const da)
{
    if (da == NULL)
        return 1;

    size_t freeSpace = da->capacity - da->count;
    if (freeSpace < DYNAMIC_ARRAY_MEMORY_INIT_CAPACITY * 0.25)
    {
        size_t newCapacity = da->capacity + DYNAMIC_ARRAY_MEMORY_INIT_CAPACITY;
        int err = _memRealloc(da, newCapacity);
        if (err)
            return err;
    }
    return 0;
}

static int _shrinkDA(DynamicArray *const da)
{
    if (da == NULL)
        return 1;

    size_t freeSpace = da->capacity - da->count;
    if (freeSpace > DYNAMIC_ARRAY_MEMORY_INIT_CAPACITY * 2)
    {
        size_t newCapacity = da->capacity - freeSpace * 0.5;

        // safe guard
        if (newCapacity <= da->count)
            newCapacity += da->count;

        int err = _memRealloc(da, newCapacity);
        if (err)
            return err;
    }
    return 0;
}

static int _moveRightOne(DynamicArray *const da)
{
    void **arr;
    switch (da->type)
    {
    case 0:
        // int
        arr = (void *)da->intArr;
        break;
    case 1:
        // char
        arr = (void *)da->charArr;
        break;
    case 2:
        // float
        arr = (void *)da->floatArr;
        break;
    case 3:
        // double
        arr = (void *)da->doubleArr;
        break;
    case 4:
        // void**
        arr = (void *)da->voidArr;
        break;
    default:
        return 1;
    }

    for (int i = da->count; i > 0; --i)
    {
        arr[i] = arr[i - 1];
    }

    return 0;
}

static int _moveLeftOne(DynamicArray *const da)
{
    void **arr;
    switch (da->type)
    {
    case 0:
        // int
        arr = (void *)da->intArr;
        break;
    case 1:
        // char
        arr = (void *)da->charArr;
        break;
    case 2:
        // float
        arr = (void *)da->floatArr;
        break;
    case 3:
        // double
        arr = (void *)da->doubleArr;
        break;
    case 4:
        // void**
        arr = (void *)da->voidArr;
        break;
    default:
        return 1;
    }

    for (int i = 0; i < da->count - 1; ++i)
    {
        arr[i] = arr[i + 1];
    }

    return 0;
}

static int _isEmpty(const DynamicArray *const da)
{
    return da->count <= 0 ? 1 : 0;
}

static int _isOutOfBounds(const DynamicArray *const da, size_t index)
{
    return index < 0 || index >= da->count ? 1 : 0;
}

static size_t _singleItemSize(enum DynamicArrayType type)
{
    switch (type)
    {
    case 0:
        // int
        return sizeof(int);
    case 1:
        // char
        return sizeof(char);
    case 2:
        // float
        return sizeof(float);
    case 3:
        // double
        return sizeof(double);
    case 4:
        // void*
        return sizeof(void *);
    default:
        return -1;
    }
}

static int _newItemsArray(DynamicArray *const da)
{
    switch (da->type)
    {
    case 0:
        // int
        int *intArr = malloc(da->capacity * da->singleItemSize);
        if (intArr == NULL)
            return 2;
        da->intArr = intArr;
        break;
    case 1:
        // char
        char *charArr = malloc(da->capacity * da->singleItemSize);
        if (charArr == NULL)
            return 2;
        da->charArr = charArr;
        break;
    case 2:
        // float
        float *floatArr = malloc(da->capacity * da->singleItemSize);
        if (floatArr == NULL)
            return 2;
        da->floatArr = floatArr;
        break;
    case 3:
        // double
        double *doubleArr = malloc(da->capacity * da->singleItemSize);
        if (doubleArr == NULL)
            return 2;
        da->doubleArr = doubleArr;
        break;
    case 4:
        // void**
        void **voidArr = malloc(da->capacity * da->singleItemSize);
        if (voidArr == NULL)
            return 2;
        da->voidArr = voidArr;
        break;
    default:
        return 1;
    }
    return 0;
}

static int _getPointerAtIndex(const DynamicArray *const da, const size_t index, void **output)
{
    switch (da->type)
    {
    case 0:
        // int
        *output = (void *)&((da->intArr)[index]);
        return 0;
    case 1:
        // char
        *output = (void *)&((da->charArr)[index]);
        return 0;
    case 2:
        // float
        *output = (void *)&((da->floatArr)[index]);
        return 0;
    case 3:
        // double
        *output = (void *)&((da->doubleArr)[index]);
        return 0;
    case 4:
        // void**
        *output = (void *)(da->voidArr)[index];
        return 0;
    }
    return 1;
}

// ##################   creation and destruction  ##################

DynamicArray *newDynamicArray(enum DynamicArrayType const type)
{
    if (_enumCheck(type))
        return NULL;

    DynamicArray *da = (DynamicArray *)malloc(sizeof(DynamicArray));
    if (da == NULL)
        return NULL;

    da->count = 0;
    da->capacity = DYNAMIC_ARRAY_MEMORY_INIT_CAPACITY;
    da->type = type;
    da->singleItemSize = _singleItemSize(type);

    int err = _newItemsArray(da);
    if (err)
    {
        free(da);
        return NULL;
    }

    return da;
}

void freeDynamicArray(DynamicArray *const da)
{
    if (da == NULL)
        return;
    switch (da->type)
    {
    case 0:
        // int
        free(da->intArr);
        break;
    case 1:
        // char
        free(da->charArr);
        break;
    case 2:
        // float
        free(da->floatArr);
        break;
    case 3:
        // double
        free(da->doubleArr);
        break;
    case 4:
        // void*
        free(da->voidArr);
        break;
    }
    free(da);
}

// ##################   add items   ##################

int pushDA(DynamicArray *const da, void *const item)
{
    if (item == NULL || da == NULL)
        return 1;

    int err = _expandDA(da);
    if (err)
        return err;

    switch (da->type)
    {
    case 0:
        // int
        (da->intArr)[(da->count)++] = *(int *)item;
        break;
    case 1:
        // char
        (da->charArr)[(da->count)++] = *(char *)item;
        break;
    case 2:
        // float
        (da->floatArr)[(da->count)++] = *(float *)item;
        break;
    case 3:
        // double
        (da->doubleArr)[(da->count)++] = *(double *)item;
        break;
    case 4:
        // void*
        (da->voidArr)[(da->count)++] = item;
        break;
    default:
        return 3;
    }

    return 0;
}

int unshiftDA(DynamicArray *const da, void *const item)
{
    if (item == NULL || da == NULL)
        return 1;

    int err = _expandDA(da);
    if (err)
        return err;

    err = _enumCheck(da->type);
    if (err)
        return 3;

    err = _moveRightOne(da);
    if (err)
        return 4;

    switch (da->type)
    {
    case 0:
        // int
        (da->intArr)[0] = *(int *)item;
        break;
    case 1:
        // char
        (da->charArr)[0] = *(char *)item;
        break;
    case 2:
        // float
        (da->floatArr)[0] = *(float *)item;
        break;
    case 3:
        // double
        (da->doubleArr)[0] = *(double *)item;
        break;
    case 4:
        // void*
        (da->voidArr)[0] = item;
        break;
    default:
        return 3;
    }

    (da->count)++;

    return 0;
}

// ##################   remove items   ##################

int popInt(DynamicArray *const da, int *const output)
{
    if (da == NULL || output == NULL || da->type != INT || _isEmpty(da))
    {
        return 1;
    }

    *output = (int)((da->intArr)[--(da->count)]);
    _shrinkDA(da);

    return 0;
}

int popChar(DynamicArray *const da, char *const output)
{
    if (da == NULL || output == NULL || da->type != CHAR || _isEmpty(da))
    {
        return 1;
    }

    *output = (char)((da->charArr)[--(da->count)]);
    _shrinkDA(da);

    return 0;
}

int popFloat(DynamicArray *const da, float *const output)
{
    if (da == NULL || output == NULL || da->type != FLOAT || _isEmpty(da))
    {
        return 1;
    }

    *output = (float)((da->floatArr)[--(da->count)]);
    _shrinkDA(da);

    return 0;
}

int popDouble(DynamicArray *const da, double *const output)
{
    if (da == NULL || output == NULL || da->type != DOUBLE || _isEmpty(da))
    {
        return 1;
    }

    *output = (double)((da->doubleArr)[--(da->count)]);
    _shrinkDA(da);

    return 0;
}

int popPointer(DynamicArray *const da, void **const output)
{
    if (da == NULL || output == NULL || da->type != VOID_PTR || _isEmpty(da))
    {
        return 1;
    }

    *output = (void *)((da->voidArr)[--(da->count)]);
    _shrinkDA(da);

    return 0;
}

int unshiftInt(DynamicArray *const da, int *const output)
{
    if (da == NULL || output == NULL || da->type != INT || _isEmpty(da))
    {
        return 1;
    }

    *output = (int)((da->intArr)[0]);
    _moveLeftOne(da);
    (da->count)--;
    _shrinkDA(da);

    return 0;
}

int unshiftChar(DynamicArray *const da, char *const output)
{
    if (da == NULL || output == NULL || da->type != CHAR || _isEmpty(da))
    {
        return 1;
    }

    *output = (char)((da->charArr)[0]);
    _moveLeftOne(da);
    (da->count)--;
    _shrinkDA(da);

    return 0;
}

int unshiftFloat(DynamicArray *const da, float *const output)
{
    if (da == NULL || output == NULL || da->type != FLOAT || _isEmpty(da))
    {
        return 1;
    }

    *output = (float)((da->floatArr)[0]);
    _moveLeftOne(da);
    (da->count)--;
    _shrinkDA(da);

    return 0;
}

int unshiftDouble(DynamicArray *const da, double *const output)
{
    if (da == NULL || output == NULL || da->type != DOUBLE || _isEmpty(da))
    {
        return 1;
    }

    *output = (double)((da->doubleArr)[0]);
    _moveLeftOne(da);
    (da->count)--;
    _shrinkDA(da);

    return 0;
}

int unshiftPointer(DynamicArray *const da, void **const output)
{
    if (da == NULL || output == NULL || da->type != VOID_PTR || _isEmpty(da))
    {
        return 1;
    }

    *output = (void *)((da->voidArr)[0]);
    _moveLeftOne(da);
    (da->count)--;
    _shrinkDA(da);

    return 0;
}

// ##################   processing   ##################

int processDA(DynamicArray *const da, void (*processor)(void *itemPtr, int *loopBreakTrigger))
{
    if (da == NULL || processor == NULL)
        return 1;

    void *ptr = (void *)malloc(sizeof(void *));
    int loopBreakTrigger = 0;
    for (size_t i = 0; i < da->count; ++i)
    {
        if (loopBreakTrigger == 1)
            break;

        int err = _getPointerAtIndex(da, i, &ptr);

        processor(ptr, &loopBreakTrigger);
    }
    free(ptr);
    return 0;
}

int filterDA(DynamicArray *const da, int (*filter)(void *itemPtr))
{
    if (da == NULL || filter == NULL)
        return 1;

    DynamicArray *tempDA = newDynamicArray(da->type);
    if (tempDA == NULL)
        return 1;

    void *ptr = (void *)malloc(sizeof(void *));

    for (size_t i = 0; i < da->count; ++i)
    {
        int err = _getPointerAtIndex(da, i, &ptr);

        if (filter(ptr) == 1)
            pushDA(ptr, tempDA);
    }

    free(ptr);

    /*
    Copy the data from the tempDA to the da.
    Memory of da::items must be freed, because it contains the old, unfiltered set.
    When copying the struct, the pointer to tempDa::items (pointing to the filtered set) is
    copied to the da. Now da::items points to the new set.
    tempDA is freed without freeing the tempDA::items pointer, as the pointer in da::items is the same.
    */

    // This (old) pointer will be overwritten by memcpy.
    switch (da->type)
    {
    case 0:
        // int
        free(da->intArr);
        break;
    case 1:
        // char
        free(da->charArr);
        break;
    case 2:
        // float
        free(da->floatArr);
        break;
    case 3:
        // double
        free(da->doubleArr);
        break;
    case 4:
        // void**
        free(da->voidArr);
        break;
    }
    memcpy(da, tempDA, sizeof(DynamicArray));
    free(tempDA); // The memory of tempDA::itemsArr is not freed. It is pointed to by da::itemsArr.

    return 0;
}

// ##################   searching   ##################

int atDA(DynamicArray *const da, size_t index, void **output)
{
    if (da == NULL || output == NULL || _isOutOfBounds(da, index))
        return 1;

    int err = _getPointerAtIndex(da, index, output);
    if (err)
        return 2;

    return 0;
}

int findDA(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr))
{
    if (da == NULL || output == NULL || selector == NULL)
        return 1;

    void *ptr = (void *)malloc(sizeof(void *));
    for (size_t i = 0; i < da->count; ++i)
    {
        int err = _getPointerAtIndex(da, i, &ptr);
        if (err)
        {
            free(ptr);
            return 2;
        }

        if (selector(ptr) == 1)
        {
            *output = ptr;
            return 0;
        }
    }
    free(ptr);
    return -1;
}

int findLastDA(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr))
{
    if (da == NULL || output == NULL || selector == NULL)
        return 1;

    void *ptr = (void *)malloc(sizeof(void *));
    for (size_t i = da->count - 1; i >= 0; --i)
    {
        int err = _getPointerAtIndex(da, i, &ptr);
        if (err)
        {
            free(ptr);
            return 2;
        }

        if (selector(ptr) == 1)
        {
            *output = ptr;
            return 0;
        }
    }
    free(ptr);
    return -1;
}

int findIndexDA(DynamicArray *const da, int *const output, int (*selector)(void *itemPtr))
{
    if (da == NULL || output == NULL || selector == NULL)
        return 1;

    void *ptr = (void *)malloc(sizeof(void *));
    for (size_t i = 0; i < da->count; ++i)
    {
        int err = _getPointerAtIndex(da, i, &ptr);
        if (err)
        {
            free(ptr);
            return 2;
        }

        if (selector(ptr) == 1)
        {
            *output = i;
            free(ptr);
            return 0;
        }
    }
    free(ptr);
    return -1;
}

int findLastIndexDA(DynamicArray *const da, int *const output, int (*selector)(void *itemPtr))
{
    if (da == NULL || output == NULL || selector == NULL)
        return 1;

    void *ptr = (void *)malloc(sizeof(void *));
    for (size_t i = da->count - 1; i >= 0; --i)
    {
        int err = _getPointerAtIndex(da, i, &ptr);
        if (err)
        {
            free(ptr);
            return 2;
        }

        if (selector(ptr) == 1)
        {
            *output = i;
            free(ptr);
            return 0;
        }
    }
    free(ptr);
    return -1;
}

int indexOfDA(DynamicArray *const da, int *const output, void *const value)
{
    if (da == NULL || output == NULL || value == NULL)
        return 1;

    for (size_t i = 0; i < da->count; ++i)
    {
        int isEqual = 0;
        switch (da->type)
        {
        case 0:
            // int
            isEqual = ((int)(da->intArr)[i]) == *(int *)value ? 1 : 0;
            break;
        case 1:
            // char
            isEqual = ((char)(da->charArr)[i]) == *(char *)value ? 1 : 0;
            break;
        case 2:
            // float
            isEqual = ((float)(da->floatArr)[i]) == *(float *)value ? 1 : 0;
            break;
        case 3:
            // double
            isEqual = ((double)(da->doubleArr)[i]) == *(double *)value ? 1 : 0;
            break;
        case 4:
            // void*
            isEqual = ((void *)(da->voidArr)[i]) == (void *)value ? 1 : 0;
            break;
        }

        if (isEqual)
        {
            *output = i;
            return 0;
        }
    }
    return -1;
}