#include <stdlib.h>
#include <string.h>
#include "../include/dynamic_array.h"

// ##################   static   ##################

static inline enum DynamicArrayError _enum_check(enum DynamicArrayType const type)
{
	switch (type)
	{
	case 0:
		// int
		return DA_SUCCESS;
	case 1:
		// char
		return DA_SUCCESS;
	case 2:
		// float
		return DA_SUCCESS;
	case 3:
		// double
		return DA_SUCCESS;
	case 4:
		// void*
		return DA_SUCCESS;
	default:
		return DA_ERR_TYPE_UNKNOWN;
	}
}

static enum DynamicArrayError _mem_realloc(DynamicArray *const da, size_t newCapacity)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	switch (da->type)
	{
	case 0:
		// int
		int *int_arr = realloc(da->int_arr, newCapacity * da->single_item_size);
		if (int_arr == NULL)
			return DA_ERR_MEMORY_ALLOCATION;
		da->int_arr = int_arr;
		break;
	case 1:
		// char
		char *char_arr = realloc(da->char_arr, newCapacity * da->single_item_size);
		if (char_arr == NULL)
			return DA_ERR_MEMORY_ALLOCATION;
		da->char_arr = char_arr;
		break;
	case 2:
		// float
		float *float_arr = realloc(da->float_arr, newCapacity * da->single_item_size);
		if (float_arr == NULL)
			return DA_ERR_MEMORY_ALLOCATION;
		da->float_arr = float_arr;
		break;
	case 3:
		// double
		double *double_arr = realloc(da->double_arr, newCapacity * da->single_item_size);
		if (double_arr == NULL)
			return DA_ERR_MEMORY_ALLOCATION;
		da->double_arr = double_arr;
		break;
	case 4:
		// void*
		void **void_arr = realloc(da->void_arr, newCapacity * da->single_item_size);
		if (void_arr == NULL)
			return DA_ERR_MEMORY_ALLOCATION;
		da->void_arr = void_arr;
		break;
	default:
		return DA_ERR_TYPE_UNKNOWN;
	}
	da->capacity = newCapacity;
	return DA_SUCCESS;
}

static enum DynamicArrayError _expand_da(DynamicArray *const da)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	size_t freeSpace = da->capacity - da->count;
	if (freeSpace < DYNAMIC_ARRAY_INIT_CAPACITY * 0.25)
	{
		size_t newCapacity = da->capacity + DYNAMIC_ARRAY_INIT_CAPACITY;
		int err = _mem_realloc(da, newCapacity); // new_capcity is set in _mem_realloc
		if (err)
			return err;
	}
	return DA_SUCCESS;
}

static enum DynamicArrayError _shrinkDA(DynamicArray *const da)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

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
	return DA_SUCCESS;
}

static enum DynamicArrayError _move_one_right(DynamicArray *const da)
{
	switch (da->type)
	{
	case 0:
		// int
		for (size_t i = da->count; i > 0; --i)
			da->int_arr[i] = da->int_arr[i - 1];
		break;
	case 1:
		// char
		for (size_t i = da->count; i > 0; --i)
			da->char_arr[i] = da->char_arr[i - 1];
		break;
	case 2:
		// float
		for (size_t i = da->count; i > 0; --i)
			da->float_arr[i] = da->float_arr[i - 1];
		break;
	case 3:
		// double
		for (size_t i = da->count; i > 0; --i)
			da->double_arr[i] = da->double_arr[i - 1];
		break;
	case 4:
		// void**
		for (size_t i = da->count; i > 0; --i)
			da->void_arr[i] = da->void_arr[i - 1];
		break;
	default:
		return DA_ERR_TYPE_UNKNOWN;
	}

	return DA_SUCCESS;
}

