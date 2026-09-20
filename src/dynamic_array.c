#include <stdlib.h>
#include <string.h>
#include "../include/dynamic_array.h"

// ##################   static   ##################

/**
 * @return GLX_SUCCESS for a valid type and GLX_ERR_TYPE_UNKNOWN for an invalid one.
 */
static inline enum GalxlibError _is_type_invalid(enum DynamicArrayType const type)
{
	switch (type)
	{
	case DA_INT:
		return GLX_SUCCESS;
	case DA_CHAR:
		return GLX_SUCCESS;
	case DA_FLOAT:
		return GLX_SUCCESS;
	case DA_DOUBLE:
		return GLX_SUCCESS;
	case DA_PTR:
		return GLX_SUCCESS;
	default:
		return GLX_ERR_TYPE_UNKNOWN;
	}
}

static enum GalxlibError _validate_da(const DynamicArray *const da)
{
	if (NULL == da)
		return GLX_ERR_NULL_ARGUMENT;

	switch (da->type)
	{
	case DA_INT:
		if (NULL == da->int_arr)
			return GLX_ERR_NULL_ARRAY;
		break;
	case DA_CHAR:
		if (NULL == da->char_arr)
			return GLX_ERR_NULL_ARRAY;
		break;
	case DA_FLOAT:
		if (NULL == da->float_arr)
			return GLX_ERR_NULL_ARRAY;
		break;
	case DA_DOUBLE:
		if (NULL == da->double_arr)
			return GLX_ERR_NULL_ARRAY;
		break;
	case DA_PTR:
		if (NULL == da->void_arr)
			return GLX_ERR_NULL_ARRAY;
		break;
	default:
		return GLX_ERR_TYPE_UNKNOWN;
	}

	return GLX_SUCCESS;
}

static enum GalxlibError _mem_realloc(DynamicArray *const da, size_t new_capcity)
{
	if (da == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	switch (da->type)
	{
	case DA_INT:
		int *int_arr = realloc(da->int_arr, new_capcity * da->single_item_size);
		if (int_arr == NULL)
			return GLX_ERR_MEMORY_ALLOCATION;
		da->int_arr = int_arr;
		break;
	case DA_CHAR:
		char *char_arr = realloc(da->char_arr, new_capcity * da->single_item_size);
		if (char_arr == NULL)
			return GLX_ERR_MEMORY_ALLOCATION;
		da->char_arr = char_arr;
		break;
	case DA_FLOAT:
		float *float_arr = realloc(da->float_arr, new_capcity * da->single_item_size);
		if (float_arr == NULL)
			return GLX_ERR_MEMORY_ALLOCATION;
		da->float_arr = float_arr;
		break;
	case DA_DOUBLE:
		double *double_arr = realloc(da->double_arr, new_capcity * da->single_item_size);
		if (double_arr == NULL)
			return GLX_ERR_MEMORY_ALLOCATION;
		da->double_arr = double_arr;
		break;
	case DA_PTR:
		void **void_arr = realloc(da->void_arr, new_capcity * da->single_item_size);
		if (void_arr == NULL)
			return GLX_ERR_MEMORY_ALLOCATION;
		da->void_arr = void_arr;
		break;
	default:
		return GLX_ERR_TYPE_UNKNOWN;
	}
	da->capacity = new_capcity;
	return GLX_SUCCESS;
}

static enum GalxlibError _expand_da(DynamicArray *const da)
{
	if (da == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	size_t free_space = da->capacity - da->count;
	if (free_space < DYNAMIC_ARRAY_INIT_CAPACITY * 0.25)
	{
		size_t new_capcity = da->capacity + DYNAMIC_ARRAY_INIT_CAPACITY;
		/**
		 * new_capcity is set in _mem_realloc.
		 */
		int err = _mem_realloc(da, new_capcity);
		if (err)
			return err;
	}
	return GLX_SUCCESS;
}

static enum GalxlibError _shrink_da(DynamicArray *const da)
{
	if (da == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	size_t free_space = da->capacity - da->count;
	if (free_space > DYNAMIC_ARRAY_INIT_CAPACITY * 2)
	{
		size_t new_capacity = da->capacity - free_space * 0.5;

		// safe guard
		if (new_capacity <= da->count)
			new_capacity += da->count;
		/**
		 * new_capcity is set in _mem_realloc.
		 */
		int err = _mem_realloc(da, new_capacity);
		if (err)
			return err;
	}
	return GLX_SUCCESS;
}

static enum GalxlibError _move_one_right(DynamicArray *const da)
{
	switch (da->type)
	{
	case DA_INT:
		for (size_t i = da->count; i > 0; --i)
			da->int_arr[i] = da->int_arr[i - 1];
		break;
	case DA_CHAR:
		for (size_t i = da->count; i > 0; --i)
			da->char_arr[i] = da->char_arr[i - 1];
		break;
	case DA_FLOAT:
		for (size_t i = da->count; i > 0; --i)
			da->float_arr[i] = da->float_arr[i - 1];
		break;
	case DA_DOUBLE:
		for (size_t i = da->count; i > 0; --i)
			da->double_arr[i] = da->double_arr[i - 1];
		break;
	case DA_PTR:
		for (size_t i = da->count; i > 0; --i)
			da->void_arr[i] = da->void_arr[i - 1];
		break;
	default:
		return GLX_ERR_TYPE_UNKNOWN;
	}

