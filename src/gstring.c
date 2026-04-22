#include <stdlib.h>
#include <stdarg.h>
#include "../include/gstring.h"

// ##################   static   ##################

static enum StringError _new_char_array(size_t length, char **const output)
{
	if (output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	char *arr = (char *)malloc(sizeof(char) * length);
	if (arr == NULL)
		return STR_ERR_MEMORY_ALLOCATION;

	*output = arr;
	return STR_SUCCESS;
}

static enum StringError _extend(String *const str_to_extend, size_t const extend_count)
{
	if (str_to_extend == NULL)
		return STR_ERR_NULL_ARGUMENT;

	if (extend_count <= 0)
		return STR_ERR_INVALID_ARGUMENT_DIMENTIONS;

	size_t new_length = extend_count + str_to_extend->length;

	char *new_str = realloc(str_to_extend->str, new_length);
	if (new_str == NULL)
		return STR_ERR_MEMORY_ALLOCATION;

	str_to_extend->str = new_str;
	str_to_extend->length = new_length;
	return STR_SUCCESS;
}

static enum StringError _shrink(String *const str_to_shrink, size_t const shrink_count)
{
	if (str_to_shrink == NULL)
		return STR_ERR_NULL_ARGUMENT;

	if (str_to_shrink->length < shrink_count)
		return STR_ERR_INVALID_ARGUMENT_DIMENTIONS;

	size_t new_size = str_to_shrink->length - shrink_count;

	char *new_str = realloc(str_to_shrink->str, new_size);
	if (new_str == NULL)
		return STR_ERR_MEMORY_ALLOCATION;

	str_to_shrink->str = new_str;
	str_to_shrink->length = new_size;
	return STR_SUCCESS;
}

static enum StringError _shift_one_right(String *const str)
{
	if (str == NULL)
		return STR_ERR_NULL_ARGUMENT;

	for (size_t i = str->length - 1; i > 0; --i)
		(str->str)[i] = (str->str)[i - 1];

	return STR_SUCCESS;
}

/**
 * @brief Eextends memory, modifies the length property of the String and shifts all characters to the right.
 */
static enum StringError _shift_count_right(String *const str, size_t const count)
{
	if (str == NULL)
		return STR_ERR_NULL_ARGUMENT;

	if (count <= 0)
		return STR_ERR_INVALID_ARGUMENT_DIMENTIONS;

	size_t new_length = count + str->length;

	char *new_arr = NULL;
	int err_new_arr = _new_char_array(new_length, &new_arr);
	if (err_new_arr)
		return err_new_arr;

	size_t j = 0;
	for (size_t i = count; i < new_length; ++i)
	{
		new_arr[i] = (str->str)[j++];
	}

	free(str->str);
	str->str = new_arr;
	str->length = new_length;

	return STR_SUCCESS;
}

static enum StringError _index_of_nt(const char *const char_arr, size_t *const output)
{
	if (char_arr == NULL || output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	for (size_t i = 0L; i <= GSTRING_LOOP_MAX_LIMIT; ++i)
	{
		if (char_arr[i] == '\0')
		{
			*output = i;
			return STR_SUCCESS;
		}
	}
	return STR_ERR_LOOP_MAX_LIMIT;
}

/**
 * @brief Copies count characters from source to dest.
 * The process starts from index 0 for both source and dest.
 */
static enum StringError _copy_chars(const char *const source, char *const dest, size_t const count)
{
	if (source == NULL || dest == NULL)
		return STR_ERR_NULL_ARGUMENT;

	for (size_t i = 0; i < count; ++i)
		dest[i] = source[i];

	return STR_SUCCESS;
}

static enum StringError _duplicate_char_array(const char *const char_arr, size_t const length, char **const output)
{
	if (char_arr == NULL || output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	char *new_arr = NULL;
	int err_new_arr = _new_char_array(length, &new_arr);
	if (err_new_arr)
		return err_new_arr;

	int err_copy = _copy_chars(char_arr, new_arr, length);
	if (err_copy)
	{
		free(new_arr);
		return err_copy;
	}

	*output = new_arr;
	return STR_SUCCESS;
}

// ##################   public   ##################

enum StringError new_string(const char *const char_arr, size_t length, String **const output)
{
	if (output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	if (char_arr == NULL && length != 0)
		return STR_ERR_INVALID_ARGUMENT_DIMENTIONS;
	if (char_arr != NULL && length == 0)
		return STR_ERR_INVALID_ARGUMENT_DIMENTIONS;

	String *new_str = malloc(sizeof(String));
	if (new_str == NULL)
		return STR_ERR_MEMORY_ALLOCATION;

	char *str = NULL;
	int err_arr_init = _new_char_array(length, &str);
	if (err_arr_init)
	{
		free(new_str);
		return err_arr_init;
	}

	if (char_arr == NULL && length == 0)
		goto _end_stage;

	int err_copy = _copy_chars(char_arr, str, length);
	if (err_copy)
	{
		free(new_str);
		free(str);
		return err_copy;
	}

_end_stage:
	new_str->str = str;
	new_str->length = length;
	*output = new_str;

	return STR_SUCCESS;
}

enum StringError free_string(String *str)
{
	if (str == NULL)
		return STR_ERR_NULL_ARGUMENT;