static enum DynamicArrayError _move_one_left(DynamicArray *const da)
{
	switch (da->type)
	{
	case 0:
		// int
		for (size_t i = 0; i < da->count - 1; ++i)
			da->int_arr[i] = da->int_arr[i + 1];
		break;
	case 1:
		// char
		for (size_t i = 0; i < da->count - 1; ++i)
			da->char_arr[i] = da->char_arr[i - 1];
		break;
	case 2:
		// float
		for (size_t i = 0; i < da->count - 1; ++i)
			da->float_arr[i] = da->float_arr[i - 1];
		break;
	case 3:
		// double
		for (size_t i = 0; i < da->count - 1; ++i)
			da->double_arr[i] = da->double_arr[i - 1];
		break;
	case 4:
		// void*
		for (size_t i = 0; i < da->count - 1; ++i)
			da->void_arr[i] = da->void_arr[i - 1];
		break;
	default:
		return DA_ERR_TYPE_UNKNOWN;
	}

	return DA_SUCCESS;
}

static inline int _is_empty(const DynamicArray *const da)
{
	// TODO: add NULL check
	return da->count <= 0 ? 1 : 0;
}

static inline enum DynamicArrayError _is_out_of_bounds(const DynamicArray *const da, size_t index)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (index < 0 || index >= da->count)
		return DA_ERR_INDEX_OUT_OF_BOUNDS;

	return DA_SUCCESS;
}

static enum DynamicArrayError _single_item_size(enum DynamicArrayType type, size_t *const output)
{
	switch (type)
	{
	case 0:
		// int
		*output = sizeof(int);
		break;
	case 1:
		// char
		*output = sizeof(char);
		break;
	case 2:
		// float
		*output = sizeof(float);
		break;
	case 3:
		// double
		*output = sizeof(double);
		break;
	case 4:
		// void*
		*output = sizeof(void *);
		break;
	default:
		return DA_ERR_TYPE_UNKNOWN;
	}

	return DA_SUCCESS;
}

static enum DynamicArrayError _new_items_array(DynamicArray *const da)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	switch (da->type)
	{
	case 0:
		// int
		int *int_arr = malloc(da->capacity * da->single_item_size);
		if (int_arr == NULL)
			return DA_ERR_MEMORY_ALLOCATION;
		da->int_arr = int_arr;
		break;
	case 1:
		// char
		char *char_arr = malloc(da->capacity * da->single_item_size);
		if (char_arr == NULL)
			return DA_ERR_MEMORY_ALLOCATION;
		da->char_arr = char_arr;
		break;
	case 2:
		// float
		float *float_arr = malloc(da->capacity * da->single_item_size);
		if (float_arr == NULL)
			return DA_ERR_MEMORY_ALLOCATION;
		da->float_arr = float_arr;
		break;
	case 3:
		// double
		double *double_arr = malloc(da->capacity * da->single_item_size);
		if (double_arr == NULL)
			return DA_ERR_MEMORY_ALLOCATION;
		da->double_arr = double_arr;
		break;
	case 4:
		// void**
		void **void_arr = malloc(da->capacity * da->single_item_size);
		if (void_arr == NULL)
			return DA_ERR_MEMORY_ALLOCATION;
		da->void_arr = void_arr;
		break;
	default:
		return DA_ERR_TYPE_UNKNOWN;
	}
	return DA_SUCCESS;
}

static enum DynamicArrayError _get_pointer_at_index(const DynamicArray *const da, const size_t index, void **output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	int err_bounds_check = _is_out_of_bounds(da, index);
	if (err_bounds_check)
		return err_bounds_check;

	switch (da->type)
	{
	case 0:
		// int
		*output = (void *)&((da->int_arr)[index]);
		break;
	case 1:
		// char
		*output = (void *)&((da->char_arr)[index]);
		break;
	case 2:
		// float
		*output = (void *)&((da->float_arr)[index]);
		break;
	case 3:
		// double
		*output = (void *)&((da->double_arr)[index]);
		break;
	case 4:
		// void**
		*output = (void *)(da->void_arr)[index];
		break;
	default:
		return DA_ERR_TYPE_UNKNOWN;
	}
	return DA_SUCCESS;
}