	return GLX_SUCCESS;
}

static enum GalxlibError _move_one_left(DynamicArray *const da)
{
	switch (da->type)
	{
	case DA_INT:
		for (size_t i = 0; i < da->count - 1; ++i)
			da->int_arr[i] = da->int_arr[i + 1];
		break;
	case DA_CHAR:
		for (size_t i = 0; i < da->count - 1; ++i)
			da->char_arr[i] = da->char_arr[i + 1];
		break;
	case DA_FLOAT:
		for (size_t i = 0; i < da->count - 1; ++i)
			da->float_arr[i] = da->float_arr[i + 1];
		break;
	case DA_DOUBLE:
		for (size_t i = 0; i < da->count - 1; ++i)
			da->double_arr[i] = da->double_arr[i + 1];
		break;
	case DA_PTR:
		for (size_t i = 0; i < da->count - 1; ++i)
			da->void_arr[i] = da->void_arr[i + 1];
		break;
	default:
		return GLX_ERR_TYPE_UNKNOWN;
	}

	return GLX_SUCCESS;
}

static inline enum GalxlibError _is_empty(const DynamicArray *const da, int *const output)
{
	if (da == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	*output = da->count <= 0 ? 1 : 0;
	return GLX_SUCCESS;
}

static inline enum GalxlibError _is_out_of_bounds(const DynamicArray *const da, size_t index)
{
	if (da == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (index < 0 || index >= da->count)
		return GLX_ERR_INDEX_OUT_OF_BOUNDS;

	return GLX_SUCCESS;
}

static enum GalxlibError _single_item_size(enum DynamicArrayType type, size_t *const output)
{
	if (NULL == output)
		return GLX_ERR_NULL_ARGUMENT;

	switch (type)
	{
	case DA_INT:
		*output = sizeof(int);
		break;
	case DA_CHAR:
		*output = sizeof(char);
		break;
	case DA_FLOAT:
		*output = sizeof(float);
		break;
	case DA_DOUBLE:
		*output = sizeof(double);
		break;
	case DA_PTR:
		*output = sizeof(void *);
		break;
	default:
		return GLX_ERR_TYPE_UNKNOWN;
	}

	return GLX_SUCCESS;
}

static enum GalxlibError _new_items_array(DynamicArray *const da)
{
	if (da == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	switch (da->type)
	{
	case DA_INT:
		int *int_arr = malloc(da->capacity * da->single_item_size);
		if (int_arr == NULL)
			return GLX_ERR_MEMORY_ALLOCATION;
		da->int_arr = int_arr;
		break;
	case DA_CHAR:
		char *char_arr = malloc(da->capacity * da->single_item_size);
		if (char_arr == NULL)
			return GLX_ERR_MEMORY_ALLOCATION;
		da->char_arr = char_arr;
		break;
	case DA_FLOAT:
		float *float_arr = malloc(da->capacity * da->single_item_size);
		if (float_arr == NULL)
			return GLX_ERR_MEMORY_ALLOCATION;
		da->float_arr = float_arr;
		break;
	case DA_DOUBLE:
		double *double_arr = malloc(da->capacity * da->single_item_size);
		if (double_arr == NULL)
			return GLX_ERR_MEMORY_ALLOCATION;
		da->double_arr = double_arr;
		break;
	case DA_PTR:
		void **void_arr = malloc(da->capacity * da->single_item_size);
		if (void_arr == NULL)
			return GLX_ERR_MEMORY_ALLOCATION;
		da->void_arr = void_arr;
		break;
	default:
		return GLX_ERR_TYPE_UNKNOWN;
	}
	return GLX_SUCCESS;
}

static enum GalxlibError _get_pointer_at_index(const DynamicArray *const da, const size_t index, void **const output)
{
	if (da == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	int err_bounds_check = _is_out_of_bounds(da, index);
	if (err_bounds_check)
		return err_bounds_check;

	switch (da->type)
	{
	case DA_INT:
		*output = (void *)&((da->int_arr)[index]);
		break;
	case DA_CHAR:
		*output = (void *)&((da->char_arr)[index]);
		break;
	case DA_FLOAT:
		*output = (void *)&((da->float_arr)[index]);
		break;
	case DA_DOUBLE:
		*output = (void *)&((da->double_arr)[index]);
		break;
	case DA_PTR:
		*output = (void *)(da->void_arr)[index];
		break;
	default:
		return GLX_ERR_TYPE_UNKNOWN;
	}
	return GLX_SUCCESS;
}

static enum GalxlibError _get_address_of_pointer_at_index(
    const DynamicArray *const da,
    const size_t index,
    void ***output)
{
	if (da == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (DA_PTR != da->type)
		return GLX_ERR_TYPE_MISMATCH;

	int err_bounds_check = _is_out_of_bounds(da, index);
	if (err_bounds_check)
		return err_bounds_check;

	*output = (void *)&((da->void_arr)[index]);

	return GLX_SUCCESS;
}

static enum GalxlibError _remove_at(DynamicArray *const da, const size_t index)
{
	if (da == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (da->count <= 0)
		return GLX_SUCCESS;

	int err_bounds_check = _is_out_of_bounds(da, index);
	if (err_bounds_check)
		return err_bounds_check;

	int err_shrink = _shrink_da(da);
	if (err_shrink)
		return err_shrink;

	DynamicArray *tmp = NULL;
	int err_new_da_init = new_dynamic_array(da->type, &tmp);
	if (err_new_da_init)
		return err_new_da_init;
	if (tmp == NULL)
		return GLX_ERR_MEMORY_ALLOCATION;

	int err_code = 0;

	for (size_t i = 0; i < da->count; ++i)
	{
		if (i == index)
			continue;

		switch (da->type)
		{
		case DA_INT:
			err_code = push_int_da(tmp, (da->int_arr)[i]);
			if (err_code)
				goto _error_case;
			break;
		case DA_CHAR:
			err_code = push_char_da(tmp, (da->char_arr)[i]);
			if (err_code)
				goto _error_case;
			break;
		case DA_FLOAT:
			err_code = push_float_da(tmp, (da->float_arr)[i]);
			if (err_code)
				goto _error_case;
			break;
		case DA_DOUBLE:
			err_code = push_double_da(tmp, (da->double_arr)[i]);
			if (err_code)
				goto _error_case;
			break;
		case DA_PTR:
			err_code = push_ptr_da(tmp, (da->void_arr)[i]);
			if (err_code)
				goto _error_case;
			break;
		default:
			err_code = GLX_ERR_TYPE_UNKNOWN;
			goto _error_case;
		}
	}

	switch (da->type)
	{
	case DA_INT:
		free(da->int_arr);
		da->int_arr = tmp->int_arr;
		break;
	case DA_CHAR:
		free(da->char_arr);
		da->char_arr = tmp->char_arr;
		break;
	case DA_FLOAT:
		free(da->float_arr);
		da->float_arr = tmp->float_arr;
		break;
	case DA_DOUBLE:
		free(da->double_arr);
		da->double_arr = tmp->double_arr;
		break;
	case DA_PTR:
		free(da->void_arr);
		da->void_arr = tmp->void_arr;
		break;
	default:
		err_code = GLX_ERR_TYPE_UNKNOWN;
		goto _error_case;
	}

	(da->count)--;
	/*
	The actual array, in the union, must not need to be freed.
	Thats why free() and not free_dynamic_array().
	*/
	free(tmp);
	return GLX_SUCCESS;

_error_case:
	free_dynamic_array(&tmp);
	return err_code;
}

// ##################   creation and destruction  ##################

enum GalxlibError new_dynamic_array(
    enum DynamicArrayType const type,
    DynamicArray **const output)
{
	if (output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (_is_type_invalid(type))
		return GLX_ERR_TYPE_UNKNOWN;

	DynamicArray *da = (DynamicArray *)malloc(sizeof(DynamicArray));
	if (da == NULL)
		return GLX_ERR_MEMORY_ALLOCATION;

	da->count = 0;
	da->capacity = DYNAMIC_ARRAY_INIT_CAPACITY;
	da->type = type;

	size_t item_size = 0;
	int err_item_size = _single_item_size(type, &item_size);
	if (err_item_size)
	{
		free(da);
		return GLX_ERR_ITEM_SIZE_DETERMINATION;
	}
	da->single_item_size = item_size;
	da->destructor = NULL;

	int err_items_array = _new_items_array(da);
	if (err_items_array)
	{
		free(da);
		return err_items_array;
	}

	*output = da;

	return GLX_SUCCESS;
}

enum GalxlibError new_dynamic_array_d(
    enum DynamicArrayType const type,
    void (*destructor)(void **ptr),
    DynamicArray **const output)
{
	if (output == NULL || destructor == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (_is_type_invalid(type))
		return GLX_ERR_TYPE_UNKNOWN;

	DynamicArray *da = (DynamicArray *)malloc(sizeof(DynamicArray));
	if (da == NULL)
		return GLX_ERR_MEMORY_ALLOCATION;

	da->count = 0;
	da->capacity = DYNAMIC_ARRAY_INIT_CAPACITY;
	da->type = type;

	size_t item_size = 0;
	int err_item_size = _single_item_size(type, &item_size);
	if (err_item_size)
	{
		free(da);
		return GLX_ERR_ITEM_SIZE_DETERMINATION;
	}
	da->single_item_size = item_size;
	da->destructor = destructor;

	int err_items_array = _new_items_array(da);
	if (err_items_array)
	{
		free(da);
		return err_items_array;
	}

	*output = da;

	return GLX_SUCCESS;
}

enum GalxlibError free_dynamic_array(DynamicArray **const da)
{
	if (da == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (*da == NULL)
		return GLX_SUCCESS;

	switch ((*da)->type)
	{
	case DA_INT:
		free((*da)->int_arr);
		(*da)->int_arr = NULL;
		break;
	case DA_CHAR:
		free((*da)->char_arr);
		(*da)->char_arr = NULL;
		break;
	case DA_FLOAT:
		free((*da)->float_arr);
		(*da)->float_arr = NULL;
		break;
	case DA_DOUBLE:
		free((*da)->double_arr);
		(*da)->double_arr = NULL;
		break;
	case DA_PTR:
		free((*da)->void_arr);
		(*da)->void_arr = NULL;
		break;
	default:
		return GLX_ERR_TYPE_UNKNOWN;
	}

	free(*da);
	*da = NULL;

	return GLX_SUCCESS;
}

enum GalxlibError free_dynamic_array_d(DynamicArray **const da)
{
	if (da == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (*da == NULL)
		return GLX_SUCCESS;

	switch ((*da)->type)
	{
	case DA_INT:
		free((*da)->int_arr);
		(*da)->int_arr = NULL;
		break;
	case DA_CHAR:
		free((*da)->char_arr);
		(*da)->char_arr = NULL;
		break;
	case DA_FLOAT:
		free((*da)->float_arr);
		(*da)->float_arr = NULL;
		break;
	case DA_DOUBLE:
		free((*da)->double_arr);
		(*da)->double_arr = NULL;
		break;
	case DA_PTR:
		if ((*da)->destructor == NULL)
			return GLX_ERR_NULL_DESTRUCTOR;

		int err = activate_destructor_da(*da);
		if (err)
			return err;

		free((*da)->void_arr);
		(*da)->void_arr = NULL;
		break;
	default:
		return GLX_ERR_TYPE_UNKNOWN;
	}

	free(*da);
	*da = NULL;

	return GLX_SUCCESS;
}

// ##################   add items   ##################

enum GalxlibError push_int_da(DynamicArray *const da, int const item)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_INT)
		return GLX_ERR_TYPE_MISMATCH;

	int err = _expand_da(da);
	if (err)
		return err;

	(da->int_arr)[(da->count)++] = item;

	return GLX_SUCCESS;
}

enum GalxlibError push_char_da(DynamicArray *const da, char const item)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_CHAR)
		return GLX_ERR_TYPE_MISMATCH;

	int err = _expand_da(da);
	if (err)
		return err;

	(da->char_arr)[(da->count)++] = item;

	return GLX_SUCCESS;
}

enum GalxlibError push_float_da(DynamicArray *const da, float const item)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_FLOAT)
		return GLX_ERR_TYPE_MISMATCH;

	int err = _expand_da(da);
	if (err)
		return err;

	(da->float_arr)[(da->count)++] = item;

	return GLX_SUCCESS;
}

enum GalxlibError push_double_da(DynamicArray *const da, double const item)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_DOUBLE)
		return GLX_ERR_TYPE_MISMATCH;

	int err = _expand_da(da);
	if (err)
		return err;

	(da->double_arr)[(da->count)++] = item;

	return GLX_SUCCESS;
}

enum GalxlibError push_ptr_da(DynamicArray *const da, void *const item)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_PTR)
		return GLX_ERR_TYPE_MISMATCH;

	int err = _expand_da(da);
	if (err)
		return err;

	(da->void_arr)[(da->count)++] = item;

	return GLX_SUCCESS;
}

enum GalxlibError unshift_int_da(DynamicArray *const da, int const item)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_INT)
		return GLX_ERR_TYPE_MISMATCH;

