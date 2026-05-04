#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../include/dynamic_array.h"
#include "../../include/gstring.h"
#include "../include/dynamic_array.h"

static void increment_int(void *ptr)
{
	int *v = (int *)ptr;
	(*v)++;
}

static int is_even(void *ptr)
{
	int *v = (int *)ptr;
	return (*v % 2) == 0;
}

static int equals_five(void *ptr)
{
	int *v = (int *)ptr;
	return (*v == 5);
}

static int greater_than_three(void *ptr)
{
	int *v = (int *)ptr;
	return (*v > 3);
}

void dynamicArrayTest()
{
	puts("################## Test: DynamicArray ##################");
	printf("Running DynamicArray tests...\n");

	// new_dynamic_array
	DynamicArray *da = NULL;
	int err_da = new_dynamic_array(INT, &da);
	assert(da != NULL);
	assert(da->count == 0);

	DynamicArray *invalid = NULL;
	int err_da_invalid = new_dynamic_array((enum DynamicArrayType)999, &invalid);
	assert(err_da_invalid == DA_ERR_TYPE_UNKNOWN);
	assert(invalid == NULL);

	// push_int_da
	assert(push_int_da(NULL, 1) == DA_ERR_NULL_ARGUMENT);

	for (int i = 0; i < 10; i++)
	{
		assert(push_int_da(da, i) == DA_SUCCESS);
	}
	assert(da->count == 10);

	DynamicArray *char_da = NULL;
	int err_da_char = new_dynamic_array(CHAR, &char_da);
	assert(err_da_char == DA_SUCCESS);
	assert(push_int_da(char_da, 5) == DA_ERR_TYPE_MISMATCH);

	// unshift_int_da
	assert(unshift_int_da(da, 100) == DA_SUCCESS);
	assert(da->count == 11);

	int tmp;
	assert(shift_int_da(da, &tmp) == DA_SUCCESS);
	assert(tmp == 100);

	// pop_int_da
	int val;
	assert(pop_int_da(NULL, &val) == DA_ERR_NULL_ARGUMENT);

	size_t prev_count = da->count;
	assert(pop_int_da(da, &val) == DA_SUCCESS);
	assert(da->count == prev_count - 1);

	DynamicArray *empty = NULL;
	int err_da_empty = new_dynamic_array(INT, &empty);
	assert(err_da_empty == DA_SUCCESS);
	assert(pop_int_da(empty, &val) == DA_ARRAY_EMPTY);

	// shift_int_da
	assert(shift_int_da(NULL, &val) == DA_ERR_NULL_ARGUMENT);
	assert(shift_int_da(empty, &val) == DA_ARRAY_EMPTY);

	// push_int_da
	push_int_da(da, 1);
	push_int_da(da, 2);
	push_int_da(da, 3);

	size_t before = da->count;
	assert(remove_at_da(da, 1) == DA_SUCCESS);
	assert(da->count == before - 1);

	// remove_first_da
	int target = 3;
	int rf = remove_first_da(da, &target);
	assert(rf == 0 || rf == -1);

	// apply_at_da
	assert(apply_at_da(NULL, 0, increment_int) == DA_ERR_NULL_ARGUMENT);
	assert(apply_at_da(da, 0, NULL) == DA_ERR_NULL_ARGUMENT);

	push_int_da(da, 1);
	assert(apply_at_da(da, 0, increment_int) == DA_SUCCESS);

	// process_da
	assert(process_da(NULL, increment_int) == DA_ERR_NULL_ARGUMENT);
	assert(process_da(da, NULL) == DA_ERR_NULL_ARGUMENT);
	assert(process_da(da, increment_int) == DA_SUCCESS);

	// filter_da
	assert(filter_da(NULL, is_even) == DA_ERR_NULL_ARGUMENT);
	assert(filter_da(da, NULL) == DA_ERR_NULL_ARGUMENT);
	assert(filter_da(da, is_even) == DA_SUCCESS);

	// at_da
	void *out = NULL;
	assert(at_da(NULL, 0, &out) == DA_ERR_NULL_ARGUMENT);

	if (da->count > 0)
	{
		assert(at_da(da, 0, &out) == DA_SUCCESS);
		assert(out != NULL);
	}

	assert(at_da(da, 9999, &out) == DA_ERR_INDEX_OUT_OF_BOUNDS);

	// find_da
	push_int_da(da, 5);

	void *found = NULL;
	assert(find_da(NULL, &found, equals_five) == DA_ERR_NULL_ARGUMENT);
	assert(find_da(da, NULL, equals_five) == DA_ERR_NULL_ARGUMENT);
	assert(find_da(da, &found, NULL) == DA_ERR_NULL_ARGUMENT);

	int res = find_da(da, &found, equals_five);
	assert(res == DA_SUCCESS || res == DA_ITEM_NOT_FOUND);

	// find_last_da
	res = find_last_da(da, &found, equals_five);
	assert(res == DA_SUCCESS || res == DA_ITEM_NOT_FOUND);

	// find_index_da
	size_t idx;
	res = find_index_da(da, &idx, greater_than_three);
	assert(res == DA_SUCCESS || res == DA_ITEM_NOT_FOUND);

	res = find_last_index_da(da, &idx, greater_than_three);
	assert(res == DA_SUCCESS || res == DA_ITEM_NOT_FOUND);

	// index_of_da
	int value = 5;
	res = index_of_da(da, &idx, &value);
	assert(res == DA_SUCCESS || res == DA_ITEM_NOT_FOUND);

	assert(index_of_da(NULL, &idx, &value) == DA_ERR_NULL_ARGUMENT);
	assert(index_of_da(da, NULL, &value) == DA_ERR_NULL_ARGUMENT);
	assert(index_of_da(da, &idx, NULL) == DA_ERR_NULL_ARGUMENT);

	// DynamicArrayIterator
	DynamicArray *da_itr = NULL;
	int err_da_itr = new_dynamic_array(INT, &da_itr);
	assert(err_da_itr == 0);
	for (size_t i = 0; i < 5; ++i)
		push_int_da(da_itr, i);
	assert(da_itr->count == 5);
	DynamicArrayIterator *itr = NULL;
	int err_itr_init = new_iterator_da(da_itr, &itr);
	assert(err_itr_init == 0);
	assert(itr->da == da_itr);
	assert(itr->current_index == 0);

	int da_has_next = 0;
	int err_has_next = has_next_dai(NULL, &da_has_next);
	assert(err_has_next == DA_ERR_NULL_ARGUMENT);
	err_has_next = has_next_dai(itr, NULL);
	assert(err_has_next == DA_ERR_NULL_ARGUMENT);
	err_has_next = has_next_dai(itr, &da_has_next);
	assert(err_has_next == 0);

	int err_next = 0;
	err_next = next_int_dai(NULL, &value);
	assert(err_next == DA_ERR_NULL_ARGUMENT);
	err_next = next_int_dai(itr, NULL);
	assert(err_next == DA_ERR_NULL_ARGUMENT);

	int itr_value_tracker = 0;
	while (da_has_next)
	{
		int value = -1;
		err_next = next_int_dai(itr, &value);
		assert(err_next == 0);
		assert(value == itr_value_tracker);
		itr_value_tracker++;
		err_has_next = has_next_dai(itr, &da_has_next);
		assert(err_has_next == 0);
	}
	// checking iteration boundaries
	err_has_next = has_next_dai(itr, &da_has_next);
	assert(err_has_next == 0);
	assert(da_has_next == 0);
	err_next = next_int_dai(itr, &value);
	assert(err_next == DA_ERR_INDEX_OUT_OF_BOUNDS);

	// Cleanup
	int err_f_da = free_dynamic_array(da);
	assert(err_f_da == 0);
	int err_f_da_c = free_dynamic_array(char_da);
	assert(err_f_da_c == 0);
	int err_f_empty = free_dynamic_array(empty);
	assert(err_f_empty == 0);
	int err_f_itr = free_iterator_da(itr);
	assert(err_f_itr == 0);

	printf(ANSI_COLOR_GREEN "All DynamicArray tests passed.\n" ANSI_COLOR_RESET);
	puts("################## Test: DynamicArray ##################");
}