static enum DynamicArrayError _remove_at(DynamicArray *const da, const size_t index)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->count <= 0)
		return DA_SUCCESS;

	int err_bounds_check = _is_out_of_bounds(da, index);
	if (err_bounds_check)
		return err_bounds_check;

	int err_shrink = _shrinkDA(da);
	if (err_shrink)
		return err_shrink;

	DynamicArray *tmp = NULL;
	int err_new_da_init = new_dynamic_array(da->type, &tmp);
	if (err_new_da_init)	
		return err_new_da_init;
	if (tmp == NULL)
		return DA_ERR_MEMORY_ALLOCATION;

	int err_code = 0;

	for (size_t i = 0; i < da->count; ++i)
	{
		if (i == index)
			continue;

		switch (da->type)
		{
		case 0:
			// int
			err_code = push_int_da(tmp, (da->int_arr)[i]);
			if (err_code)
				goto _error_case;
			break;
		case 1:
			// char
			err_code = push_char_da(tmp, (da->char_arr)[i]);
			if (err_code)
				goto _error_case;
			break;
		case 2:
			// float
			err_code = push_float_da(tmp, (da->float_arr)[i]);
			if (err_code)
				goto _error_case;
			break;
		case 3:
			// double
			err_code = push_double_da(tmp, (da->double_arr)[i]);
			if (err_code)
				goto _error_case;
			break;
		case 4:
			// void*
			err_code = push_ptr_da(tmp, (da->void_arr)[i]);
			if (err_code)
				goto _error_case;
			break;
		default:
			err_code = DA_ERR_TYPE_UNKNOWN;
			goto _error_case;
		}
	}

	switch (da->type)
	{
	case 0:
		// int
		free(da->int_arr);
		da->int_arr = tmp->int_arr;
		break;
	case 1:
		// char
		free(da->char_arr);
		da->char_arr = tmp->char_arr;
		break;
	case 2:
		// float
		free(da->float_arr);
		da->float_arr = tmp->float_arr;
		break;
	case 3:
		// double
		free(da->double_arr);
		da->double_arr = tmp->double_arr;
		break;
	case 4:
		// void*
		free(da->void_arr);
		da->void_arr = tmp->void_arr;
		break;
	default:
		err_code = DA_ERR_TYPE_UNKNOWN;
		goto _error_case;
	}

	(da->count)--;
	/*
	The actual array, in the union, must not need to be freed.
	Thats why free() and not free_dynamic_array().
	*/
	free(tmp);
	return DA_SUCCESS;

_error_case:
	free_dynamic_array(tmp);
	return err_code;
}

// ##################   creation and destruction  ##################

enum DynamicArrayError new_dynamic_array(enum DynamicArrayType const type, DynamicArray **const output)
{
	if (_enum_check(type))
		return DA_ERR_TYPE_UNKNOWN;

	DynamicArray *da = (DynamicArray *)malloc(sizeof(DynamicArray));
	if (da == NULL)
		return DA_ERR_MEMORY_ALLOCATION;

	da->count = 0;
	da->capacity = DYNAMIC_ARRAY_INIT_CAPACITY;
	da->type = type;

	size_t item_size = 0;
	int err_item_size = _single_item_size(type, &item_size);
	if (err_item_size)
	{
		free(da);
		return DA_ERR_ITEM_SIZE_DETERMINATION;
	}
	da->single_item_size = item_size;

	int err_items_array = _new_items_array(da);
	if (err_items_array)
	{
		free(da);
		return err_items_array;
	}

	*output = da;

	return DA_SUCCESS;
}

enum DynamicArrayError free_dynamic_array(DynamicArray *const da)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	switch (da->type)
	{
	case 0:
		// int
		free(da->int_arr);
		break;
	case 1:
		// char
		free(da->char_arr);
		break;
	case 2:
		// float
		free(da->float_arr);
		break;
	case 3:
		// double
		free(da->double_arr);
		break;
	case 4:
		// void*
		free(da->void_arr);
		break;
	default:
		return DA_ERR_TYPE_UNKNOWN;
	}

	free(da);
	return DA_SUCCESS;
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
	(da->int_arr)[(da->count)++] = *(int *)item;
	break;
    case 1:
	// char
	(da->char_arr)[(da->count)++] = *(char *)item;
	break;
    case 2:
	// float
	(da->float_arr)[(da->count)++] = *(float *)item;
	break;
    case 3:
	// double
	(da->double_arr)[(da->count)++] = *(double *)item;
	break;
    case 4:
	// void*
	(da->void_arr)[(da->count)++] = item;
	break;
    default:
	return 3;
    }

    return 0;
}
 */

