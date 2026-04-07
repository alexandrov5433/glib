#include <stdlib.h>
#include <string.h>
#include "../include/dynamic_array.h"

// ##################   static   ##################

static inline int _enum_check(enum DynamicArrayType const type)
{
    switch (type)
    {
    case 0:
        // int
        return 0;
    case 1:
        // char
        return 0;
    case 2:
        // float
        return 0;
    case 3:
        // double
        return 0;
    case 4:
        // void*
        return 0;
    default:
        return 3;
    }
}

static int _mem_realloc(DynamicArray *const da, size_t newCapacity)
{
    if (da == NULL)
        return 1;

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
        return 3;
    }
    da->capacity = newCapacity;
    return 0;
}

static int _expand_da(DynamicArray *const da)
{
    if (da == NULL)
        return 1;

    size_t freeSpace = da->capacity - da->count;
    if (freeSpace < DYNAMIC_ARRAY_INIT_CAPACITY * 0.25)
    {
        size_t newCapacity = da->capacity + DYNAMIC_ARRAY_INIT_CAPACITY;
        int err = _mem_realloc(da, newCapacity); // new_capcity is set in _mem_realloc
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
    if (freeSpace > DYNAMIC_ARRAY_INIT_CAPACITY * 2)
    {
        size_t newCapacity = da->capacity - freeSpace * 0.5;

        // safe guard
        if (newCapacity <= da->count)
            newCapacity += da->count;

        int err = _mem_realloc(da, newCapacity); // new_capcity is set in _mem_realloc
        if (err)
            return err;
    }
    return 0;
}

static int _move_one_right(DynamicArray *const da)
{
    switch (da->type)
    {
    case 0:
        // int
        for (size_t i = da->count; i > 0; --i)
            da->intArr[i] = da->intArr[i - 1];
        break;
    case 1:
        // char
        for (size_t i = da->count; i > 0; --i)
            da->charArr[i] = da->charArr[i - 1];
        break;
    case 2:
        // float
        for (size_t i = da->count; i > 0; --i)
            da->floatArr[i] = da->floatArr[i - 1];
        break;
    case 3:
        // double
        for (size_t i = da->count; i > 0; --i)
            da->doubleArr[i] = da->doubleArr[i - 1];
        break;
    case 4:
        // void**
        for (size_t i = da->count; i > 0; --i)
            da->voidArr[i] = da->voidArr[i - 1];
        break;
    default:
        return 1;
    }

    return 0;
}

static int _move_one_left(DynamicArray *const da)
{
    switch (da->type)
    {
    case 0:
        // int
        for (size_t i = 0; i < da->count - 1; ++i)
        {
            da->intArr[i] = da->intArr[i + 1];
        }
        break;
    case 1:
        // char
        for (size_t i = 0; i < da->count - 1; ++i)
        {
            da->charArr[i] = da->charArr[i - 1];
        }
        break;
    case 2:
        // float
        for (size_t i = 0; i < da->count - 1; ++i)
        {
            da->floatArr[i] = da->floatArr[i - 1];
        }
        break;
    case 3:
        // double
        for (size_t i = 0; i < da->count - 1; ++i)
        {
            da->doubleArr[i] = da->doubleArr[i - 1];
        }
        break;
    case 4:
        // void**
        for (size_t i = 0; i < da->count - 1; ++i)
        {
            da->voidArr[i] = da->voidArr[i - 1];
        }
        break;
    default:
        return 1;
    }

    return 0;
}

static inline int _is_empty(const DynamicArray *const da)
{
    return da->count <= 0 ? 1 : 0;
}

static int _is_out_of_bounds(const DynamicArray *const da, size_t index)
{
    return index < 0 || index >= da->count ? 1 : 0;
}

static size_t _single_item_size(enum DynamicArrayType type)
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

static int _new_items_array(DynamicArray *const da)
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

static int _get_pointer_at_index(const DynamicArray *const da, const size_t index, void **output)
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

static int _remove_at(DynamicArray *const da, const size_t index)
{
    if (da == NULL)
        return 1;

    if (da->count <= 0)
        return 0;

    if (_shrinkDA(da))
        return 1;

    DynamicArray *tmp = new_dynamic_array(da->type);
    if (tmp == NULL)
        return 2;

    for (size_t i = 0; i < da->count; ++i)
    {
        if (i == index)
            continue;

        switch (da->type)
        {
        case 0:
            // int
            if (push_da(tmp, &((da->intArr)[i])))
                goto _error_case;
            break;
        case 1:
            // char
            if (push_da(tmp, &((da->charArr)[i])))
                goto _error_case;
            break;
        case 2:
            // float
            if (push_da(tmp, &((da->floatArr)[i])))
                goto _error_case;
            break;
        case 3:
            // double
            if (push_da(tmp, &((da->doubleArr)[i])))
                goto _error_case;
            break;
        case 4:
            // void*
            if (push_da(tmp, (da->voidArr)[i]))
                goto _error_case;
            break;
        default:
            goto _error_case;
        }
    }

    switch (da->type)
    {
    case 0:
        // int
        free(da->intArr);
        da->intArr = tmp->intArr;
        break;
    case 1:
        // char
        free(da->charArr);
        da->charArr = tmp->charArr;
        break;
    case 2:
        // float
        free(da->floatArr);
        da->floatArr = tmp->floatArr;
        break;
    case 3:
        // double
        free(da->doubleArr);
        da->doubleArr = tmp->doubleArr;
        break;
    case 4:
        // void*
        free(da->voidArr);
        da->voidArr = tmp->voidArr;
        break;
    }

    (da->count)--;
    /*
    The actual array, in the union, must not need to be freed.
    Thats why free() and not free_dynamic_array().
    */
    free(tmp);
    return 0;

_error_case:
    free_dynamic_array(tmp);
    return 3;
}

// ##################   creation and destruction  ##################

DynamicArray *new_dynamic_array(enum DynamicArrayType const type)
{
    if (_enum_check(type))
        return NULL;

    DynamicArray *da = (DynamicArray *)malloc(sizeof(DynamicArray));
    if (da == NULL)
        return NULL;

    da->count = 0;
    da->capacity = DYNAMIC_ARRAY_INIT_CAPACITY;
    da->type = type;
    da->singleItemSize = _single_item_size(type);

    int err = _new_items_array(da);
    if (err)
    {
        free(da);
        return NULL;
    }

    return da;
}

void free_dynamic_array(DynamicArray *const da)
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

/*
int push_da(DynamicArray *const da, void *const item)
{
    if (item == NULL || da == NULL)
        return 1;

    int err = _expand_da(da);
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
 */

int push_int_da(DynamicArray *const da, int const item)
{
    if (da == NULL)
        return 1;

    if (da->type != INT)
        return 3;

    int err = _expand_da(da);
    if (err)
        return err;

    (da->intArr)[(da->count)++] = item;

    return 0;
}

int push_char_da(DynamicArray *const da, char const item)
{
    if (da == NULL)
        return 1;

    if (da->type != CHAR)
        return 3;

    int err = _expand_da(da);
    if (err)
        return err;

    (da->charArr)[(da->count)++] = item;

    return 0;
}

int push_float_da(DynamicArray *const da, float const item)
{
    if (da == NULL)
        return 1;

    if (da->type != FLOAT)
        return 3;

    int err = _expand_da(da);
    if (err)
        return err;

    (da->floatArr)[(da->count)++] = item;

    return 0;
}

int push_double_da(DynamicArray *const da, double const item)
{
    if (da == NULL)
        return 1;

    if (da->type != DOUBLE)
        return 3;

    int err = _expand_da(da);
    if (err)
        return err;

    (da->doubleArr)[(da->count)++] = item;

    return 0;
}

int push_ptr_da(DynamicArray *const da, void *const item)
{
    if (da == NULL)
        return 1;

    if (da->type != VOID_PTR)
        return 3;

    int err = _expand_da(da);
    if (err)
        return err;

    (da->voidArr)[(da->count)++] = item;

    return 0;
}

/*
int unshift_da(DynamicArray *const da, void *const item)
{
    if (item == NULL || da == NULL)
        return 1;

    int err = _expand_da(da);
    if (err)
        return err;

    err = _enum_check(da->type);
    if (err)
        return 3;

    err = _move_one_right(da);
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
 */

int unshift_int_da(DynamicArray *const da, int const item)
{
    if (da == NULL)
        return 1;

    if (da->type != INT)
        return 3;

    int expansion_err = _expand_da(da);
    if (expansion_err)
        return expansion_err;

    int move_err = _move_one_right(da);
    if (move_err)
        return 3;

    (da->intArr)[0] = item;

    (da->count)++;

    return 0;
}

int unshift_char_da(DynamicArray *const da, char const item)
{
    if (da == NULL)
        return 1;

    if (da->type != CHAR)
        return 3;

    int expansion_err = _expand_da(da);
    if (expansion_err)
        return expansion_err;

    int move_err = _move_one_right(da);
    if (move_err)
        return 4;

    (da->charArr)[0] = item;

    (da->count)++;

    return 0;
}

int unshift_float_da(DynamicArray *const da, float const item)
{
    if (da == NULL)
        return 1;

    if (da->type != FLOAT)
        return 3;

    int expansion_err = _expand_da(da);
    if (expansion_err)
        return expansion_err;

    int move_err = _move_one_right(da);
    if (move_err)
        return 4;

    (da->floatArr)[0] = item;

    (da->count)++;

    return 0;
}

int unshift_double_da(DynamicArray *const da, double const item)
{
    if (da == NULL)
        return 1;

    if (da->type != DOUBLE)
        return 3;

    int expansion_err = _expand_da(da);
    if (expansion_err)
        return expansion_err;

    int move_err = _move_one_right(da);
    if (move_err)
        return 4;

    (da->doubleArr)[0] = item;

    (da->count)++;

    return 0;
}

int unshift_ptr_da(DynamicArray *const da, void *const item)
{
    if (da == NULL)
        return 1;

    if (da->type != VOID_PTR)
        return 3;

    int expansion_err = _expand_da(da);
    if (expansion_err)
        return expansion_err;

    int move_err = _move_one_right(da);
    if (move_err)
        return 4;

    (da->voidArr)[0] = item;

    (da->count)++;

    return 0;
}

// ##################   remove items   ##################

/*
int pop_da(DynamicArray *const da, void *const output)
{
    if (da == NULL || _is_empty(da))
    {
        return 1;
    }

    switch (da->type)
    {
    case 0:
        // int
        *(int *)output = (int)((da->intArr)[--(da->count)]);
        break;
    case 1:
        // char
        *(char *)output = (char)((da->charArr)[--(da->count)]);
        break;
    case 2:
        // float
        *(float *)output = (float)((da->floatArr)[--(da->count)]);
        break;
    case 3:
        // double
        *(double *)output = (double)((da->doubleArr)[--(da->count)]);
        break;
    case 4:
        // void**
        *(void **)output = (void *)((da->voidArr)[--(da->count)]);
        break;
    default:
        return 3;
    }
    _shrinkDA(da);

    return 0;
}
 */

int pop_int_da(DynamicArray *const da, int *const output)
{
    if (da == NULL)
        return 1;
        
    if (da->type != INT)
        return 3;

    if (_is_empty(da))
        return -1;

    *output = (int)((da->intArr)[--(da->count)]);

    int shrink_err = _shrinkDA(da);
    if (shrink_err)
        return shrink_err;

    return 0;
}

int pop_char_da(DynamicArray *const da, char *const output)
{
    if (da == NULL)
        return 1;

    if (da->type != CHAR)
        return 3;

    if (_is_empty(da))
        return -1;

    *output = (char)((da->charArr)[--(da->count)]);

    int shrink_err = _shrinkDA(da);
    if (shrink_err)
        return shrink_err;

    return 0;
}

int pop_float_da(DynamicArray *const da, float *const output)
{
    if (da == NULL)
        return 1;

    if (da->type != FLOAT)
        return 3;

    if (_is_empty(da))
        return -1;

    *output = (float)((da->floatArr)[--(da->count)]);

    int shrink_err = _shrinkDA(da);
    if (shrink_err)
        return shrink_err;

    return 0;
}

int pop_double_da(DynamicArray *const da, double *const output)
{
    if (da == NULL)
        return 1;

    if (da->type != DOUBLE)
        return 3;

    if (_is_empty(da))
        return -1;

    *output = (double)((da->doubleArr)[--(da->count)]);

    int shrink_err = _shrinkDA(da);
    if (shrink_err)
        return shrink_err;

    return 0;
}

int pop_ptr_da(DynamicArray *const da, void **const output)
{
    if (da == NULL)
        return 1;

    if (da->type != VOID_PTR)
        return 3;

    if (_is_empty(da))
        return -1;

    *output = (void *)((da->voidArr)[--(da->count)]);

    int shrink_err = _shrinkDA(da);
    if (shrink_err)
        return shrink_err;

    return 0;
}

int shift_da(DynamicArray *const da, void *const output)
{
    if (da == NULL || _is_empty(da))
    {
        return 1;
    }

    switch (da->type)
    {
    case 0:
        // int
        *(int *)output = (int)((da->intArr)[0]);
        break;
    case 1:
        // char
        *(char *)output = (char)((da->charArr)[0]);
        break;
    case 2:
        // float
        *(float *)output = (float)((da->floatArr)[0]);
        break;
    case 3:
        // double
        *(double *)output = (double)((da->doubleArr)[0]);
        break;
    case 4:
        // void**
        *(void **)output = (void *)((da->voidArr)[0]);
        break;
    default:
        return 3;
    }
    _move_one_left(da);
    (da->count)--;
    _shrinkDA(da);

    return 0;
}

int remove_at_da(DynamicArray *const da, const size_t index)
{
    if (da == NULL)
        return 1;

    int err_remove = _remove_at(da, index);
    if (err_remove)
        return 2;

    return 0;
}

int remove_first_da(DynamicArray *const da, void *const target)
{
    if (da == NULL || target == NULL)
        return 1;

    int target_index = -1;
    int status = index_of_da(da, &target_index, target);
    if (status == -1 || target_index <= -1)
    {
        // not found
        return -1;
    }
    else if (status >= 1)
    {
        // err
        return status;
    }
    // status == 0
    int err_remove = _remove_at(da, target_index);
    if (err_remove)
        return err_remove;

    return 0;
}

// ##################   processing   ##################

int apply_at_da(const DynamicArray *const da, const size_t index, const void (*worker)(void *item_ptr))
{
    if (da == NULL || worker == NULL)
        return 1;

    void *ptr = NULL;
    int err = _get_pointer_at_index(da, index, &ptr);
    if (err || ptr == NULL)
        return 2;

    worker(ptr);
    return 0;
}

int process_da(DynamicArray *const da, void (*processor)(void *item_ptr))
{
    if (da == NULL || processor == NULL)
        return 1;

    void *ptr = NULL;
    for (size_t i = 0; i < da->count; ++i)
    {
        int err = _get_pointer_at_index(da, i, &ptr);
        if (err || ptr == NULL)
            return 2;

        processor(ptr);
    }

    return 0;
}

int filter_da(DynamicArray *const da, int (*filter)(void *item_ptr))
{
    if (da == NULL || filter == NULL)
        return 1;

    DynamicArray *tempDA = new_dynamic_array(da->type);
    if (tempDA == NULL)
        return 1;

    void *ptr = NULL;

    for (size_t i = 0; i < da->count; ++i)
    {
        int err = _get_pointer_at_index(da, i, &ptr);
        if (err || ptr == NULL)
            return 2;

        if (filter(ptr) == 1)
            push_da(tempDA, ptr);
    }

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

int at_da(DynamicArray *const da, size_t index, void **output)
{
    if (da == NULL || _is_out_of_bounds(da, index))
        return 1;

    int err = _get_pointer_at_index(da, index, output);
    if (err)
        return 2;

    return 0;
}

int find_da(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr))
{
    if (da == NULL || output == NULL || selector == NULL)
        return 1;

    void *ptr = (void *)malloc(sizeof(void *));
    for (size_t i = 0; i < da->count; ++i)
    {
        int err = _get_pointer_at_index(da, i, &ptr);
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

int find_last_da(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr))
{
    if (da == NULL || output == NULL || selector == NULL)
        return 1;

    void *ptr = (void *)malloc(sizeof(void *));
    for (size_t i = da->count - 1; i >= 0; --i)
    {
        int err = _get_pointer_at_index(da, i, &ptr);
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

int find_index_da(DynamicArray *const da, int *const output, int (*selector)(void *itemPtr))
{
    if (da == NULL || output == NULL || selector == NULL)
        return 1;

    void *ptr = (void *)malloc(sizeof(void *));
    for (size_t i = 0; i < da->count; ++i)
    {
        int err = _get_pointer_at_index(da, i, &ptr);
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

int find_last_index_da(DynamicArray *const da, int *const output, int (*selector)(void *itemPtr))
{
    if (da == NULL || output == NULL || selector == NULL)
        return 1;

    void *ptr = (void *)malloc(sizeof(void *));
    for (size_t i = da->count - 1; i >= 0; --i)
    {
        int err = _get_pointer_at_index(da, i, &ptr);
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

int index_of_da(DynamicArray *const da, int *const output, void *const value)
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