	free(str->str);
	free(str);

	return STR_SUCCESS;
}

enum StringError append_char(String *const str, const char c)
{
	if (str == NULL)
		return STR_ERR_NULL_ARGUMENT;

	int err = _extend(str, 1);
	if (err)
		return err;

	(str->str)[str->length - 1] = c;

	return STR_SUCCESS;
}

enum StringError append_char_array(String *const str_dest, const char *const source, const size_t source_length)
{
	if (source == NULL || str_dest == NULL)
		return STR_ERR_NULL_ARGUMENT;

	int err = _extend(str_dest, source_length);
	if (err)
		return err;

	for (size_t i = 0; i < source_length; ++i)
		(str_dest->str)[str_dest->length - source_length + i] = source[i];

	return STR_SUCCESS;
}

enum StringError append_nt(String *const str_dest, const char *const source)
{
	if (str_dest == NULL || source == NULL)
		return STR_ERR_NULL_ARGUMENT;

	size_t nt_index = 0;
	int err_index = _index_of_nt(source, &nt_index);
	if (err_index)
		return err_index;

	/**
	 * nt_index is the length of source, because it is the index of the null-terminator.
	 */
	int err_extend = _extend(str_dest, nt_index);
	if (err_extend)
		return err_extend;

	for (size_t i = 0; i < nt_index; ++i)
		(str_dest->str)[str_dest->length - nt_index + i] = source[i];

	return STR_SUCCESS;
}

enum StringError append_str(const String *const str_source, String *const str_dest)
{
	if (str_source == NULL || str_dest == NULL)
		return STR_ERR_NULL_ARGUMENT;

	size_t old_dest_length = str_dest->length;

	int err = _extend(str_dest, str_source->length);
	if (err)
		return err;

	for (size_t i = 0; i < str_source->length; ++i)
	{
		(str_dest->str)[old_dest_length + i] = (str_source->str)[i];
	}

	return STR_SUCCESS;
}

enum StringError prepend_char(String *const str, const char c)
{
	if (str == NULL)
		return STR_ERR_NULL_ARGUMENT;

	int err_extend = _extend(str, 1);
	if (err_extend)
		return err_extend;

	int err_shift = _shift_one_right(str);
	if (err_shift)
		return err_shift;

	(str->str)[0] = c;

	return STR_SUCCESS;
}

enum StringError prepend_char_array(String *const str_dest, const char *const source, const size_t source_length)
{
	if (str_dest == NULL || source == NULL)
		return STR_ERR_NULL_ARGUMENT;
	/*
		TODO: implement a String back-up system
		String *str_backup = NULL;
		int err_duplication  = duplicate_str(str_dest, &str_backup);
		if (err_duplication)
			return err_duplication;
	 */
	int err_extend = _extend(str_dest, source_length);
	if (err_extend)
		return err_extend;

	int err_shift = _shift_count_right(str_dest, source_length);
	if (err_shift)
	{

		// free_string(str_backup);
		return err_shift;
	}

	int err_copy = _copy_chars(source, str_dest->str, source_length);
	if (err_copy)
		return err_copy;

	return STR_SUCCESS;
}

enum StringError prepend_nt(String *const str_dest, const char *const source)
{
	if (str_dest == NULL || source == NULL)
		return STR_ERR_NULL_ARGUMENT;

	size_t nt_index = 0;
	int err_index = _index_of_nt(source, &nt_index);
	if (err_index)
		return err_index;
	/**
	 * nt_index is the length of the actual characters, because it is the index of the null-terminator.
	 */
	int err_shift = _shift_count_right(str_dest, nt_index);
	if (err_shift)
		return err_shift;

	int err_copy = _copy_chars(source, str_dest->str, nt_index);
	if (err_copy)
		return err_copy;

	return STR_SUCCESS;
}

enum StringError prepend_str(const String *const source, String *const dest)
{
	if (source == NULL || dest == NULL)
		return STR_ERR_NULL_ARGUMENT;

	int err_shift = _shift_count_right(dest, source->length);
	if (err_shift)
		return err_shift;

	int err_copy = _copy_chars(source->str, dest->str, source->length);
	if (err_copy)
		return err_copy;