enum DynamicArrayError push_int_da(DynamicArray *const da, int const item)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != INT)
		return DA_ERR_TYPE_MISMATCH;

	int err = _expand_da(da);
	if (err)
		return err;

	(da->int_arr)[(da->count)++] = item;

	return DA_SUCCESS;
}

enum DynamicArrayError push_char_da(DynamicArray *const da, char const item)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != CHAR)
		return DA_ERR_TYPE_MISMATCH;

	int err = _expand_da(da);
	if (err)
		return err;

	(da->char_arr)[(da->count)++] = item;

	return DA_SUCCESS;
}

enum DynamicArrayError push_float_da(DynamicArray *const da, float const item)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != FLOAT)
		return DA_ERR_TYPE_MISMATCH;

	int err = _expand_da(da);
	if (err)
		return err;

	(da->float_arr)[(da->count)++] = item;

	return DA_SUCCESS;
}

enum DynamicArrayError push_double_da(DynamicArray *const da, double const item)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != DOUBLE)
		return DA_ERR_TYPE_MISMATCH;

	int err = _expand_da(da);
	if (err)
		return err;

	(da->double_arr)[(da->count)++] = item;

	return DA_SUCCESS;
}

enum DynamicArrayError push_ptr_da(DynamicArray *const da, void *const item)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != VOID_PTR)
		return DA_ERR_TYPE_MISMATCH;

	int err = _expand_da(da);
	if (err)
		return err;

	(da->void_arr)[(da->count)++] = item;

	return DA_SUCCESS;
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
	(da->int_arr)[0] = *(int *)item;
	break;
    case 1:
	// char
	(da->char_arr)[0] = *(char *)item;
	break;
    case 2:
	// float
	(da->float_arr)[0] = *(float *)item;
	break;
    case 3:
	// double
	(da->double_arr)[0] = *(double *)item;
	break;
    case 4:
	// void*
	(da->void_arr)[0] = item;
	break;
    default:
	return 3;
    }

    (da->count)++;

    return 0;
}
 */

enum DynamicArrayError unshift_int_da(DynamicArray *const da, int const item)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != INT)
		return DA_ERR_TYPE_MISMATCH;

	int expansion_err = _expand_da(da);
	if (expansion_err)
		return expansion_err;

	int move_err = _move_one_right(da);
	if (move_err)
		return move_err;

	(da->int_arr)[0] = item;

	(da->count)++;

	return DA_SUCCESS;
}

enum DynamicArrayError unshift_char_da(DynamicArray *const da, char const item)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != CHAR)
		return DA_ERR_TYPE_MISMATCH;

	int expansion_err = _expand_da(da);
	if (expansion_err)
		return expansion_err;

	int move_err = _move_one_right(da);
	if (move_err)
		return move_err;

	(da->char_arr)[0] = item;

	(da->count)++;

	return DA_SUCCESS;
}

enum DynamicArrayError unshift_float_da(DynamicArray *const da, float const item)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != FLOAT)
		return DA_ERR_TYPE_MISMATCH;

	int expansion_err = _expand_da(da);
	if (expansion_err)
		return expansion_err;

	int move_err = _move_one_right(da);
	if (move_err)
		return move_err;

	(da->float_arr)[0] = item;

	(da->count)++;

	return DA_SUCCESS;
}

enum DynamicArrayError unshift_double_da(DynamicArray *const da, double const item)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != DOUBLE)
		return DA_ERR_TYPE_MISMATCH;

	int expansion_err = _expand_da(da);
	if (expansion_err)
		return expansion_err;

	int move_err = _move_one_right(da);
	if (move_err)
		return move_err;

	(da->double_arr)[0] = item;

	(da->count)++;

	return DA_SUCCESS;
}

enum DynamicArrayError unshift_ptr_da(DynamicArray *const da, void *const item)
{
	if (da == NULL)
		return DA_SUCCESS;

	if (da->type != VOID_PTR)
		return DA_ERR_TYPE_MISMATCH;

	int expansion_err = _expand_da(da);
	if (expansion_err)
		return expansion_err;

	int move_err = _move_one_right(da);
	if (move_err)
		return move_err;

	(da->void_arr)[0] = item;

	(da->count)++;

