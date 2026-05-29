#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../include/dynamic_array.h"
#include "../../include/gstring.h"
#include "../include/dynamic_array.h"

DynamicArray *da_i = NULL;
DynamicArray *da_c = NULL;
DynamicArray *da_f = NULL;
DynamicArray *da_d = NULL;
DynamicArray *da_p = NULL;
DynamicArray *da_invalid_type = NULL;

typedef struct Da_Person
{
	char *name;
	int age;
} Da_Person;

static void _person_destructor(void **ptr)
{
	assert(ptr != NULL);
	if (*ptr == NULL)
		return;

	free(*ptr);
	*ptr = NULL;
}

static void _test_activate_destructor_da()
{
	Da_Person *p1 = (Da_Person *)malloc(sizeof(Da_Person));
	Da_Person *p2 = (Da_Person *)malloc(sizeof(Da_Person));
	p1->name = "One";
	p2->name = "Two";
	p1->age = 1;
	p2->age = 2;

	DynamicArray *da = NULL;
	assert(activate_destructor_da(da) == DA_ERR_NULL_ARGUMENT);
	assert(activate_destructor_da(da_i) == DA_ERR_TYPE_MISMATCH);
	assert(activate_destructor_da(da_c) == DA_ERR_TYPE_MISMATCH);
	assert(activate_destructor_da(da_f) == DA_ERR_TYPE_MISMATCH);
	assert(activate_destructor_da(da_d) == DA_ERR_TYPE_MISMATCH);
	assert(activate_destructor_da(da_invalid_type) == DA_ERR_TYPE_MISMATCH);

	assert(new_dynamic_array(DA_PTR, &da) == DA_SUCCESS);
	assert(da != NULL);
	assert(push_ptr_da(da, p1) == DA_SUCCESS);
	assert(push_ptr_da(da, p2) == DA_SUCCESS);
	assert((da->void_arr)[0] == p1);
	assert((da->void_arr)[1] == p2);

	assert(activate_destructor_da(da) == DA_ERR_NULL_DESTRUCTOR);
	assert((da->void_arr)[0] == p1);
	assert((da->void_arr)[1] == p2);

	da->destructor = _person_destructor;
	assert(activate_destructor_da(da) == DA_SUCCESS);
	assert(da != NULL);
	assert((da->void_arr)[0] == NULL);
	assert((da->void_arr)[1] == NULL);
}

static void _test_free()
{
	assert(free_dynamic_array(NULL) == DA_ERR_NULL_ARGUMENT);
	assert(free_dynamic_array(&da_invalid_type) == DA_ERR_TYPE_UNKNOWN);

	DynamicArray *da = NULL;

	/* Free NULL DynamicArray pointer */
	assert(free_dynamic_array(&da) == DA_SUCCESS);
	assert(da == NULL);

	/* ==================== INT ==================== */

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);
	assert(da != NULL);

	for (int i = 0; i < 10; i++)
	{
		assert(push_int_da(da, i) == DA_SUCCESS);
	}

	assert(da->count == 10);

	assert(free_dynamic_array(&da) == DA_SUCCESS);
	assert(da == NULL);

	/* ==================== CHAR ==================== */

	assert(new_dynamic_array(DA_CHAR, &da) == DA_SUCCESS);
	assert(da != NULL);

	for (int i = 0; i < 10; i++)
	{
		assert(push_char_da(da, 'a' + i) == DA_SUCCESS);
	}

	assert(da->count == 10);

	assert(free_dynamic_array(&da) == DA_SUCCESS);
	assert(da == NULL);

	/* ==================== FLOAT ==================== */

	assert(new_dynamic_array(DA_FLOAT, &da) == DA_SUCCESS);
	assert(da != NULL);

	for (int i = 0; i < 10; i++)
	{
		assert(push_float_da(da, (float)i) == DA_SUCCESS);
	}

	assert(da->count == 10);

	assert(free_dynamic_array(&da) == DA_SUCCESS);
	assert(da == NULL);

	/* ==================== DOUBLE ==================== */

	assert(new_dynamic_array(DA_DOUBLE, &da) == DA_SUCCESS);
	assert(da != NULL);

	for (int i = 0; i < 10; i++)
	{
		assert(push_double_da(da, (double)i) == DA_SUCCESS);
	}

	assert(da->count == 10);

	assert(free_dynamic_array(&da) == DA_SUCCESS);
	assert(da == NULL);

	/* ==================== PTR ==================== */

	int values[10];

	assert(new_dynamic_array(DA_PTR, &da) == DA_SUCCESS);
	assert(da != NULL);

	for (int i = 0; i < 10; i++)
	{
		values[i] = i;
		assert(push_ptr_da(da, &values[i]) == DA_SUCCESS);
	}

	assert(da->count == 10);

	assert(free_dynamic_array(&da) == DA_SUCCESS);
	assert(da == NULL);

	// free_dynamic_array_d
	assert(free_dynamic_array_d(&da_invalid_type) == DA_ERR_TYPE_UNKNOWN);
	assert(free_dynamic_array_d(&da_p) == DA_ERR_NULL_DESTRUCTOR);

	assert(new_dynamic_array_d(DA_PTR, _person_destructor, &da) == DA_SUCCESS);
	assert(da != NULL);

	for (int i = 0; i < 2; i++)
	{
		Da_Person *p = malloc(sizeof(Da_Person));
		assert(p != NULL);
		p->name = "Name";
		p->age = i;
		assert(push_ptr_da(da, p) == DA_SUCCESS);
	}

	assert(da->count == 2);

	assert(free_dynamic_array_d(&da) == DA_SUCCESS);
	assert(da == NULL);
}

