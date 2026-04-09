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
		int *int_arr = realloc(da->int_arr, newCapacity * da->single_item_size);
		if (int_arr == NULL)
			return 2;
		da->int_arr = int_arr;
		break;
	case 1:
		// char
		char *char_arr = realloc(da->char_arr, newCapacity * da->single_item_size);
		if (char_arr == NULL)
			return 2;
		da->char_arr = char_arr;
		break;
	case 2:
		// float
		float *float_arr = realloc(da->float_arr, newCapacity * da->single_item_size);
		if (float_arr == NULL)
			return 2;
		da->float_arr = float_arr;
		break;
	case 3:
		// double
		double *double_arr = realloc(da->double_arr, newCapacity * da->single_item_size);
		if (double_arr == NULL)
			return 2;
		da->double_arr = double_arr;
		break;
	case 4:
		// void*
		void **void_arr = realloc(da->void_arr, newCapacity * da->single_item_size);
		if (void_arr == NULL)
			return 2;
		da->void_arr = void_arr;
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
		return 1;
	}

	return 0;
}

static inline int _is_empty(const DynamicArray *const da)
{
	return da->count <= 0 ? 1 : 0;
}

static inline int _is_out_of_bounds(const DynamicArray *const da, size_t index)
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
		int *int_arr = malloc(da->capacity * da->single_item_size);
		if (int_arr == NULL)
			return 2;
		da->int_arr = int_arr;
		break;
	case 1:
		// char
		char *char_arr = malloc(da->capacity * da->single_item_size);
		if (char_arr == NULL)
			return 2;
		da->char_arr = char_arr;
		break;
	case 2:
		// float
		float *float_arr = malloc(da->capacity * da->single_item_size);
		if (float_arr == NULL)
			return 2;
		da->float_arr = float_arr;
		break;
	case 3:
		// double
		double *double_arr = malloc(da->capacity * da->single_item_size);
		if (double_arr == NULL)
			return 2;
		da->double_arr = double_arr;
		break;
	case 4:
		// void**
		void **void_arr = malloc(da->capacity * da->single_item_size);
		if (void_arr == NULL)
			return 2;
		da->void_arr = void_arr;
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
		*output = (void *)&((da->int_arr)[index]);
		return 0;
	case 1:
		// char
		*output = (void *)&((da->char_arr)[index]);
		return 0;
	case 2:
		// float
		*output = (void *)&((da->float_arr)[index]);
		return 0;
	case 3:
		// double
		*output = (void *)&((da->double_arr)[index]);
		return 0;
	case 4:
		// void**
		*output = (void *)(da->void_arr)[index];
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
			if (push_int_da(tmp, (da->int_arr)[i]))
				goto _error_case;
			break;
		case 1:
			// char
			if (push_char_da(tmp, (da->char_arr)[i]))
				goto _error_case;
			break;
		case 2:
			// float
			if (push_float_da(tmp, (da->float_arr)[i]))
				goto _error_case;
			break;
		case 3:
			// double
			if (push_double_da(tmp, (da->double_arr)[i]))
				goto _error_case;
			break;
		case 4:
			// void*
			if (push_ptr_da(tmp, (da->void_arr)[i]))
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

	size_t item_size = _single_item_size(type);
	if (item_size == -1)
	{
		free(da);
		return NULL;
	}
	da->single_item_size = item_size;

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