	return DA_SUCCESS;
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
	*(int *)output = (int)((da->int_arr)[--(da->count)]);
	break;
    case 1:
	// char
	*(char *)output = (char)((da->char_arr)[--(da->count)]);
	break;
    case 2:
	// float
	*(float *)output = (float)((da->float_arr)[--(da->count)]);
	break;
    case 3:
	// double
	*(double *)output = (double)((da->double_arr)[--(da->count)]);
	break;
    case 4:
	// void**
	*(void **)output = (void *)((da->void_arr)[--(da->count)]);
	break;
    default:
	return 3;
    }
    _shrinkDA(da);

    return 0;
}
 */

enum DynamicArrayError pop_int_da(DynamicArray *const da, int *const output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != INT)
		return DA_ERR_TYPE_MISMATCH;

	if (_is_empty(da))
		return DA_ARRAY_EMPTY;

	*output = (int)((da->int_arr)[--(da->count)]);

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return DA_SUCCESS;
}

enum DynamicArrayError pop_char_da(DynamicArray *const da, char *const output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != CHAR)
		return DA_ERR_TYPE_MISMATCH;

	if (_is_empty(da))
		return DA_ARRAY_EMPTY;

	*output = (char)((da->char_arr)[--(da->count)]);

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return DA_SUCCESS;
}

enum DynamicArrayError pop_float_da(DynamicArray *const da, float *const output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != FLOAT)
		return DA_ERR_TYPE_MISMATCH;

	if (_is_empty(da))
		return DA_ARRAY_EMPTY;

	*output = (float)((da->float_arr)[--(da->count)]);

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return DA_SUCCESS;
}

enum DynamicArrayError pop_double_da(DynamicArray *const da, double *const output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != DOUBLE)
		return DA_ERR_TYPE_MISMATCH;

	if (_is_empty(da))
		return DA_ARRAY_EMPTY;

	*output = (double)((da->double_arr)[--(da->count)]);

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return DA_SUCCESS;
}

enum DynamicArrayError pop_ptr_da(DynamicArray *const da, void **const output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != VOID_PTR)
		return DA_ERR_TYPE_MISMATCH;

	if (_is_empty(da))
		return DA_ARRAY_EMPTY;

	*output = (void *)((da->void_arr)[--(da->count)]);

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return DA_SUCCESS;
}

/*
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
	*(int *)output = (int)((da->int_arr)[0]);
	break;
    case 1:
	// char
	*(char *)output = (char)((da->char_arr)[0]);
	break;
    case 2:
	// float
	*(float *)output = (float)((da->float_arr)[0]);
	break;
    case 3:
	// double
	*(double *)output = (double)((da->double_arr)[0]);
	break;
    case 4:
	// void**
	*(void **)output = (void *)((da->void_arr)[0]);
	break;
    default:
	return 3;
    }
    _move_one_left(da);
    (da->count)--;
    _shrinkDA(da);

    return 0;
}
 */