static void _test_push()
{
	int tmp = 0;
	assert(push_int_da(NULL, 1) == DA_ERR_NULL_ARGUMENT);
	assert(push_int_da(da_c, 1) == DA_ERR_TYPE_MISMATCH);
	assert(push_int_da(da_f, 1) == DA_ERR_TYPE_MISMATCH);
	assert(push_int_da(da_d, 1) == DA_ERR_TYPE_MISMATCH);
	assert(push_int_da(da_p, 1) == DA_ERR_TYPE_MISMATCH);

	assert(push_char_da(NULL, 1) == DA_ERR_NULL_ARGUMENT);
	assert(push_char_da(da_i, 1) == DA_ERR_TYPE_MISMATCH);
	assert(push_char_da(da_f, 1) == DA_ERR_TYPE_MISMATCH);
	assert(push_char_da(da_d, 1) == DA_ERR_TYPE_MISMATCH);
	assert(push_char_da(da_p, 1) == DA_ERR_TYPE_MISMATCH);

	assert(push_float_da(NULL, 1.0) == DA_ERR_NULL_ARGUMENT);
	assert(push_float_da(da_i, 1.0) == DA_ERR_TYPE_MISMATCH);
	assert(push_float_da(da_c, 1.0) == DA_ERR_TYPE_MISMATCH);
	assert(push_float_da(da_d, 1.0) == DA_ERR_TYPE_MISMATCH);
	assert(push_float_da(da_p, 1.0) == DA_ERR_TYPE_MISMATCH);

	assert(push_double_da(NULL, 1.0) == DA_ERR_NULL_ARGUMENT);
	assert(push_double_da(da_i, 1.0) == DA_ERR_TYPE_MISMATCH);
	assert(push_double_da(da_c, 1.0) == DA_ERR_TYPE_MISMATCH);
	assert(push_double_da(da_f, 1.0) == DA_ERR_TYPE_MISMATCH);
	assert(push_double_da(da_p, 1.0) == DA_ERR_TYPE_MISMATCH);

	assert(push_ptr_da(NULL, &tmp) == DA_ERR_NULL_ARGUMENT);
	assert(push_ptr_da(da_i, &tmp) == DA_ERR_TYPE_MISMATCH);
	assert(push_ptr_da(da_c, &tmp) == DA_ERR_TYPE_MISMATCH);
	assert(push_ptr_da(da_f, &tmp) == DA_ERR_TYPE_MISMATCH);
	assert(push_ptr_da(da_d, &tmp) == DA_ERR_TYPE_MISMATCH);

	DynamicArray *da_int = NULL;
	assert(new_dynamic_array(DA_INT, &da_int) == DA_SUCCESS);
	for (int i = 0; i < 10; i++)
	{
		assert(push_int_da(da_int, i) == DA_SUCCESS);
	}
	assert(da_int->count == 10);

	DynamicArray *da_char = NULL;
	assert(new_dynamic_array(DA_CHAR, &da_char) == DA_SUCCESS);
	for (int i = 0; i < 10; i++)
	{
		assert(push_char_da(da_char, 'c') == DA_SUCCESS);
	}
	assert(da_char->count == 10);

	DynamicArray *da_float = NULL;
	assert(new_dynamic_array(DA_FLOAT, &da_float) == DA_SUCCESS);
	for (int i = 0; i < 10; i++)
	{
		assert(push_float_da(da_float, (float)i) == DA_SUCCESS);
	}
	assert(da_float->count == 10);

	DynamicArray *da_double = NULL;
	assert(new_dynamic_array(DA_DOUBLE, &da_double) == DA_SUCCESS);
	for (int i = 0; i < 10; i++)
	{
		assert(push_double_da(da_double, (double)i) == DA_SUCCESS);
	}
	assert(da_double->count == 10);

	DynamicArray *da_ptr = NULL;
	assert(new_dynamic_array(DA_PTR, &da_ptr) == DA_SUCCESS);
	for (int i = 0; i < 10; i++)
	{
		assert(push_ptr_da(da_ptr, &tmp) == DA_SUCCESS);
	}
	assert(da_ptr->count == 10);

	assert(free_dynamic_array(&da_int) == DA_SUCCESS);
	assert(free_dynamic_array(&da_char) == DA_SUCCESS);
	assert(free_dynamic_array(&da_float) == DA_SUCCESS);
	assert(free_dynamic_array(&da_double) == DA_SUCCESS);
	assert(free_dynamic_array(&da_ptr) == DA_SUCCESS);
}

static void _test_unshift()
{
	int tmp = 0;

	assert(unshift_int_da(NULL, 1) == DA_ERR_NULL_ARGUMENT);
	assert(unshift_int_da(da_c, 1) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_int_da(da_f, 1) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_int_da(da_d, 1) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_int_da(da_p, 1) == DA_ERR_TYPE_MISMATCH);

	assert(unshift_char_da(NULL, 'a') == DA_ERR_NULL_ARGUMENT);
	assert(unshift_char_da(da_i, 'a') == DA_ERR_TYPE_MISMATCH);
	assert(unshift_char_da(da_f, 'a') == DA_ERR_TYPE_MISMATCH);
	assert(unshift_char_da(da_d, 'a') == DA_ERR_TYPE_MISMATCH);
	assert(unshift_char_da(da_p, 'a') == DA_ERR_TYPE_MISMATCH);

	assert(unshift_float_da(NULL, 1.0f) == DA_ERR_NULL_ARGUMENT);
	assert(unshift_float_da(da_i, 1.0f) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_float_da(da_c, 1.0f) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_float_da(da_d, 1.0f) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_float_da(da_p, 1.0f) == DA_ERR_TYPE_MISMATCH);

	assert(unshift_double_da(NULL, 1.0) == DA_ERR_NULL_ARGUMENT);
	assert(unshift_double_da(da_i, 1.0) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_double_da(da_c, 1.0) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_double_da(da_f, 1.0) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_double_da(da_p, 1.0) == DA_ERR_TYPE_MISMATCH);

	assert(unshift_ptr_da(NULL, &tmp) == DA_ERR_NULL_ARGUMENT);
	assert(unshift_ptr_da(da_i, &tmp) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_ptr_da(da_c, &tmp) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_ptr_da(da_f, &tmp) == DA_ERR_TYPE_MISMATCH);
	assert(unshift_ptr_da(da_d, &tmp) == DA_ERR_TYPE_MISMATCH);

	DynamicArray *da_int = NULL;
	assert(new_dynamic_array(DA_INT, &da_int) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(unshift_int_da(da_int, i) == DA_SUCCESS);
	}

	assert(da_int->count == 10);

	for (int i = 0; i < 10; i++)
	{
		assert(da_int->int_arr[i] == 9 - i);
	}

	DynamicArray *da_char = NULL;
	assert(new_dynamic_array(DA_CHAR, &da_char) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(unshift_char_da(da_char, 'a' + i) == DA_SUCCESS);
	}

	assert(da_char->count == 10);

	for (int i = 0; i < 10; i++)
	{
		assert(da_char->char_arr[i] == ('a' + (9 - i)));
	}

	DynamicArray *da_float = NULL;
	assert(new_dynamic_array(DA_FLOAT, &da_float) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(unshift_float_da(da_float, (float)i) == DA_SUCCESS);
	}

	assert(da_float->count == 10);

	for (int i = 0; i < 10; i++)
	{
		assert(da_float->float_arr[i] == (float)(9 - i));
	}

	DynamicArray *da_double = NULL;
	assert(new_dynamic_array(DA_DOUBLE, &da_double) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(unshift_double_da(da_double, (double)i) == DA_SUCCESS);
	}

	assert(da_double->count == 10);

	for (int i = 0; i < 10; i++)
	{
		assert(da_double->double_arr[i] == (double)(9 - i));
	}

	DynamicArray *da_ptr = NULL;
	assert(new_dynamic_array(DA_PTR, &da_ptr) == DA_SUCCESS);

	void *ptrs[10];

	for (int i = 0; i < 10; i++)
	{
		ptrs[i] = &tmp;
		assert(unshift_ptr_da(da_ptr, ptrs[i]) == DA_SUCCESS);
	}

	assert(da_ptr->count == 10);

	for (int i = 0; i < 10; i++)
	{
		assert(da_ptr->void_arr[i] == ptrs[9 - i]);
	}

	assert(free_dynamic_array(&da_int) == DA_SUCCESS);
	assert(free_dynamic_array(&da_char) == DA_SUCCESS);
	assert(free_dynamic_array(&da_float) == DA_SUCCESS);
	assert(free_dynamic_array(&da_double) == DA_SUCCESS);
	assert(free_dynamic_array(&da_ptr) == DA_SUCCESS);
}