	int expansion_err = _expand_da(da);
	if (expansion_err)
		return expansion_err;

	int move_err = _move_one_right(da);
	if (move_err)
		return move_err;

	(da->int_arr)[0] = item;

	(da->count)++;

	return GLX_SUCCESS;
}

enum GalxlibError unshift_char_da(DynamicArray *const da, char const item)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_CHAR)
		return GLX_ERR_TYPE_MISMATCH;

	int expansion_err = _expand_da(da);
	if (expansion_err)
		return expansion_err;

	int move_err = _move_one_right(da);
	if (move_err)
		return move_err;

	(da->char_arr)[0] = item;

	(da->count)++;

	return GLX_SUCCESS;
}

enum GalxlibError unshift_float_da(DynamicArray *const da, float const item)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_FLOAT)
		return GLX_ERR_TYPE_MISMATCH;

	int expansion_err = _expand_da(da);
	if (expansion_err)
		return expansion_err;

	int move_err = _move_one_right(da);
	if (move_err)
		return move_err;

	(da->float_arr)[0] = item;

	(da->count)++;

	return GLX_SUCCESS;
}

enum GalxlibError unshift_double_da(DynamicArray *const da, double const item)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_DOUBLE)
		return GLX_ERR_TYPE_MISMATCH;

	int expansion_err = _expand_da(da);
	if (expansion_err)
		return expansion_err;

	int move_err = _move_one_right(da);
	if (move_err)
		return move_err;

	(da->double_arr)[0] = item;

	(da->count)++;

	return GLX_SUCCESS;
}