	return STR_SUCCESS;
}

enum StringError duplicate_str(const String *const str_source, String **const output)
{
	if (str_source == NULL)
		return STR_ERR_NULL_ARGUMENT;

	String *str_duplicate = malloc(sizeof(String));
	if (str_duplicate == NULL)
		return STR_ERR_MEMORY_ALLOCATION;

	char *str_char_arr_duplicate = NULL;
	int err_duplicate = _duplicate_char_array(str_source->str, str_source->length, &str_char_arr_duplicate);
	if (err_duplicate)
	{
		free(str_duplicate);
		return err_duplicate;
	}

	str_duplicate->str = str_char_arr_duplicate;
	str_duplicate->length = str_source->length;
	*output = str_duplicate;

	return STR_SUCCESS;
}

enum StringError get_raw(const String *const source, char **const output)
{
	if (source == NULL || output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	char *result = NULL;
	int err = _duplicate_char_array(source->str, source->length, &result);
	if (err)
		return err;

	*output = result;

	return STR_SUCCESS;
}

enum StringError get_raw_nt(const String *const source, char **const output)
{
	if (source == NULL || output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	char *new_arr = NULL;
	int err_new_arr = _new_char_array(source->length + 1, &new_arr);
	if (err_new_arr)
		return err_new_arr;

	int err_copy = _copy_chars(source->str, new_arr, source->length);
	if (err_copy)
		return err_copy;

	new_arr[source->length] = '\0';
	*output = new_arr;

	return STR_SUCCESS;
}

enum StringError filter_str(String *const str, int (*filter)(char c))
{
	if (str == NULL || filter == NULL)
		return STR_ERR_NULL_ARGUMENT;

	char *wanted_chars = NULL;
	int err_new_arr = _new_char_array(str->length, &wanted_chars);
	if (err_new_arr)
		return err_new_arr;

	size_t wanted_count = 0;

	/* filter return: 1 -> stay; 0 -> remove */
	for (size_t i = 0; i < str->length; ++i)
	{
		char c = (str->str)[i];
		if (filter(c) == 1)
			wanted_chars[wanted_count++] = c;
	}

	/* wantedCount is now the length, because of the last post-incrementation */
	char *resized_wanted_chars = realloc(wanted_chars, sizeof(char) * wanted_count);
	if (resized_wanted_chars == NULL)
		return STR_ERR_MEMORY_ALLOCATION;

	free(str->str);
	str->str = resized_wanted_chars;
	str->length = wanted_count;

	return STR_SUCCESS;
}

enum StringError replace_char(String *const str, const char to_replace, const char replacement)
{
	if (str == NULL)
		return STR_ERR_NULL_ARGUMENT;

	if (str->length == 0)
		return STR_SUCCESS;

	for (size_t i = 0; i < str->length; ++i)
	{
		if ((str->str)[i] == to_replace)
			(str->str)[i] = replacement;
	}

	return STR_SUCCESS;
}

enum StringError remove_char(String *const str, const char to_remove)
{
	if (str == NULL)
		return STR_ERR_NULL_ARGUMENT;

	if (str->length == 0)
		return STR_SUCCESS;

	char *tmp_arr = NULL;
	int err_new_arr = _new_char_array(str->length, &tmp_arr);
	if (err_new_arr)
		return err_new_arr;

	size_t tmp_arr_length = 0;
	for (size_t i = 0; i < str->length; ++i)
	{
		if ((str->str)[i] != to_remove)
			tmp_arr[tmp_arr_length++] = (str->str)[i];
	}

	char *new_str = realloc(tmp_arr, sizeof(char) * tmp_arr_length);
	if (new_str == NULL)
		return STR_ERR_MEMORY_ALLOCATION;

	free(str->str);
	str->str = new_str;
	str->length = tmp_arr_length;

	return STR_SUCCESS;
}

enum StringError concat(String **const output, const size_t n_str, ...)
{
	/**
	 * Documentation on stdarg.h and variadic functions.
	 * https://en.cppreference.com/c/header/stdarg
	 * https://en.cppreference.com/c/variadic
	 * https://en.cppreference.com/c/language/variadic
	 */
	if (output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	String *str_result = NULL;
	int err_result_init = new_string(NULL, 0, &str_result);
	if (err_result_init)
		return err_result_init;

	va_list list;
	va_start(list, n_str);
	for (size_t i = 0; i < n_str; ++i)
	{
		String *str = va_arg(list, String *);
		if (str == NULL)
			return STR_ERR_NULL_ARGUMENT;
		int err_concat = append_str(str, str_result);
		if (err_concat)
		{
			free_string(str_result);
			return err_concat;
		}
	}
	va_end(list);

	*output = str_result;

	return STR_SUCCESS;
}