static void _test_pop()
{
	int int_out = 0;
	char char_out = '\0';
	float float_out = 0.0f;
	double double_out = 0.0;
	void *ptr_out = NULL;

	int tmp = 123;

	assert(pop_int_da(NULL, &int_out) == DA_ERR_NULL_ARGUMENT);
	assert(pop_int_da(da_c, &int_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_int_da(da_f, &int_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_int_da(da_d, &int_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_int_da(da_p, &int_out) == DA_ERR_TYPE_MISMATCH);

	assert(pop_char_da(NULL, &char_out) == DA_ERR_NULL_ARGUMENT);
	assert(pop_char_da(da_i, &char_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_char_da(da_f, &char_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_char_da(da_d, &char_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_char_da(da_p, &char_out) == DA_ERR_TYPE_MISMATCH);

	assert(pop_float_da(NULL, &float_out) == DA_ERR_NULL_ARGUMENT);
	assert(pop_float_da(da_i, &float_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_float_da(da_c, &float_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_float_da(da_d, &float_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_float_da(da_p, &float_out) == DA_ERR_TYPE_MISMATCH);

	assert(pop_double_da(NULL, &double_out) == DA_ERR_NULL_ARGUMENT);
	assert(pop_double_da(da_i, &double_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_double_da(da_c, &double_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_double_da(da_f, &double_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_double_da(da_p, &double_out) == DA_ERR_TYPE_MISMATCH);

	assert(pop_ptr_da(NULL, &ptr_out) == DA_ERR_NULL_ARGUMENT);
	assert(pop_ptr_da(da_i, &ptr_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_ptr_da(da_c, &ptr_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_ptr_da(da_f, &ptr_out) == DA_ERR_TYPE_MISMATCH);
	assert(pop_ptr_da(da_d, &ptr_out) == DA_ERR_TYPE_MISMATCH);

	DynamicArray *da_int = NULL;
	assert(new_dynamic_array(DA_INT, &da_int) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(push_int_da(da_int, i) == DA_SUCCESS);
	}

	for (int i = 9; i >= 0; i--)
	{
		assert(pop_int_da(da_int, &int_out) == DA_SUCCESS);
		assert(int_out == i);
	}

	assert(da_int->count == 0);
	assert(pop_int_da(da_int, &int_out) == DA_EMPTY);

	DynamicArray *da_char = NULL;
	assert(new_dynamic_array(DA_CHAR, &da_char) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(push_char_da(da_char, 'a' + i) == DA_SUCCESS);
	}

	for (int i = 9; i >= 0; i--)
	{
		assert(pop_char_da(da_char, &char_out) == DA_SUCCESS);
		assert(char_out == ('a' + i));
	}

	assert(da_char->count == 0);
	assert(pop_char_da(da_char, &char_out) == DA_EMPTY);

	DynamicArray *da_float = NULL;
	assert(new_dynamic_array(DA_FLOAT, &da_float) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(push_float_da(da_float, (float)i) == DA_SUCCESS);
	}

	for (int i = 9; i >= 0; i--)
	{
		assert(pop_float_da(da_float, &float_out) == DA_SUCCESS);
		assert(float_out == (float)i);
	}

	assert(da_float->count == 0);
	assert(pop_float_da(da_float, &float_out) == DA_EMPTY);

	DynamicArray *da_double = NULL;
	assert(new_dynamic_array(DA_DOUBLE, &da_double) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(push_double_da(da_double, (double)i) == DA_SUCCESS);
	}

	for (int i = 9; i >= 0; i--)
	{
		assert(pop_double_da(da_double, &double_out) == DA_SUCCESS);
		assert(double_out == (double)i);
	}

	assert(da_double->count == 0);
	assert(pop_double_da(da_double, &double_out) == DA_EMPTY);

	DynamicArray *da_ptr = NULL;
	assert(new_dynamic_array(DA_PTR, &da_ptr) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(push_ptr_da(da_ptr, &tmp) == DA_SUCCESS);
	}

	for (int i = 9; i >= 0; i--)
	{
		assert(pop_ptr_da(da_ptr, &ptr_out) == DA_SUCCESS);
		assert(ptr_out == &tmp);
	}

	assert(da_ptr->count == 0);
	assert(pop_ptr_da(da_ptr, &ptr_out) == DA_EMPTY);

	assert(free_dynamic_array(&da_int) == DA_SUCCESS);
	assert(free_dynamic_array(&da_char) == DA_SUCCESS);
	assert(free_dynamic_array(&da_float) == DA_SUCCESS);
	assert(free_dynamic_array(&da_double) == DA_SUCCESS);
	assert(free_dynamic_array(&da_ptr) == DA_SUCCESS);
}

static void _test_shift()
{
	int int_out = 0;
	char char_out = '\0';
	float float_out = 0.0f;
	double double_out = 0.0;
	void *ptr_out = NULL;

	int tmp_values[10];

	assert(shift_int_da(NULL, &int_out) == DA_ERR_NULL_ARGUMENT);
	assert(shift_int_da(da_c, &int_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_int_da(da_f, &int_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_int_da(da_d, &int_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_int_da(da_p, &int_out) == DA_ERR_TYPE_MISMATCH);

	assert(shift_char_da(NULL, &char_out) == DA_ERR_NULL_ARGUMENT);
	assert(shift_char_da(da_i, &char_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_char_da(da_f, &char_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_char_da(da_d, &char_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_char_da(da_p, &char_out) == DA_ERR_TYPE_MISMATCH);

	assert(shift_float_da(NULL, &float_out) == DA_ERR_NULL_ARGUMENT);
	assert(shift_float_da(da_i, &float_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_float_da(da_c, &float_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_float_da(da_d, &float_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_float_da(da_p, &float_out) == DA_ERR_TYPE_MISMATCH);

	assert(shift_double_da(NULL, &double_out) == DA_ERR_NULL_ARGUMENT);
	assert(shift_double_da(da_i, &double_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_double_da(da_c, &double_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_double_da(da_f, &double_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_double_da(da_p, &double_out) == DA_ERR_TYPE_MISMATCH);

	assert(shift_ptr_da(NULL, &ptr_out) == DA_ERR_NULL_ARGUMENT);
	assert(shift_ptr_da(da_i, &ptr_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_ptr_da(da_c, &ptr_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_ptr_da(da_f, &ptr_out) == DA_ERR_TYPE_MISMATCH);
	assert(shift_ptr_da(da_d, &ptr_out) == DA_ERR_TYPE_MISMATCH);

	DynamicArray *da_int = NULL;
	assert(new_dynamic_array(DA_INT, &da_int) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(push_int_da(da_int, i) == DA_SUCCESS);
	}

	for (int i = 0; i < 10; i++)
	{
		assert(shift_int_da(da_int, &int_out) == DA_SUCCESS);
		assert(int_out == i);
	}

	assert(da_int->count == 0);
	assert(shift_int_da(da_int, &int_out) == DA_EMPTY);

	DynamicArray *da_char = NULL;
	assert(new_dynamic_array(DA_CHAR, &da_char) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(push_char_da(da_char, 'a' + i) == DA_SUCCESS);
	}

	for (int i = 0; i < 10; i++)
	{
		assert(shift_char_da(da_char, &char_out) == DA_SUCCESS);
		assert(char_out == ('a' + i));
	}

	assert(da_char->count == 0);
	assert(shift_char_da(da_char, &char_out) == DA_EMPTY);

	DynamicArray *da_float = NULL;
	assert(new_dynamic_array(DA_FLOAT, &da_float) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(push_float_da(da_float, (float)i) == DA_SUCCESS);
	}

	for (int i = 0; i < 10; i++)
	{
		assert(shift_float_da(da_float, &float_out) == DA_SUCCESS);
		assert(float_out == (float)i);
	}

	assert(da_float->count == 0);
	assert(shift_float_da(da_float, &float_out) == DA_EMPTY);

	DynamicArray *da_double = NULL;
	assert(new_dynamic_array(DA_DOUBLE, &da_double) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		assert(push_double_da(da_double, (double)i) == DA_SUCCESS);
	}

	for (int i = 0; i < 10; i++)
	{
		assert(shift_double_da(da_double, &double_out) == DA_SUCCESS);
		assert(double_out == (double)i);
	}

	assert(da_double->count == 0);
	assert(shift_double_da(da_double, &double_out) == DA_EMPTY);

	DynamicArray *da_ptr = NULL;
	assert(new_dynamic_array(DA_PTR, &da_ptr) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		tmp_values[i] = i;
		assert(push_ptr_da(da_ptr, &tmp_values[i]) == DA_SUCCESS);
	}

	for (int i = 0; i < 10; i++)
	{
		assert(shift_ptr_da(da_ptr, &ptr_out) == DA_SUCCESS);
		assert(ptr_out == &tmp_values[i]);
	}

	assert(da_ptr->count == 0);
	assert(shift_ptr_da(da_ptr, &ptr_out) == DA_EMPTY);

	assert(free_dynamic_array(&da_int) == DA_SUCCESS);
	assert(free_dynamic_array(&da_char) == DA_SUCCESS);
	assert(free_dynamic_array(&da_float) == DA_SUCCESS);
	assert(free_dynamic_array(&da_double) == DA_SUCCESS);
	assert(free_dynamic_array(&da_ptr) == DA_SUCCESS);
}

static int destructor_counter = 0;

static void _test_destructor(void **ptr)
{
	if (ptr != NULL && *ptr != NULL)
	{
		destructor_counter++;
		free(*ptr);
		*ptr = NULL;
	}
}

static void _test_apply_destructor_da()
{
	destructor_counter = 0;

	/* ==================== NULL ARGUMENTS ==================== */

	assert(apply_destructor_da(NULL, _test_destructor) == DA_ERR_NULL_ARGUMENT);

	DynamicArray *da = NULL;
	assert(new_dynamic_array(DA_PTR, &da) == DA_SUCCESS);

	assert(apply_destructor_da(da, NULL) == DA_ERR_NULL_ARGUMENT);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== TYPE MISMATCH ==================== */

	assert(apply_destructor_da(da_i, _test_destructor) == DA_ERR_TYPE_MISMATCH);
	assert(apply_destructor_da(da_c, _test_destructor) == DA_ERR_TYPE_MISMATCH);
	assert(apply_destructor_da(da_f, _test_destructor) == DA_ERR_TYPE_MISMATCH);
	assert(apply_destructor_da(da_d, _test_destructor) == DA_ERR_TYPE_MISMATCH);
	assert(apply_destructor_da(da_invalid_type, _test_destructor) == DA_ERR_TYPE_MISMATCH);

	/* ==================== SUCCESS CASE ==================== */

	assert(new_dynamic_array(DA_PTR, &da) == DA_SUCCESS);

	for (int i = 0; i < 10; i++)
	{
		int *value = malloc(sizeof(int));

		assert(value != NULL);

		*value = i;

		assert(push_ptr_da(da, value) == DA_SUCCESS);
	}

	assert(da->count == 10);

	assert(apply_destructor_da(da, _test_destructor) == DA_SUCCESS);

	assert(destructor_counter == 10);

	for (size_t i = 0; i < da->count; i++)
	{
		assert(da->void_arr[i] == NULL);
	}

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== EMPTY ARRAY ==================== */

	destructor_counter = 0;

	assert(new_dynamic_array(DA_PTR, &da) == DA_SUCCESS);

	assert(apply_destructor_da(da, _test_destructor) == DA_SUCCESS);

	assert(destructor_counter == 0);

	assert(free_dynamic_array(&da) == DA_SUCCESS);
}

static void _test_remove_at()
{
	/* ==================== NULL ARGUMENT ==================== */

	assert(remove_at_da(NULL, 0) == DA_ERR_NULL_ARGUMENT);

	/* ==================== EMPTY ARRAY ==================== */

	DynamicArray *da = NULL;

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(remove_at_da(da, 0) == DA_EMPTY);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== INDEX OUT OF BOUNDS ==================== */

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	for (int i = 0; i < 5; i++)
	{
		assert(push_int_da(da, i) == DA_SUCCESS);
	}

	assert(da->count == 5);

	assert(remove_at_da(da, 5) == DA_ERR_INDEX_OUT_OF_BOUNDS);
	assert(remove_at_da(da, 100) == DA_ERR_INDEX_OUT_OF_BOUNDS);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - INT ==================== */

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	for (int i = 0; i < 5; i++)
	{
		assert(push_int_da(da, i) == DA_SUCCESS);
	}

	assert(remove_at_da(da, 2) == DA_SUCCESS);

	assert(da->count == 4);

	assert(da->int_arr[0] == 0);
	assert(da->int_arr[1] == 1);
	assert(da->int_arr[2] == 3);
	assert(da->int_arr[3] == 4);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - CHAR ==================== */

	assert(new_dynamic_array(DA_CHAR, &da) == DA_SUCCESS);

	assert(push_char_da(da, 'a') == DA_SUCCESS);
	assert(push_char_da(da, 'b') == DA_SUCCESS);
	assert(push_char_da(da, 'c') == DA_SUCCESS);

	assert(remove_at_da(da, 1) == DA_SUCCESS);

	assert(da->count == 2);

	assert(da->char_arr[0] == 'a');
	assert(da->char_arr[1] == 'c');

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - FLOAT ==================== */

	assert(new_dynamic_array(DA_FLOAT, &da) == DA_SUCCESS);

	assert(push_float_da(da, 1.0f) == DA_SUCCESS);
	assert(push_float_da(da, 2.0f) == DA_SUCCESS);
	assert(push_float_da(da, 3.0f) == DA_SUCCESS);

	assert(remove_at_da(da, 0) == DA_SUCCESS);

	assert(da->count == 2);

	assert(da->float_arr[0] == 2.0f);
	assert(da->float_arr[1] == 3.0f);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - DOUBLE ==================== */

	assert(new_dynamic_array(DA_DOUBLE, &da) == DA_SUCCESS);

	assert(push_double_da(da, 1.0) == DA_SUCCESS);
	assert(push_double_da(da, 2.0) == DA_SUCCESS);
	assert(push_double_da(da, 3.0) == DA_SUCCESS);

	assert(remove_at_da(da, 2) == DA_SUCCESS);

	assert(da->count == 2);

	assert(da->double_arr[0] == 1.0);
	assert(da->double_arr[1] == 2.0);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - PTR ==================== */

	int a = 1;
	int b = 2;
	int c = 3;

	assert(new_dynamic_array(DA_PTR, &da) == DA_SUCCESS);

	assert(push_ptr_da(da, &a) == DA_SUCCESS);
	assert(push_ptr_da(da, &b) == DA_SUCCESS);
	assert(push_ptr_da(da, &c) == DA_SUCCESS);

	assert(remove_at_da(da, 1) == DA_SUCCESS);

	assert(da->count == 2);

	assert(da->void_arr[0] == &a);
	assert(da->void_arr[1] == &c);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== REMOVE LAST ELEMENT ==================== */

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(push_int_da(da, 10) == DA_SUCCESS);
	assert(push_int_da(da, 20) == DA_SUCCESS);

	assert(remove_at_da(da, 1) == DA_SUCCESS);

	assert(da->count == 1);
	assert(da->int_arr[0] == 10);

	assert(remove_at_da(da, 0) == DA_SUCCESS);

	assert(da->count == 0);

	assert(free_dynamic_array(&da) == DA_SUCCESS);
}

static void _test_remove_first()
{
	/* ==================== NULL ARGUMENTS ==================== */

	int value = 5;

	assert(remove_first_da(NULL, &value) == DA_ERR_NULL_ARGUMENT);

	DynamicArray *da = NULL;

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(remove_first_da(da, NULL) == DA_ERR_NULL_ARGUMENT);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== EMPTY ARRAY ==================== */

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(remove_first_da(da, &value) == DA_EMPTY);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== ITEM NOT FOUND ==================== */

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	for (int i = 0; i < 5; i++)
	{
		assert(push_int_da(da, i) == DA_SUCCESS);
	}

	value = 100;

	assert(remove_first_da(da, &value) == DA_ITEM_NOT_FOUND);

	assert(da->count == 5);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - INT ==================== */

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	for (int i = 0; i < 5; i++)
	{
		assert(push_int_da(da, i) == DA_SUCCESS);
	}

	value = 2;

	assert(remove_first_da(da, &value) == DA_SUCCESS);

	/*
		Array before:
		[0, 1, 2, 3, 4]

		Expected after removing 2:
		[0, 1, 3, 4]
	*/

	assert(da->count == 4);

	assert(da->int_arr[0] == 0);
	assert(da->int_arr[1] == 1);
	assert(da->int_arr[2] == 3);
	assert(da->int_arr[3] == 4);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - REMOVE FIRST MATCH ONLY ==================== */

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(push_int_da(da, 1) == DA_SUCCESS);
	assert(push_int_da(da, 2) == DA_SUCCESS);
	assert(push_int_da(da, 2) == DA_SUCCESS);
	assert(push_int_da(da, 3) == DA_SUCCESS);

	value = 2;

	assert(remove_first_da(da, &value) == DA_SUCCESS);

	/*
		Expected:
		[1, 2, 3]
	*/

	assert(da->count == 3);

	assert(da->int_arr[0] == 1);
	assert(da->int_arr[1] == 2);
	assert(da->int_arr[2] == 3);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - CHAR ==================== */

	char c = 'b';

	assert(new_dynamic_array(DA_CHAR, &da) == DA_SUCCESS);

	assert(push_char_da(da, 'a') == DA_SUCCESS);
	assert(push_char_da(da, 'b') == DA_SUCCESS);
	assert(push_char_da(da, 'c') == DA_SUCCESS);

	assert(remove_first_da(da, &c) == DA_SUCCESS);

	assert(da->count == 2);

	assert(da->char_arr[0] == 'a');
	assert(da->char_arr[1] == 'c');

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - FLOAT ==================== */

	float f = 2.0f;

	assert(new_dynamic_array(DA_FLOAT, &da) == DA_SUCCESS);

	assert(push_float_da(da, 1.0f) == DA_SUCCESS);
	assert(push_float_da(da, 2.0f) == DA_SUCCESS);
	assert(push_float_da(da, 3.0f) == DA_SUCCESS);

	assert(remove_first_da(da, &f) == DA_SUCCESS);

	assert(da->count == 2);

	assert(da->float_arr[0] == 1.0f);
	assert(da->float_arr[1] == 3.0f);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - DOUBLE ==================== */

	double d = 2.0;

	assert(new_dynamic_array(DA_DOUBLE, &da) == DA_SUCCESS);

	assert(push_double_da(da, 1.0) == DA_SUCCESS);
	assert(push_double_da(da, 2.0) == DA_SUCCESS);
	assert(push_double_da(da, 3.0) == DA_SUCCESS);

	assert(remove_first_da(da, &d) == DA_SUCCESS);

	assert(da->count == 2);

	assert(da->double_arr[0] == 1.0);
	assert(da->double_arr[1] == 3.0);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - PTR ==================== */

	int a = 1;
	int b = 2;
	int c2 = 3;

	void *target = &b;

	assert(new_dynamic_array(DA_PTR, &da) == DA_SUCCESS);

	assert(push_ptr_da(da, &a) == DA_SUCCESS);
	assert(push_ptr_da(da, &b) == DA_SUCCESS);
	assert(push_ptr_da(da, &c2) == DA_SUCCESS);

	assert(remove_first_da(da, target) == DA_SUCCESS);

	assert(da->count == 2);

	assert(da->void_arr[0] == &a);
	assert(da->void_arr[1] == &c2);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== REMOVE LAST ELEMENT ==================== */

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(push_int_da(da, 10) == DA_SUCCESS);

	value = 10;

	assert(remove_first_da(da, &value) == DA_SUCCESS);

	assert(da->count == 0);

	assert(free_dynamic_array(&da) == DA_SUCCESS);
}

static int worker_call_count = 0;

static void _increment_int(void *item_ptr)
{
	assert(item_ptr != NULL);

	int *value = (int *)item_ptr;

	(*value)++;

	worker_call_count++;
}

static void _uppercase_char(void *item_ptr)
{
	assert(item_ptr != NULL);

	char *c = (char *)item_ptr;

	if (*c >= 'a' && *c <= 'z')
	{
		*c = (char)(*c - ('a' - 'A'));
	}

	worker_call_count++;
}

static void _double_float(void *item_ptr)
{
	assert(item_ptr != NULL);

	float *f = (float *)item_ptr;

	*f *= 2.0f;

	worker_call_count++;
}

static void _double_double(void *item_ptr)
{
	assert(item_ptr != NULL);

	double *d = (double *)item_ptr;

	*d *= 2.0;

	worker_call_count++;
}

static void _increment_ptr_value(void *item_ptr)
{
	assert(item_ptr != NULL);

	int *value = (int *)item_ptr;

	(*value)++;

	worker_call_count++;
}

static void _test_apply_at()
{
	worker_call_count = 0;

	/* ==================== NULL ARGUMENTS ==================== */

	assert(apply_at_da(NULL, 0, _increment_int) == DA_ERR_NULL_ARGUMENT);

	DynamicArray *da = NULL;

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(apply_at_da(da, 0, NULL) == DA_ERR_NULL_ARGUMENT);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== EMPTY ARRAY ==================== */

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(apply_at_da(da, 0, _increment_int) == DA_EMPTY);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== INDEX OUT OF BOUNDS ==================== */

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	for (int i = 0; i < 5; i++)
	{
		assert(push_int_da(da, i) == DA_SUCCESS);
	}

	assert(apply_at_da(da, 5, _increment_int) == DA_ERR_INDEX_OUT_OF_BOUNDS);
	assert(apply_at_da(da, 100, _increment_int) == DA_ERR_INDEX_OUT_OF_BOUNDS);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - INT ==================== */

	worker_call_count = 0;

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(push_int_da(da, 10) == DA_SUCCESS);

	assert(apply_at_da(da, 0, _increment_int) == DA_SUCCESS);

	assert(da->int_arr[0] == 11);

	assert(worker_call_count == 1);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - CHAR ==================== */

	worker_call_count = 0;

	assert(new_dynamic_array(DA_CHAR, &da) == DA_SUCCESS);

	assert(push_char_da(da, 'a') == DA_SUCCESS);

	assert(apply_at_da(da, 0, _uppercase_char) == DA_SUCCESS);

	assert(da->char_arr[0] == 'A');

	assert(worker_call_count == 1);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - FLOAT ==================== */

	worker_call_count = 0;

	assert(new_dynamic_array(DA_FLOAT, &da) == DA_SUCCESS);

	assert(push_float_da(da, 5.0f) == DA_SUCCESS);

	assert(apply_at_da(da, 0, _double_float) == DA_SUCCESS);

	assert(da->float_arr[0] == 10.0f);

	assert(worker_call_count == 1);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - DOUBLE ==================== */

	worker_call_count = 0;

	assert(new_dynamic_array(DA_DOUBLE, &da) == DA_SUCCESS);

	assert(push_double_da(da, 2.5) == DA_SUCCESS);

	assert(apply_at_da(da, 0, _double_double) == DA_SUCCESS);

	assert(da->double_arr[0] == 5.0);

	assert(worker_call_count == 1);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - PTR ==================== */

	worker_call_count = 0;

	int value = 100;

	assert(new_dynamic_array(DA_PTR, &da) == DA_SUCCESS);

	assert(push_ptr_da(da, &value) == DA_SUCCESS);

	assert(apply_at_da(da, 0, _increment_ptr_value) == DA_SUCCESS);

	assert(value == 101);

	assert(worker_call_count == 1);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== APPLY ONLY TARGET INDEX ==================== */

	worker_call_count = 0;

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(push_int_da(da, 1) == DA_SUCCESS);
	assert(push_int_da(da, 2) == DA_SUCCESS);
	assert(push_int_da(da, 3) == DA_SUCCESS);

	assert(apply_at_da(da, 1, _increment_int) == DA_SUCCESS);

	assert(da->int_arr[0] == 1);
	assert(da->int_arr[1] == 3);
	assert(da->int_arr[2] == 3);

	assert(worker_call_count == 1);

	assert(free_dynamic_array(&da) == DA_SUCCESS);
}

static int process_call_count = 0;

static void _process_int(void *item_ptr)
{
	assert(item_ptr != NULL);

	int *value = (int *)item_ptr;

	(*value)++;

	process_call_count++;
}

static void _process_char(void *item_ptr)
{
	assert(item_ptr != NULL);

	char *c = (char *)item_ptr;

	if (*c >= 'a' && *c <= 'z')
	{
		*c = (char)(*c - ('a' - 'A'));
	}

	process_call_count++;
}

static void _process_float(void *item_ptr)
{
	assert(item_ptr != NULL);

	float *f = (float *)item_ptr;

	*f *= 2.0f;

	process_call_count++;
}

static void _process_double(void *item_ptr)
{
	assert(item_ptr != NULL);

	double *d = (double *)item_ptr;

	*d *= 2.0;

	process_call_count++;
}

static void _process_ptr(void *item_ptr)
{
	assert(item_ptr != NULL);

	int *value = (int *)item_ptr;

	(*value)++;

	process_call_count++;
}

static void _test_process_da()
{
	process_call_count = 0;

	/* ==================== NULL ARGUMENTS ==================== */

	assert(process_da(NULL, _process_int) == DA_ERR_NULL_ARGUMENT);

	DynamicArray *da = NULL;

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(process_da(da, NULL) == DA_ERR_NULL_ARGUMENT);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== EMPTY ARRAY ==================== */

	process_call_count = 0;

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(process_da(da, _process_int) == DA_SUCCESS);

	assert(process_call_count == 0);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - INT ==================== */

	process_call_count = 0;

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	for (int i = 0; i < 5; i++)
	{
		assert(push_int_da(da, i) == DA_SUCCESS);
	}

	assert(process_da(da, _process_int) == DA_SUCCESS);

	assert(process_call_count == 5);

	for (int i = 0; i < 5; i++)
	{
		assert(da->int_arr[i] == i + 1);
	}

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - CHAR ==================== */

	process_call_count = 0;

	assert(new_dynamic_array(DA_CHAR, &da) == DA_SUCCESS);

	assert(push_char_da(da, 'a') == DA_SUCCESS);
	assert(push_char_da(da, 'b') == DA_SUCCESS);
	assert(push_char_da(da, 'c') == DA_SUCCESS);

	assert(process_da(da, _process_char) == DA_SUCCESS);

	assert(process_call_count == 3);

	assert(da->char_arr[0] == 'A');
	assert(da->char_arr[1] == 'B');
	assert(da->char_arr[2] == 'C');

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - FLOAT ==================== */

	process_call_count = 0;

	assert(new_dynamic_array(DA_FLOAT, &da) == DA_SUCCESS);

	assert(push_float_da(da, 1.0f) == DA_SUCCESS);
	assert(push_float_da(da, 2.0f) == DA_SUCCESS);
	assert(push_float_da(da, 3.0f) == DA_SUCCESS);

	assert(process_da(da, _process_float) == DA_SUCCESS);

	assert(process_call_count == 3);

	assert(da->float_arr[0] == 2.0f);
	assert(da->float_arr[1] == 4.0f);
	assert(da->float_arr[2] == 6.0f);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - DOUBLE ==================== */

	process_call_count = 0;

	assert(new_dynamic_array(DA_DOUBLE, &da) == DA_SUCCESS);

	assert(push_double_da(da, 1.0) == DA_SUCCESS);
	assert(push_double_da(da, 2.0) == DA_SUCCESS);
	assert(push_double_da(da, 3.0) == DA_SUCCESS);

	assert(process_da(da, _process_double) == DA_SUCCESS);

	assert(process_call_count == 3);

	assert(da->double_arr[0] == 2.0);
	assert(da->double_arr[1] == 4.0);
	assert(da->double_arr[2] == 6.0);

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== SUCCESS CASE - PTR ==================== */

	process_call_count = 0;

	int values[5];

	assert(new_dynamic_array(DA_PTR, &da) == DA_SUCCESS);

	for (int i = 0; i < 5; i++)
	{
		values[i] = i;

		assert(push_ptr_da(da, &values[i]) == DA_SUCCESS);
	}

	assert(process_da(da, _process_ptr) == DA_SUCCESS);

	assert(process_call_count == 5);

	for (int i = 0; i < 5; i++)
	{
		assert(values[i] == i + 1);
	}

	assert(free_dynamic_array(&da) == DA_SUCCESS);

	/* ==================== PROCESS ORDER CHECK ==================== */

	process_call_count = 0;

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);

	assert(push_int_da(da, 10) == DA_SUCCESS);
	assert(push_int_da(da, 20) == DA_SUCCESS);
	assert(push_int_da(da, 30) == DA_SUCCESS);

	assert(process_da(da, _process_int) == DA_SUCCESS);

	assert(da->int_arr[0] == 11);
	assert(da->int_arr[1] == 21);
	assert(da->int_arr[2] == 31);

	assert(process_call_count == 3);

	assert(free_dynamic_array(&da) == DA_SUCCESS);
}

static int _is_even_filter(void *item_ptr)
{
	if (item_ptr == NULL)
		return 0;
	int value = *(int *)item_ptr;
	return (value % 2 == 0) ? 1 : 0;
}

static void _test_filter_da()
{
	enum DynamicArrayError err;
	DynamicArray *da = NULL;

	// ---------------------------------------------------------
	// TEST 1: NULL Array Argument
	// ---------------------------------------------------------
	err = filter_da(NULL, _is_even_filter);
	assert(err == DA_ERR_NULL_ARGUMENT);

	// ---------------------------------------------------------
	// TEST 2: NULL Filter Argument
	// ---------------------------------------------------------
	err = new_dynamic_array(DA_INT, &da);
	assert(err == DA_SUCCESS);
	assert(da != NULL);

	err = filter_da(da, NULL);
	assert(err == DA_ERR_NULL_ARGUMENT);

	// ---------------------------------------------------------
	// TEST 3: Empty Array Execution
	// ---------------------------------------------------------
	err = filter_da(da, _is_even_filter);
	assert(err == DA_SUCCESS);
	assert(da->count == 0); // Sanity check that size remains 0

	// ---------------------------------------------------------
	// TEST 4: Valid Filtering (Success Case)
	// ---------------------------------------------------------
	// Populate the array: [1, 2, 3, 4, 5]
	assert(push_int_da(da, 1) == DA_SUCCESS);
	assert(push_int_da(da, 2) == DA_SUCCESS);
	assert(push_int_da(da, 3) == DA_SUCCESS);
	assert(push_int_da(da, 4) == DA_SUCCESS);
	assert(push_int_da(da, 5) == DA_SUCCESS);

	err = filter_da(da, _is_even_filter);

	// After filtering even numbers, array should be: [2, 4]
	assert(err == DA_SUCCESS);
	assert(da->count == 2);
	assert(da->int_arr[0] == 2);
	assert(da->int_arr[1] == 4);

	// ---------------------------------------------------------
	// TEST 5: Unknown/Invalid Array Type
	// ---------------------------------------------------------
	// We intentionally corrupt the type to see if internal allocations catch it.
	da->type = (enum DynamicArrayType)999;
	err = filter_da(da, _is_even_filter);
	assert(err == DA_ERR_TYPE_UNKNOWN);

	// Cleanup: We must manually restore the type so the free function works properly.
	da->type = DA_INT;
	err = free_dynamic_array(&da);
	assert(err == DA_SUCCESS);
	assert(da == NULL);
}

void dynamicArrayTest()
{
	puts("################## Test: DynamicArray ##################");
	printf("Running DynamicArray tests...\n");

	// new_dynamic_array
	DynamicArray *da = NULL;
	assert(new_dynamic_array((enum DynamicArrayType)999, &da) == DA_ERR_TYPE_UNKNOWN);
	assert(new_dynamic_array(DA_INT, NULL) == DA_ERR_NULL_ARGUMENT);

	assert(new_dynamic_array(DA_INT, &da) == DA_SUCCESS);
	assert(da != NULL);
	assert(da->count == 0);
	assert(da->type == DA_INT);
	assert(da->capacity == DYNAMIC_ARRAY_INIT_CAPACITY);
	assert(da->destructor == NULL);
	assert(da->single_item_size == sizeof(int));

	assert(new_dynamic_array(DA_INT, &da_i) == DA_SUCCESS);
	assert(new_dynamic_array(DA_CHAR, &da_c) == DA_SUCCESS);
	assert(new_dynamic_array(DA_FLOAT, &da_f) == DA_SUCCESS);
	assert(new_dynamic_array(DA_DOUBLE, &da_d) == DA_SUCCESS);
	assert(new_dynamic_array(DA_PTR, &da_p) == DA_SUCCESS);

	assert(new_dynamic_array(DA_INT, &da_invalid_type) == DA_SUCCESS);
	da_invalid_type->type = (enum DynamicArrayType)10000;
	assert(da_invalid_type->type == 10000);

	// new_dynamic_array_d
	DynamicArray *dad = NULL;
	assert(new_dynamic_array_d((enum DynamicArrayType)999, _person_destructor, &dad) == DA_ERR_TYPE_UNKNOWN);
	assert(new_dynamic_array_d(DA_PTR, NULL, &dad) == DA_ERR_NULL_ARGUMENT);
	assert(new_dynamic_array_d(DA_PTR, _person_destructor, NULL) == DA_ERR_NULL_ARGUMENT);

	assert(new_dynamic_array_d(DA_PTR, _person_destructor, &dad) == DA_SUCCESS);
	assert(dad != NULL);
	assert(dad->count == 0);
	assert(dad->type == DA_PTR);
	assert(dad->capacity == DYNAMIC_ARRAY_INIT_CAPACITY);
	assert(dad->destructor == _person_destructor);
	assert(dad->single_item_size == sizeof(void *));

	// free
	_test_free();

	// push
	_test_push();

	// unshift
	_test_unshift();

	// pop
	_test_pop();

	// shift
	_test_shift();

	// activate_destructor_da
	_test_activate_destructor_da();

	// apply_destructor_da
	_test_apply_destructor_da();

	// remove_at_da
	_test_remove_at();

	// remove_first_da
	_test_remove_first();

	// apply_at_da
	_test_apply_at();

	// process_da
	_test_process_da();

	// filter_da
	_test_filter_da();

	// at_da
	_test_at_da();

	// if (da->count > 0)
	// {
	// 	assert(at_da(da, 0, &out) == DA_SUCCESS);
	// 	assert(out != NULL);
	// }

	// assert(at_da(da, 9999, &out) == DA_ERR_INDEX_OUT_OF_BOUNDS);

	// find_da
	// push_int_da(da, 5);

	// void *found = NULL;
	// assert(find_da(NULL, &found, equals_five) == DA_ERR_NULL_ARGUMENT);
	// assert(find_da(da, NULL, equals_five) == DA_ERR_NULL_ARGUMENT);
	// assert(find_da(da, &found, NULL) == DA_ERR_NULL_ARGUMENT);

	// int res = find_da(da, &found, equals_five);
	// assert(res == DA_SUCCESS || res == DA_ITEM_NOT_FOUND);

	// find_last_da
	// res = find_last_da(da, &found, equals_five);
	// assert(res == DA_SUCCESS || res == DA_ITEM_NOT_FOUND);

	// find_index_da
	// size_t idx;
	// res = find_index_da(da, &idx, greater_than_three);
	// assert(res == DA_SUCCESS || res == DA_ITEM_NOT_FOUND);

	// res = find_last_index_da(da, &idx, greater_than_three);
	// assert(res == DA_SUCCESS || res == DA_ITEM_NOT_FOUND);

	// index_of_da
	// int value = 5;
	// res = index_of_da(da, &idx, &value);
	// assert(res == DA_SUCCESS || res == DA_ITEM_NOT_FOUND);

	// assert(index_of_da(NULL, &idx, &value) == DA_ERR_NULL_ARGUMENT);
	// assert(index_of_da(da, NULL, &value) == DA_ERR_NULL_ARGUMENT);
	// assert(index_of_da(da, &idx, NULL) == DA_ERR_NULL_ARGUMENT);

	// DynamicArrayIterator
	// DynamicArray *da_itr = NULL;
	// int err_da_itr = new_dynamic_array(DA_INT, &da_itr);
	// assert(err_da_itr == 0);
	// for (size_t i = 0; i < 5; ++i)
	// 	push_int_da(da_itr, i);
	// assert(da_itr->count == 5);
	// DynamicArrayIterator *itr = NULL;
	// int err_itr_init = new_iterator_da(da_itr, &itr);
	// assert(err_itr_init == 0);
	// assert(itr->da == da_itr);
	// assert(itr->current_index == 0);

	// int da_has_next = 0;
	// int err_has_next = has_next_dai(NULL, &da_has_next);
	// assert(err_has_next == DA_ERR_NULL_ARGUMENT);
	// err_has_next = has_next_dai(itr, NULL);
	// assert(err_has_next == DA_ERR_NULL_ARGUMENT);
	// err_has_next = has_next_dai(itr, &da_has_next);
	// assert(err_has_next == 0);

	// int err_next = 0;
	// err_next = next_int_dai(NULL, &value);
	// assert(err_next == DA_ERR_NULL_ARGUMENT);
	// err_next = next_int_dai(itr, NULL);
	// assert(err_next == DA_ERR_NULL_ARGUMENT);

	// int itr_value_tracker = 0;
	// while (da_has_next)
	// {
	// 	int value = -1;
	// 	err_next = next_int_dai(itr, &value);
	// 	assert(err_next == 0);
	// 	assert(value == itr_value_tracker);
	// 	itr_value_tracker++;
	// 	err_has_next = has_next_dai(itr, &da_has_next);
	// 	assert(err_has_next == 0);
	// }
	// // checking iteration boundaries
	// err_has_next = has_next_dai(itr, &da_has_next);
	// assert(err_has_next == 0);
	// assert(da_has_next == 0);
	// err_next = next_int_dai(itr, &value);
	// assert(err_next == DA_ERR_INDEX_OUT_OF_BOUNDS);

	// Cleanup
	int err_f_da = free_dynamic_array(&da);
	assert(err_f_da == 0);
	// int err_f_itr = free_iterator_da(&itr);
	// assert(err_f_itr == 0);

	free_dynamic_array(&da_i);
	free_dynamic_array(&da_c);
	free_dynamic_array(&da_f);
	free_dynamic_array(&da_d);
	free_dynamic_array(&da_p);
	da_invalid_type->type = DA_INT; // fix type to free correctly
	free_dynamic_array(&da_invalid_type);

	printf(ANSI_COLOR_GREEN "All DynamicArray tests passed.\n" ANSI_COLOR_RESET);
	puts("################## Test: DynamicArray ##################");
}