enum DynamicArrayError shift_int_da(DynamicArray *const da, int *const output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != INT)
		return DA_ERR_TYPE_MISMATCH;

	if (_is_empty(da))
		return DA_ARRAY_EMPTY;

	*output = (int)((da->int_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return move_err;

	(da->count)--;

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return DA_SUCCESS;
}

enum DynamicArrayError shift_char_da(DynamicArray *const da, char *const output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != CHAR)
		return DA_ERR_TYPE_MISMATCH;

	if (_is_empty(da))
		return DA_ARRAY_EMPTY;

	*output = (char)((da->char_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return move_err;

	(da->count)--;

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return DA_SUCCESS;
}

enum DynamicArrayError shift_float_da(DynamicArray *const da, float *const output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != FLOAT)
		return DA_ERR_TYPE_MISMATCH;

	if (_is_empty(da))
		return DA_ARRAY_EMPTY;

	*output = (float)((da->float_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return move_err;

	(da->count)--;

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return DA_SUCCESS;
}

enum DynamicArrayError shift_double_da(DynamicArray *const da, double *const output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != DOUBLE)
		return DA_ERR_TYPE_MISMATCH;

	if (_is_empty(da))
		return DA_ARRAY_EMPTY;

	*output = (double)((da->double_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return move_err;

	(da->count)--;

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return DA_SUCCESS;
}

enum DynamicArrayError shift_ptr_da(DynamicArray *const da, void **const output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	if (da->type != VOID_PTR)
		return DA_ERR_TYPE_MISMATCH;

	if (_is_empty(da))
		return DA_ARRAY_EMPTY;

	*output = (void *)((da->void_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return move_err;

	(da->count)--;

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return DA_SUCCESS;
}

enum DynamicArrayError remove_at_da(DynamicArray *const da, const size_t index)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	int err_remove = _remove_at(da, index);
	if (err_remove)
		return err_remove;

	return DA_SUCCESS;
}

enum DynamicArrayError remove_first_da(DynamicArray *const da, void *const target)
{
	if (da == NULL || target == NULL)
		return DA_ERR_NULL_ARGUMENT;

	size_t target_index = 0;
	int err_index_of = index_of_da(da, &target_index, target);
	if (err_index_of)
		return err_index_of;

	int err_remove = _remove_at(da, target_index);
	if (err_remove)
		return err_remove;

	return DA_SUCCESS;
}

// ##################   processing   ##################

enum DynamicArrayError apply_at_da(const DynamicArray *const da, const size_t index, const void (*worker)(void *item_ptr))
{
	if (da == NULL || worker == NULL)
		return DA_ERR_NULL_ARGUMENT;

	void *ptr = NULL;
	int err_get_pointer = _get_pointer_at_index(da, index, &ptr);
	if (err_get_pointer)
		return err_get_pointer;

	worker(ptr);
	return DA_SUCCESS;
}

enum DynamicArrayError process_da(DynamicArray *const da, void (*processor)(void *item_ptr))
{
	if (da == NULL || processor == NULL)
		return DA_ERR_NULL_ARGUMENT;

	void *ptr = NULL;
	for (size_t i = 0; i < da->count; ++i)
	{
		int err_get_pointer = _get_pointer_at_index(da, i, &ptr);
		if (err_get_pointer)
			return err_get_pointer;

		processor(ptr);
	}

	return DA_SUCCESS;
}

enum DynamicArrayError filter_da(DynamicArray *const da, int (*filter)(void *item_ptr))
{
	if (da == NULL || filter == NULL)
		return DA_ERR_NULL_ARGUMENT;

	DynamicArray *tempDA = NULL;
	int err_new_da_init = new_dynamic_array(da->type, &tempDA);
	if (err_new_da_init)
		return err_new_da_init;

	if (tempDA == NULL)
		return DA_ERR_MEMORY_ALLOCATION;

	void *ptr = NULL;
	int err_code = 0;
	for (size_t i = 0; i < da->count; ++i)
	{
		int err_get_pointer = _get_pointer_at_index(da, i, &ptr);
		if (err_get_pointer)
			return err_get_pointer;

		if (filter(ptr) == 1)
		{
			switch (da->type)
			{
			case 0:
				// int
				err_code = push_int_da(tempDA, *(int *)ptr);
				if (err_code)
					goto _error_case;
				break;
			case 1:
				// char
				err_code = push_char_da(tempDA, *(char *)ptr);
				if (err_code)
					goto _error_case;
				break;
			case 2:
				// float
				err_code = push_float_da(tempDA, *(float *)ptr);
				if (err_code)
					goto _error_case;
				break;
			case 3:
				// double
				err_code = push_double_da(tempDA, *(double *)ptr);
				if (err_code)
					goto _error_case;
				break;
			case 4:
				// void**
				err_code = push_ptr_da(tempDA, ptr);
				if (err_code)
					goto _error_case;
				break;
			default:
				err_code = DA_ERR_TYPE_UNKNOWN;
				goto _error_case;
			}
		}
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
		free(da->int_arr);
		break;
	case 1:
		// char
		free(da->char_arr);
		break;
	case 2:
		// float
		free(da->float_arr);
		break;
	case 3:
		// double
		free(da->double_arr);
		break;
	case 4:
		// void**
		free(da->void_arr);
		break;
	default:
		err_code = DA_ERR_TYPE_UNKNOWN;
		goto _error_case;
	}
	memcpy(da, tempDA, sizeof(DynamicArray));
	free(tempDA); // The memory of tempDA::itemsArr is not freed. It is pointed to by da::itemsArr.

	return DA_SUCCESS;

_error_case:
	free_dynamic_array(tempDA);
	return err_code;
}

// ##################   searching   ##################

enum DynamicArrayError at_da(DynamicArray *const da, size_t index, void **output)
{
	if (da == NULL)
		return DA_ERR_NULL_ARGUMENT;

	int err_get_pointer = _get_pointer_at_index(da, index, output);
	if (err_get_pointer)
		return err_get_pointer;

	return DA_SUCCESS;
}

enum DynamicArrayError find_da(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr))
{
	if (da == NULL || output == NULL || selector == NULL)
		return DA_ERR_NULL_ARGUMENT;

	void *ptr = NULL;
	for (size_t i = 0; i < da->count; ++i)
	{
		int err_get_pointer = _get_pointer_at_index(da, i, &ptr);
		if (err_get_pointer)
			return err_get_pointer;

		if (selector(ptr) == 1)
		{
			*output = ptr;
			return DA_SUCCESS;
		}
	}

	return DA_ITEM_NOT_FOUND;
}

enum DynamicArrayError find_last_da(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr))
{
	if (da == NULL || output == NULL || selector == NULL)
		return DA_ERR_NULL_ARGUMENT;

	void *ptr = NULL;
	for (size_t i = da->count - 1; i >= 0; --i)
	{
		int err_get_pointer = _get_pointer_at_index(da, i, &ptr);
		if (err_get_pointer)
			return err_get_pointer;

		if (selector(ptr) == 1)
		{
			*output = ptr;
			return DA_SUCCESS;
		}
	}

	return DA_ITEM_NOT_FOUND;
}

enum DynamicArrayError find_index_da(DynamicArray *const da, size_t *const output, int (*selector)(void *itemPtr))
{
	if (da == NULL || output == NULL || selector == NULL)
		return DA_ERR_NULL_ARGUMENT;

	void *ptr = NULL;
	for (size_t i = 0; i < da->count; ++i)
	{
		int err_get_pointer = _get_pointer_at_index(da, i, &ptr);
		if (err_get_pointer)
			return err_get_pointer;

		if (selector(ptr) == 1)
		{
			*output = i;
			return DA_SUCCESS;
		}
	}

	return DA_ITEM_NOT_FOUND;
}

enum DynamicArrayError find_last_index_da(DynamicArray *const da, size_t *const output, int (*selector)(void *itemPtr))
{
	if (da == NULL || output == NULL || selector == NULL)
		return DA_ERR_NULL_ARGUMENT;

	void *ptr = NULL;
	for (size_t i = da->count - 1; i >= 0; --i)
	{
		int err_get_pointer = _get_pointer_at_index(da, i, &ptr);
		if (err_get_pointer)
			return err_get_pointer;

		if (selector(ptr) == 1)
		{
			*output = i;
			return DA_SUCCESS;
		}
	}

	return DA_ITEM_NOT_FOUND;
}

enum DynamicArrayError index_of_da(DynamicArray *const da, size_t *const output, void *const value)
{
	if (da == NULL || output == NULL || value == NULL)
		return DA_ERR_NULL_ARGUMENT;

	for (size_t i = 0; i < da->count; ++i)
	{
		int is_equal = 0;
		switch (da->type)
		{
		case 0:
			// int
			is_equal = ((int)(da->int_arr)[i]) == *(int *)value ? 1 : 0;
			break;
		case 1:
			// char
			is_equal = ((char)(da->char_arr)[i]) == *(char *)value ? 1 : 0;
			break;
		case 2:
			// float
			is_equal = ((float)(da->float_arr)[i]) == *(float *)value ? 1 : 0;
			break;
		case 3:
			// double
			is_equal = ((double)(da->double_arr)[i]) == *(double *)value ? 1 : 0;
			break;
		case 4:
			// void*
			is_equal = ((void *)(da->void_arr)[i]) == (void *)value ? 1 : 0;
			break;
		default:
			return DA_ERR_TYPE_UNKNOWN;
		}

		if (is_equal)
		{
			*output = i;
			return DA_SUCCESS;
		}
	}
	return DA_ITEM_NOT_FOUND;
}