enum GalxlibError unshift_ptr_da(DynamicArray *const da, void *const item)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_PTR)
		return GLX_ERR_TYPE_MISMATCH;

	int expansion_err = _expand_da(da);
	if (expansion_err)
		return expansion_err;

	int move_err = _move_one_right(da);
	if (move_err)
		return move_err;

	(da->void_arr)[0] = item;

	(da->count)++;

	return GLX_SUCCESS;
}

// ##################   remove items   ##################

enum GalxlibError pop_int_da(DynamicArray *const da, int *const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_INT)
		return GLX_ERR_TYPE_MISMATCH;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	*output = (int)((da->int_arr)[--(da->count)]);

	int shrink_err = _shrink_da(da);
	if (shrink_err)
		return shrink_err;

	return GLX_SUCCESS;
}

enum GalxlibError pop_char_da(DynamicArray *const da, char *const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_CHAR)
		return GLX_ERR_TYPE_MISMATCH;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	*output = (char)((da->char_arr)[--(da->count)]);

	int shrink_err = _shrink_da(da);
	if (shrink_err)
		return shrink_err;

	return GLX_SUCCESS;
}

enum GalxlibError pop_float_da(DynamicArray *const da, float *const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_FLOAT)
		return GLX_ERR_TYPE_MISMATCH;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	*output = (float)((da->float_arr)[--(da->count)]);

	int shrink_err = _shrink_da(da);
	if (shrink_err)
		return shrink_err;

	return GLX_SUCCESS;
}