int push_int_da(DynamicArray *const da, int const item)
{
	if (da == NULL)
		return 1;

	if (da->type != INT)
		return 3;

	int err = _expand_da(da);
	if (err)
		return err;

	(da->int_arr)[(da->count)++] = item;

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

	(da->char_arr)[(da->count)++] = item;

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

	(da->float_arr)[(da->count)++] = item;

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

	(da->double_arr)[(da->count)++] = item;

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

	(da->void_arr)[(da->count)++] = item;

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

	(da->int_arr)[0] = item;

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

	(da->char_arr)[0] = item;

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

	(da->float_arr)[0] = item;

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

	(da->double_arr)[0] = item;

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

	(da->void_arr)[0] = item;

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

int pop_int_da(DynamicArray *const da, int *const output)
{
	if (da == NULL)
		return 1;

	if (da->type != INT)
		return 3;

	if (_is_empty(da))
		return -1;

	*output = (int)((da->int_arr)[--(da->count)]);

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

	*output = (char)((da->char_arr)[--(da->count)]);

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

	*output = (float)((da->float_arr)[--(da->count)]);

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

	*output = (double)((da->double_arr)[--(da->count)]);

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

	*output = (void *)((da->void_arr)[--(da->count)]);

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return 0;
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

int shift_int_da(DynamicArray *const da, int *const output)
{
	if (da == NULL)
		return 1;

	if (da->type != INT)
		return 3;

	if (_is_empty(da))
		return -1;

	*output = (int)((da->int_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return 3;

	(da->count)--;

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return 0;
}

int shift_char_da(DynamicArray *const da, char *const output)
{
	if (da == NULL)
		return 1;

	if (da->type != CHAR)
		return 3;

	if (_is_empty(da))
		return -1;

	*output = (char)((da->char_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return 3;

	(da->count)--;

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return 0;
}

int shift_float_da(DynamicArray *const da, float *const output)
{
	if (da == NULL)
		return 1;

	if (da->type != FLOAT)
		return 3;

	if (_is_empty(da))
		return -1;

	*output = (float)((da->float_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return 3;

	(da->count)--;

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return 0;
}

int shift_double_da(DynamicArray *const da, double *const output)
{
	if (da == NULL)
		return 1;

	if (da->type != DOUBLE)
		return 3;

	if (_is_empty(da))
		return -1;

	*output = (double)((da->double_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return 3;

	(da->count)--;

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

	return 0;
}

int shift_ptr_da(DynamicArray *const da, void **const output)
{
	if (da == NULL)
		return 1;

	if (da->type != VOID_PTR)
		return 3;

	if (_is_empty(da))
		return -1;

	*output = (void *)((da->void_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return 3;

	(da->count)--;

	int shrink_err = _shrinkDA(da);
	if (shrink_err)
		return shrink_err;

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
		{
			switch (da->type)
			{
			case 0:
				// int
				push_int_da(tempDA, *(int *)ptr);
				break;
			case 1:
				// char
				push_char_da(tempDA, *(char *)ptr);
				break;
			case 2:
				// float
				push_float_da(tempDA, *(float *)ptr);
				break;
			case 3:
				// double
				push_double_da(tempDA, *(double *)ptr);
				break;
			case 4:
				// void**
				push_ptr_da(tempDA, ptr);
				break;
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

	void *ptr = NULL;
	for (size_t i = 0; i < da->count; ++i)
	{
		int err = _get_pointer_at_index(da, i, &ptr);
		if (err)
			return 2;

		if (selector(ptr) == 1)
		{
			*output = ptr;
			return 0;
		}
	}

	return -1;
}

int find_last_da(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr))
{
	if (da == NULL || output == NULL || selector == NULL)
		return 1;

	void *ptr = NULL;
	for (size_t i = da->count - 1; i >= 0; --i)
	{
		int err = _get_pointer_at_index(da, i, &ptr);
		if (err)
			return 2;

		if (selector(ptr) == 1)
		{
			*output = ptr;
			return 0;
		}
	}

	return -1;
}

int find_index_da(DynamicArray *const da, int *const output, int (*selector)(void *itemPtr))
{
	if (da == NULL || output == NULL || selector == NULL)
		return 1;

	void *ptr = NULL;
	for (size_t i = 0; i < da->count; ++i)
	{
		int err = _get_pointer_at_index(da, i, &ptr);
		if (err)
			return 2;

		if (selector(ptr) == 1)
		{
			*output = i;
			return 0;
		}
	}

	return -1;
}

int find_last_index_da(DynamicArray *const da, int *const output, int (*selector)(void *itemPtr))
{
	if (da == NULL || output == NULL || selector == NULL)
		return 1;

	void *ptr = NULL;
	for (size_t i = da->count - 1; i >= 0; --i)
	{
		int err = _get_pointer_at_index(da, i, &ptr);
		if (err)
			return 2;

		if (selector(ptr) == 1)
		{
			*output = i;
			return 0;
		}
	}

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
			isEqual = ((int)(da->int_arr)[i]) == *(int *)value ? 1 : 0;
			break;
		case 1:
			// char
			isEqual = ((char)(da->char_arr)[i]) == *(char *)value ? 1 : 0;
			break;
		case 2:
			// float
			isEqual = ((float)(da->float_arr)[i]) == *(float *)value ? 1 : 0;
			break;
		case 3:
			// double
			isEqual = ((double)(da->double_arr)[i]) == *(double *)value ? 1 : 0;
			break;
		case 4:
			// void*
			isEqual = ((void *)(da->void_arr)[i]) == (void *)value ? 1 : 0;
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