enum GalxlibError pop_double_da(DynamicArray *const da, double *const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_DOUBLE)
		return GLX_ERR_TYPE_MISMATCH;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	*output = (double)((da->double_arr)[--(da->count)]);

	int shrink_err = _shrink_da(da);
	if (shrink_err)
		return shrink_err;

	return GLX_SUCCESS;
}

enum GalxlibError pop_ptr_da(DynamicArray *const da, void **const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_PTR)
		return GLX_ERR_TYPE_MISMATCH;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	*output = (void *)((da->void_arr)[--(da->count)]);

	int shrink_err = _shrink_da(da);
	if (shrink_err)
		return shrink_err;

	return GLX_SUCCESS;
}

enum GalxlibError shift_int_da(DynamicArray *const da, int *const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_INT)
		return GLX_ERR_TYPE_MISMATCH;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	*output = (int)((da->int_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return move_err;

	(da->count)--;

	int shrink_err = _shrink_da(da);
	if (shrink_err)
		return shrink_err;

	return GLX_SUCCESS;
}

enum GalxlibError shift_char_da(DynamicArray *const da, char *const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_CHAR)
		return GLX_ERR_TYPE_MISMATCH;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	*output = (char)((da->char_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return move_err;

	(da->count)--;

	int shrink_err = _shrink_da(da);
	if (shrink_err)
		return shrink_err;

	return GLX_SUCCESS;
}

enum GalxlibError shift_float_da(DynamicArray *const da, float *const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_FLOAT)
		return GLX_ERR_TYPE_MISMATCH;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	*output = (float)((da->float_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return move_err;

	(da->count)--;

	int shrink_err = _shrink_da(da);
	if (shrink_err)
		return shrink_err;

	return GLX_SUCCESS;
}

enum GalxlibError shift_double_da(DynamicArray *const da, double *const output)
{
	if (da == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (da->type != DA_DOUBLE)
		return GLX_ERR_TYPE_MISMATCH;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	*output = (double)((da->double_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return move_err;

	(da->count)--;

	int shrink_err = _shrink_da(da);
	if (shrink_err)
		return shrink_err;

	return GLX_SUCCESS;
}

enum GalxlibError shift_ptr_da(DynamicArray *const da, void **const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (da->type != DA_PTR)
		return GLX_ERR_TYPE_MISMATCH;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	*output = (void *)((da->void_arr)[0]);

	int move_err = _move_one_left(da);
	if (move_err)
		return move_err;

	(da->count)--;

	int shrink_err = _shrink_da(da);
	if (shrink_err)
		return shrink_err;

	return GLX_SUCCESS;
}

enum GalxlibError remove_at_da(DynamicArray *const da, const size_t index)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	int err_remove = _remove_at(da, index);
	if (err_remove)
		return err_remove;

	return GLX_SUCCESS;
}

enum GalxlibError remove_first_da(DynamicArray *const da, void *const target)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (target == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	size_t target_index = 0;
	int err_index_of = index_of_da(da, target, &target_index);
	if (err_index_of)
		return err_index_of;

	int err_remove = _remove_at(da, target_index);
	if (err_remove)
		return err_remove;

	return GLX_SUCCESS;
}

// ##################   processing   ##################

enum GalxlibError apply_at_da(const DynamicArray *const da, const size_t index, const void (*worker)(void *item_ptr))
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (worker == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	void *ptr = NULL;
	int err_get_pointer = _get_pointer_at_index(da, index, &ptr);
	if (err_get_pointer)
		return err_get_pointer;

	worker(ptr);
	return GLX_SUCCESS;
}

enum GalxlibError process_da(DynamicArray *const da, void (*processor)(void *item_ptr))
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (processor == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	void *ptr = NULL;
	/**
	 * Empty check is done indirectly by the for-cicle.
	 */
	for (size_t i = 0; i < da->count; ++i)
	{
		int err_get_pointer = _get_pointer_at_index(da, i, &ptr);
		if (err_get_pointer)
			return err_get_pointer;

		processor(ptr);
	}

	return GLX_SUCCESS;
}

enum GalxlibError activate_destructor_da(DynamicArray *const da)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (DA_PTR != da->type)
		return GLX_ERR_TYPE_MISMATCH;

	if (da->destructor == NULL)
		return GLX_ERR_NULL_DESTRUCTOR;

	void **ptr = NULL;
	for (size_t i = 0; i < da->count; ++i)
	{
		int err_get_address = _get_address_of_pointer_at_index(da, i, &ptr);
		if (err_get_address)
			return err_get_address;

		(da->destructor)(ptr);
	}

	return GLX_SUCCESS;
}

enum GalxlibError apply_destructor_da(DynamicArray *const da, void (*destructor)(void **value))
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (destructor == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (DA_PTR != da->type)
		return GLX_ERR_TYPE_MISMATCH;

	void **ptr = NULL;
	for (size_t i = 0; i < da->count; ++i)
	{
		int err_get_address = _get_address_of_pointer_at_index(da, i, &ptr);
		if (err_get_address)
			return err_get_address;

		destructor(ptr);
	}

	return GLX_SUCCESS;
}

enum GalxlibError filter_da(DynamicArray *const da, int (*filter)(void *item_ptr))
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (filter == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	/**
	 * If the array is empty there is nothing to filter. A success error code must be returned.
	 */
	if (output_is_empty == 1)
		return GLX_SUCCESS;

	DynamicArray *tempDA = NULL;
	int err_new_da_init = new_dynamic_array(da->type, &tempDA);
	if (err_new_da_init)
		return err_new_da_init;

	if (tempDA == NULL)
		return GLX_ERR_MEMORY_ALLOCATION;

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
			case DA_INT:
				err_code = push_int_da(tempDA, *(int *)ptr);
				if (err_code)
					goto _error_case;
				break;
			case DA_CHAR:
				err_code = push_char_da(tempDA, *(char *)ptr);
				if (err_code)
					goto _error_case;
				break;
			case DA_FLOAT:
				err_code = push_float_da(tempDA, *(float *)ptr);
				if (err_code)
					goto _error_case;
				break;
			case DA_DOUBLE:
				err_code = push_double_da(tempDA, *(double *)ptr);
				if (err_code)
					goto _error_case;
				break;
			case DA_PTR:
				err_code = push_ptr_da(tempDA, ptr);
				if (err_code)
					goto _error_case;
				break;
			default:
				err_code = GLX_ERR_TYPE_UNKNOWN;
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
	case DA_INT:
		free(da->int_arr);
		break;
	case DA_CHAR:
		free(da->char_arr);
		break;
	case DA_FLOAT:
		free(da->float_arr);
		break;
	case DA_DOUBLE:
		free(da->double_arr);
		break;
	case DA_PTR:
		free(da->void_arr);
		break;
	default:
		err_code = GLX_ERR_TYPE_UNKNOWN;
		goto _error_case;
	}
	memcpy(da, tempDA, sizeof(DynamicArray));
	free(tempDA); // The memory of tempDA::itemsArr is not freed. It is pointed to by da::itemsArr.

	return GLX_SUCCESS;

_error_case:
	free_dynamic_array(&tempDA);
	return err_code;
}

// ##################   searching   ##################

enum GalxlibError at_da(DynamicArray *const da, const size_t index, void **const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (NULL == output)
		return GLX_ERR_NULL_ARGUMENT;

	int output_is_empty = 0;
	int err_is_empty = _is_empty(da, &output_is_empty);
	if (err_is_empty)
		return err_is_empty;
	if (output_is_empty == 1)
		return GLX_ERR_ZERO_LENGTH;

	return _get_pointer_at_index(da, index, output);
}

// enum GalxlibError find_da(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr))
// {
// 	if (da == NULL || output == NULL || selector == NULL)
// 		return GLX_ERR_NULL_ARGUMENT;

// 	void *ptr = NULL;
// 	for (size_t i = 0; i < da->count; ++i)
// 	{
// 		int err_get_pointer = _get_pointer_at_index(da, i, &ptr);
// 		if (err_get_pointer)
// 			return err_get_pointer;

// 		if (selector(ptr) == 1)
// 		{
// 			*output = ptr;
// 			return GLX_SUCCESS;
// 		}
// 	}

// 	return GLX_ERR_NO_MATCH;
// }

// enum GalxlibError find_last_da(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr))
// {
// 	if (da == NULL || output == NULL || selector == NULL)
// 		return GLX_ERR_NULL_ARGUMENT;

// 	void *ptr = NULL;
// 	for (size_t i = da->count; i-- > 0;)
// 	// for (size_t i = da->count - 1; i >= 0; --i)
// 	{
// 		int err_get_pointer = _get_pointer_at_index(da, i - 1, &ptr);
// 		if (err_get_pointer)
// 			return err_get_pointer;

// 		if (selector(ptr) == 1)
// 		{
// 			*output = ptr;
// 			return GLX_SUCCESS;
// 		}
// 	}

// 	return GLX_ERR_NO_MATCH;
// }

// enum GalxlibError find_index_da(DynamicArray *const da, size_t *const output, int (*selector)(void *itemPtr))
// {
// 	if (da == NULL || output == NULL || selector == NULL)
// 		return GLX_ERR_NULL_ARGUMENT;

// 	void *ptr = NULL;
// 	for (size_t i = 0; i < da->count; ++i)
// 	{
// 		int err_get_pointer = _get_pointer_at_index(da, i, &ptr);
// 		if (err_get_pointer)
// 			return err_get_pointer;

// 		if (selector(ptr) == 1)
// 		{
// 			*output = i;
// 			return GLX_SUCCESS;
// 		}
// 	}

// 	return GLX_ERR_NO_MATCH;
// }

// enum GalxlibError find_last_index_da(DynamicArray *const da, size_t *const output, int (*selector)(void *itemPtr))
// {
// 	if (da == NULL || output == NULL || selector == NULL)
// 		return GLX_ERR_NULL_ARGUMENT;

// 	void *ptr = NULL;
// 	for (size_t i = da->count; i-- > 0;)
// 	// for (size_t i = da->count - 1; i >= 0; --i)
// 	{
// 		int err_get_pointer = _get_pointer_at_index(da, i - 1, &ptr);
// 		if (err_get_pointer)
// 			return err_get_pointer;

// 		if (selector(ptr) == 1)
// 		{
// 			*output = i;
// 			return GLX_SUCCESS;
// 		}
// 	}

// 	return GLX_ERR_NO_MATCH;
// }

enum GalxlibError index_of_da(DynamicArray *const da, void *const value, size_t *const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (output == NULL || value == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (da->count == 0)
		return GLX_ERR_ZERO_LENGTH;

	for (size_t i = 0; i < da->count; ++i)
	{
		int is_equal = 0;
		switch (da->type)
		{
		case DA_INT:
			is_equal = ((int)(da->int_arr)[i]) == *(int *)value ? 1 : 0;
			break;
		case DA_CHAR:
			is_equal = ((char)(da->char_arr)[i]) == *(char *)value ? 1 : 0;
			break;
		case DA_FLOAT:
			is_equal = ((float)(da->float_arr)[i]) == *(float *)value ? 1 : 0;
			break;
		case DA_DOUBLE:
			is_equal = ((double)(da->double_arr)[i]) == *(double *)value ? 1 : 0;
			break;
		case DA_PTR:
			is_equal = ((void *)(da->void_arr)[i]) == (void *)value ? 1 : 0;
			break;
		default:
			return GLX_ERR_TYPE_UNKNOWN;
		}

		if (is_equal)
		{
			*output = i;
			return GLX_SUCCESS;
		}
	}
	return GLX_ERR_NO_MATCH;
}

// ##################   DynamicArrayIterator   ##################

enum GalxlibError new_iterator_da(DynamicArray *const da, DynamicArrayIterator **const output)
{
	int err_validate = _validate_da(da);
	if (err_validate)
		return err_validate;

	if (output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	DynamicArrayIterator *itr = (DynamicArrayIterator *)malloc(sizeof(DynamicArrayIterator));
	if (itr == NULL)
		return GLX_ERR_MEMORY_ALLOCATION;

	itr->da = da;
	itr->current_index = 0;
	*output = itr;

	return GLX_SUCCESS;
}

enum GalxlibError free_iterator_da(DynamicArrayIterator **itr)
{
	if (itr == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (*itr == NULL)
		return GLX_SUCCESS;

	free(*itr);
	*itr = NULL;

	return GLX_SUCCESS;
}

enum GalxlibError has_next_dai(const DynamicArrayIterator *const itr, int *const output)
{
	if (itr == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (itr->current_index + 1 <= itr->da->count)
		*output = 1;
	else
		*output = 0;

	return GLX_SUCCESS;
}

enum GalxlibError next_int_dai(DynamicArrayIterator *const itr, int *const output)
{
	if (itr == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (itr->da->type != DA_INT)
		return GLX_ERR_TYPE_MISMATCH;

	int output_has_next = 0;
	int err_has_next = has_next_dai(itr, &output_has_next);
	if (err_has_next)
		return err_has_next;
	if (output_has_next == 0)
		return GLX_ERR_INDEX_OUT_OF_BOUNDS;

	*output = (int)((itr->da->int_arr)[(itr->current_index)++]);

	return GLX_SUCCESS;
}

enum GalxlibError next_char_dai(DynamicArrayIterator *const itr, char *const output)
{
	if (itr == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (itr->da->type != DA_CHAR)
		return GLX_ERR_TYPE_MISMATCH;

	int output_has_next = 0;
	int err_has_next = has_next_dai(itr, &output_has_next);
	if (err_has_next)
		return err_has_next;
	if (output_has_next == 0)
		return GLX_ERR_INDEX_OUT_OF_BOUNDS;

	*output = (char)((itr->da->char_arr)[(itr->current_index)++]);

	return GLX_SUCCESS;
}

enum GalxlibError next_float_dai(DynamicArrayIterator *const itr, float *const output)
{
	if (itr == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (itr->da->type != DA_FLOAT)
		return GLX_ERR_TYPE_MISMATCH;

	int output_has_next = 0;
	int err_has_next = has_next_dai(itr, &output_has_next);
	if (err_has_next)
		return err_has_next;
	if (output_has_next == 0)
		return GLX_ERR_INDEX_OUT_OF_BOUNDS;

	*output = (float)((itr->da->float_arr)[(itr->current_index)++]);

	return GLX_SUCCESS;
}

enum GalxlibError next_double_dai(DynamicArrayIterator *const itr, double *const output)
{
	if (itr == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (itr->da->type != DA_DOUBLE)
		return GLX_ERR_TYPE_MISMATCH;

	int output_has_next = 0;
	int err_has_next = has_next_dai(itr, &output_has_next);
	if (err_has_next)
		return err_has_next;
	if (output_has_next == 0)
		return GLX_ERR_INDEX_OUT_OF_BOUNDS;

	*output = (double)((itr->da->double_arr)[(itr->current_index)++]);

	return GLX_SUCCESS;
}

enum GalxlibError next_ptr_dai(DynamicArrayIterator *const itr, void **const output)
{
	if (itr == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (itr->da->type != DA_PTR)
		return GLX_ERR_TYPE_MISMATCH;

	int output_has_next = 0;
	int err_has_next = has_next_dai(itr, &output_has_next);
	if (err_has_next)
		return err_has_next;
	if (output_has_next == 0)
		return GLX_ERR_INDEX_OUT_OF_BOUNDS;

	*output = (void *)((itr->da->void_arr)[(itr->current_index)++]);

	return GLX_SUCCESS;
}