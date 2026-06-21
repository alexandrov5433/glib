#include <stdlib.h>
#include <stdarg.h>
#include "../include/gstring.h"
#include "../include/dynamic_array.h"

// ##################   static   ##################

static enum StringError _new_char_array(size_t length, char **const output)
{
	if (output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	if (length == 0)
		return STR_ERR_INVALID_ARGUMENT_DIMENTIONS;

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

	/**
	 * https://en.cppreference.com/c/memory/realloc
	 * If str_to_extend->str is NULL, the behavior of realloc is the same as calling malloc(new_size).
	 */
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

	if (shrink_count == 0)
		return STR_SUCCESS;

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

	if (str->str == NULL)
		return STR_ERR_NULL_STR;

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

	for (size_t i = 0; i < count; ++i)
	{
		int err = _shift_one_right(str);
		if (err)
			return err;
	}

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

/**
 * Validates the @ref String structure.
 * @param str @ref String to validate.
 * @return A value of the @ref StringError:
 *
 * - STR_SUCCESS
 *
 * - STR_ERR_NULL_ARGUMENT
 *
 * - STR_ERR_NULL_STR
 *
 * - STR_ERR_ZERO_LENGTH
 */
static enum StringError _validate_nsl(const String *const str)
{
	if (str == NULL)
		return STR_ERR_NULL_ARGUMENT;

	if (str->str == NULL && str->length == 0)
		return STR_SUCCESS;

	if (str->str == NULL)
		return STR_ERR_NULL_STR;

	if (str->length == 0)
		return STR_ERR_ZERO_LENGTH;

	return STR_SUCCESS;
}

/**
 * Splits the String by a pattern. The result is a DynamicArray with a String at each index.
 * The indexes holding pattern parts are empty Strings. The other indexes hold the Strings, extracted from the given String.
 * PATasdPATasdf -> [ "", "asd", "", "asdf" ]
 * @return A value of the @ref StringError:
 *
 * - STR_SUCCESS
 *
 * - STR_ERR_NULL_ARGUMENT
 *
 * - STR_ERR_INVALID_ARGUMENT_DIMENTIONS
 *
 * - STR_ERR_MEMORY_ALLOCATION
 *
 * - STR_ERR_NULL_STR
 *
 * - STR_ERR_ZERO_LENGTH
 *
 * - STR_ERR_DYNAMIC_ARRAY
 */
static enum StringError _split_str_pattern_to_empty(
    const String *const str,
    const String *const pattern,
    DynamicArray **const output)
{
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

	err_valid = _validate_nsl(pattern);
	if (err_valid)
		return err_valid;

	if (output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	DynamicArray *parts = NULL;
	int err_da_init = new_dynamic_array(DA_PTR, &parts);
	if (err_da_init)
		return STR_ERR_DYNAMIC_ARRAY;

	if (str->length == 0 ||
	    pattern->length == 0 ||
	    str->length <= pattern->length)
		goto _end_stage;

	String *part = NULL;
	size_t index_part_start = 0;
	size_t index_str = 0;
	int must_add_empty_string = 0;
_main_loop:
	while (index_str < str->length)
	{
		must_add_empty_string = 0; // Do not add empty String.
		// Check if pattern starts, else continue.
		if ((str->str)[index_str] != (pattern->str)[0])
		{
			index_str++;
			/*
			If the end of the String is reached and the start of the pattern is not encountered,
			then the String ends with a part, which must be extracted.
			Pattern look-ahead must be skiped.
			*/
			if (index_str == str->length - 1)
				goto _loop_part_extraction_stage;

			goto _main_loop;
		}

		must_add_empty_string = 1; // Assume the pattern is complete. The value is reset on every _main_loop cicle.
		// Look ahead if the pattern is complete, else continue.
		for (size_t i = 0; i < pattern->length; ++i)
		{
			if (index_str + i >= str->length)
			{
				/*
				The String ends with a part of the pattern. This must be included in the current part.
				*/
				index_str = str->length - 1;
				must_add_empty_string = 0; // Do not add empty String.
				break;
			}

			if ((str->str)[index_str + i] != (pattern->str)[i])
			{
				index_str += i;
				goto _main_loop;
			}
		}

		/*
		If the pattern was confirmed and index_str == index_part_start is still true,
		then this is a case where this pattern comes directly after the previous pattern.
		There is nothing to extract from inbetween.
		*/
		if (index_str == index_part_start)
			goto _loop_empty_str_addition_stage;

	_loop_part_extraction_stage:
		// The part starts at index_part_start and ends at index_str - 1. At index_str starts the pattern.
		int err_part_init = new_string_nt(NULL, &part);
		if (err_part_init)
		{
			// TODO: Update clean-up with activate_destructor
			apply_destructor_da(parts, (void (*)(void **))free_string);
			return err_part_init;
		}
		size_t part_length = index_str - index_part_start;
		/*
		If the String ends with a part for extraction, index_str points to
		the last character of the part, and not to the first part of the (usually) upcomming pattern.
		Therefore, the part_length must be corrected.
		*/
		if (index_str == str->length - 1)
			part_length++;

		for (size_t i = 0; i < part_length; ++i)
		{
			int err_append = append_char(part, (str->str)[index_part_start + i]);
			if (err_append)
			{
				apply_destructor_da(parts, (void (*)(void **))free_string);
				return err_append;
			}
		}

		// Add part to array.
		int err_push_part = push_ptr_da(parts, (void *)part);
		if (err_push_part)
		{
			apply_destructor_da(parts, (void (*)(void **))free_string);
			return STR_ERR_DYNAMIC_ARRAY;
		}

	_loop_empty_str_addition_stage:
		// Add emtpy String.
		if (1 == must_add_empty_string)
		{
			String *empty = NULL;
			int err_empty_init = new_string_nt(NULL, &empty);
			if (err_empty_init)
			{
				apply_destructor_da(parts, (void (*)(void **))free_string);
				return err_empty_init;
			}
			int err_push_empty = push_ptr_da(parts, (void *)empty);
			if (err_push_empty)
			{
				apply_destructor_da(parts, (void (*)(void **))free_string);
				return STR_ERR_DYNAMIC_ARRAY;
			}
		}

		// Update indexes, so that we jump over the current pattern, to the start of the next part.
		index_str += pattern->length;
		index_part_start = index_str;
	}

_end_stage:
	*output = parts;
	return STR_SUCCESS;
}

// ##################   workers   ##################

/**
 * Frees the memory of empty Strings with free_string.
 */
static inline void _worker_free_empty_str(void *str)
{
	if (NULL == str)
		return;

	String *tmp = str;
	if (tmp->length == 0)
		free_string(&tmp);
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
	if (char_arr == NULL && length == 0)
		goto _end_stage;

	int err_arr_init = _new_char_array(length, &str);
	if (err_arr_init)
	{
		free(new_str);
		return err_arr_init;
	}

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

enum StringError new_string_nt(const char *const char_arr, String **const output)
{
	if (output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	String *new_str = malloc(sizeof(String));
	if (new_str == NULL)
		return STR_ERR_MEMORY_ALLOCATION;

	char *str = NULL;
	/* The index of the null-terminator is the length of the String. */
	size_t str_length = 0;
	if (char_arr == NULL)
		goto _end_stage;

	int err_index = _index_of_nt(char_arr, &str_length);
	if (err_index)
		return err_index;

	if (str_length == 0)
		goto _end_stage;

	int err_arr_init = _new_char_array(str_length, &str);
	if (err_arr_init)
	{
		free(new_str);
		return err_arr_init;
	}

	int err_copy = _copy_chars(char_arr, str, str_length);
	if (err_copy)
	{
		free(new_str);
		free(str);
		return err_copy;
	}

_end_stage:
	new_str->str = str;
	new_str->length = str_length;
	*output = new_str;

	return STR_SUCCESS;
}

enum StringError free_string(String **str)
{
	if (NULL == str)
		return STR_ERR_NULL_ARGUMENT;
	if (NULL == *str)
		return STR_SUCCESS;

	free(*str);
	*str = NULL;

	return STR_SUCCESS;
}

enum StringError append_char(String *const str, const char c)
{
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

	int err = _extend(str, 1);
	if (err)
		return err;

	(str->str)[str->length - 1] = c;

	return STR_SUCCESS;
}

enum StringError append_char_array(String *const str_dest, const char *const source, const size_t source_length)
{
	int err_valid = _validate_nsl(str_dest);
	if (err_valid)
		return err_valid;

	if (source == NULL)
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
	int err_valid = _validate_nsl(str_dest);
	if (err_valid)
		return err_valid;

	if (source == NULL)
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
	int err_valid = _validate_nsl(str_source);
	if (err_valid)
		return err_valid;

	err_valid = _validate_nsl(str_dest);
	if (err_valid)
		return err_valid;

	if (str_source->length == 0)
		return STR_SUCCESS;

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
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

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
	int err_valid = _validate_nsl(str_dest);
	if (err_valid)
		return err_valid;

	if (source == NULL)
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

		return err_shift;
	}

	int err_copy = _copy_chars(source, str_dest->str, source_length);
	if (err_copy)
		return err_copy;

	return STR_SUCCESS;
}

enum StringError prepend_nt(String *const str_dest, const char *const source)
{
	int err_valid = _validate_nsl(str_dest);
	if (err_valid)
		return err_valid;

	if (source == NULL)
		return STR_ERR_NULL_ARGUMENT;

	size_t nt_index = 0;
	int err_index = _index_of_nt(source, &nt_index);
	if (err_index)
		return err_index;
	/**
	 * nt_index is the length of the actual characters, because it is the index of the null-terminator.
	 */
	int err_extend = _extend(str_dest, nt_index);
	if (err_extend)
		return err_extend;
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
	int err_valid = _validate_nsl(source);
	if (err_valid)
		return err_valid;

	err_valid = _validate_nsl(dest);
	if (err_valid)
		return err_valid;

	int err_extend = _extend(dest, source->length);
	if (err_extend)
		return err_extend;

	int err_shift = _shift_count_right(dest, source->length);
	if (err_shift)
		return err_shift;

	int err_copy = _copy_chars(source->str, dest->str, source->length);
	if (err_copy)
		return err_copy;

	return STR_SUCCESS;
}

enum StringError duplicate_str(const String *const source, String **const output)
{
	int err_valid = _validate_nsl(source);
	if (err_valid)
		return err_valid;

	if (output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	String *str_duplicate = malloc(sizeof(String));
	if (str_duplicate == NULL)
		return STR_ERR_MEMORY_ALLOCATION;

	char *str_char_arr_duplicate = NULL;
	int err_duplicate = _duplicate_char_array(source->str, source->length, &str_char_arr_duplicate);
	if (err_duplicate)
	{
		free(str_duplicate);
		return err_duplicate;
	}

	str_duplicate->str = str_char_arr_duplicate;
	str_duplicate->length = source->length;
	*output = str_duplicate;

	return STR_SUCCESS;
}

enum StringError get_raw(const String *const source, char **const output)
{
	int err_valid = _validate_nsl(source);
	if (err_valid)
		return err_valid;

	if (output == NULL)
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
	int err_valid = _validate_nsl(source);
	if (err_valid)
		return err_valid;

	if (output == NULL)
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
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

	if (filter == NULL)
		return STR_ERR_NULL_ARGUMENT;

	if (str->length == 0)
		return STR_SUCCESS;

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
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

	if (str->length == 0)
		return STR_SUCCESS;

	for (size_t i = 0; i < str->length; ++i)
	{
		if ((str->str)[i] == to_replace)
			(str->str)[i] = replacement;
	}

	return STR_SUCCESS;
}

enum StringError replace_str(
    String *const str,
    const String *const str_to_replace,
    const String *const str_replacement)
{
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

	err_valid = _validate_nsl(str_to_replace);
	if (err_valid)
		return err_valid;

	err_valid = _validate_nsl(str_replacement);
	if (err_valid)
		return err_valid;

	String *str_updated = NULL;

	/*
	str_replacement->length == 0 => may remove parts from the String
	str->length == str_to_replace->length => may replace the whole String
	The ratio of the length-s of str and str_replacement does not matter.
	*/
	if (str->length == 0 ||
	    str_to_replace->length == 0 ||
	    str_replacement->length == 0 ||
	    str->length < str_to_replace->length)
		goto _case_nothing_to_do;

	DynamicArray *output_split = NULL;
	int err_split = _split_str_pattern_to_empty(str, str_to_replace, &output_split);
	if (err_split)
		return err_split;

	DynamicArrayIterator *dai = NULL;
	if (new_iterator_da(output_split, &dai))
	{
		apply_destructor_da(output_split, (void (*)(void **))free_string);
		return STR_ERR_DYNAMIC_ARRAY;
	}

	int dai_has_next = 0;
	if (has_next_dai(dai, &dai_has_next))
	{
		apply_destructor_da(output_split, (void (*)(void **))free_string);
		return STR_ERR_DYNAMIC_ARRAY;
	}

	while (dai_has_next)
	{
		String *tmp = NULL;
		if (next_ptr_dai(dai, (void **)&tmp))
		{

			apply_destructor_da(output_split, (void (*)(void **))free_string);
			return STR_ERR_DYNAMIC_ARRAY;
		}
		if (0 == tmp->length)
		{
			int err_append = append_str(str_replacement, tmp);
			if (err_append)
			{
				apply_destructor_da(output_split, (void (*)(void **))free_string);
				return err_append;
			}
		}
		if (has_next_dai(dai, &dai_has_next))
		{
			apply_destructor_da(output_split, (void (*)(void **))free_string);
			return STR_ERR_DYNAMIC_ARRAY;
		}
	}

	int err_concat = concat_str_da(&str_updated, output_split);
	if (err_concat)
	{
		apply_destructor_da(output_split, (void (*)(void **))free_string);
		return err_concat;
	}

_end_stage:
	free(str->str);
	str->str = str_updated->str;
	str->length = str_updated->length;
	free(str_updated);						   // str_updated->str must NOT be freed
	apply_destructor_da(output_split, (void (*)(void **))free_string); // Strigns from output_split are copied and can be freed.
_case_nothing_to_do:
	return STR_SUCCESS;
}

enum StringError remove_char(String *const str, const char to_remove)
{
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

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

enum StringError concat_str(String **const output, const size_t n_str, ...)
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
		{
			free_string(&str_result);
			return STR_ERR_NULL_ARGUMENT;
		}
		int err_concat = append_str(str, str_result);
		if (err_concat)
		{
			free_string(&str_result);
			return err_concat;
		}
	}
	va_end(list);

	*output = str_result;

	return STR_SUCCESS;
}

enum StringError concat_str_da(String **const output, DynamicArray *const strings)
{
	if (output == NULL || strings == NULL)
		return STR_ERR_NULL_ARGUMENT;

	if (strings->type != DA_PTR)
		return STR_ERR_INVALID_ARGUMENT_DIMENTIONS;

	String *str_result = NULL;
	int err_result_init = new_string(NULL, 0, &str_result);
	if (err_result_init)
		return err_result_init;

	if (strings->count == 0)
		goto _end_stage;

	DynamicArrayIterator *dai = NULL;
	int err_dai_init = new_iterator_da(strings, &dai);
	if (err_dai_init)
		return STR_ERR_DYNAMIC_ARRAY;

	int has_next = 0;
	int err_has_next = has_next_dai(dai, &has_next);
	if (err_has_next)
		return STR_ERR_DYNAMIC_ARRAY;

	while (has_next)
	{
		String *part = NULL;
		int err_next = next_ptr_dai(dai, (void **)&part);
		if (err_next)
		{
			free_string(&str_result);
			return STR_ERR_DYNAMIC_ARRAY;
		}

		int err_append_str = append_str(part, str_result);
		if (err_append_str)
		{
			free_string(&str_result);
			return err_append_str;
		}

		err_has_next = has_next_dai(dai, &has_next);
		if (err_has_next)
		{
			free_string(&str_result);
			return STR_ERR_DYNAMIC_ARRAY;
		}
	}

_end_stage:
	*output = str_result;
	return STR_SUCCESS;
}

enum StringError concat_str_da_c(
    String **const output,
    DynamicArray *const strings,
    const String *const connector)
{
	int err_valid = _validate_nsl(connector);
	if (err_valid)
		return err_valid;

	if (output == NULL || strings == NULL)
		return STR_ERR_NULL_ARGUMENT;

	if (strings->type != DA_PTR)
		return STR_ERR_INVALID_ARGUMENT_DIMENTIONS;

	String *str_result = NULL;
	int err_result_init = new_string_nt(NULL, &str_result);
	if (err_result_init)
		return err_result_init;

	if (strings->count == 0)
		goto _end_stage;

	DynamicArrayIterator *dai = NULL;
	int err_dai_init = new_iterator_da(strings, &dai);
	if (err_dai_init)
	{
		free_string(&str_result);
		return STR_ERR_DYNAMIC_ARRAY;
	}

	int has_next = 0;
	int err_has_next = has_next_dai(dai, &has_next);
	if (err_has_next)
	{
		free_string(&str_result);
		free_iterator_da(&dai);
		return STR_ERR_DYNAMIC_ARRAY;
	}
_main_loop:
	while (has_next)
	{
		String *part = NULL;
		int err_next = next_ptr_dai(dai, (void **)&part);
		if (err_next)
		{
			free_string(&str_result);
			free_iterator_da(&dai);
			return STR_ERR_DYNAMIC_ARRAY;
		}

		int err_append_str = append_str(part, str_result);
		if (err_append_str)
		{
			free_string(&str_result);
			free_iterator_da(&dai);
			return err_append_str;
		}

		err_has_next = has_next_dai(dai, &has_next);
		if (err_has_next)
		{
			free_string(&str_result);
			free_iterator_da(&dai);
			return STR_ERR_DYNAMIC_ARRAY;
		}

		// If a part is empty "", skip the connector in order to avoid placing two connectors next to each other.
		if (part->length == 0)
			goto _main_loop;

		// Append the connector only if there is a following part.
		if (has_next)
		{
			int err_append_str = append_str(connector, str_result);
			if (err_append_str)
			{
				free_string(&str_result);
				free_iterator_da(&dai);
				return err_append_str;
			}
		}
	}

_end_stage:
	*output = str_result;
	free_iterator_da(&dai);
	return STR_SUCCESS;
}

enum StringError trim(String *const str)
{
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

	if (str->length == 0)
		return STR_SUCCESS;

	size_t left_trim_count = 0;
	size_t right_trim_count = 0;

	for (size_t i = 0; i < str->length; ++i)
	{
		if ((str->str)[i] != ' ')
			break;
		left_trim_count++;
	}
	for (size_t i = str->length - 1; i > 0; --i)
	{
		if ((str->str)[i] != ' ')
			break;
		right_trim_count++;
	}
	// --asdf-- l = 2; r = 2; sum = 4; length = 8; new_length = 4
	// --asdf   l = 2; r = 0; sum = 2; length = 6; new_length = 4
	// asdf--   l = 0; r = 2; sum = 2; length = 6; new_length = 4
	// ----     l = 4; r = 3; sum = 7; length = 4; new_length = 0
	// asdf     l = 0; r = 0; sum = 0; length = 4; new_length = 4
	size_t trim_sum = left_trim_count + right_trim_count;
	if (trim_sum == 0)
		return STR_SUCCESS;

	size_t new_length = trim_sum < str->length ? str->length - trim_sum : 0;

	char *new_str = NULL;
	if (new_length == 0)
	{
		free(str->str);
		goto _end_stage;
	}

	int err_new_str = _new_char_array(new_length, &new_str);
	if (err_new_str)
		return err_new_str;

	for (size_t i = 0; i < new_length; ++i)
		new_str[i] = (str->str)[left_trim_count + i];

	free(str->str);

_end_stage:
	str->str = new_str;
	str->length = new_length;
	return STR_SUCCESS;
}

enum StringError split_str(const String *const str, const String *const pattern, DynamicArray **const output)
{
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

	err_valid = _validate_nsl(pattern);
	if (err_valid)
		return err_valid;

	if (output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	DynamicArray *output_split = NULL;
	int err_split = _split_str_pattern_to_empty(str, pattern, &output_split);
	if (err_split)
		return err_split;

	int error_code = STR_SUCCESS;
	DynamicArray *parts = NULL;
	DynamicArrayIterator *dai = NULL;

	if (new_dynamic_array(DA_PTR, &parts))
	{
		error_code = STR_ERR_DYNAMIC_ARRAY;
		goto _error_case;
	}

	// Extract parts Strings.
	if (new_iterator_da(output_split, &dai))
	{
		error_code = STR_ERR_DYNAMIC_ARRAY;
		goto _error_case;
	}
	int dai_has_next = 0;
	if (has_next_dai(dai, &dai_has_next))
	{
		error_code = STR_ERR_DYNAMIC_ARRAY;
		goto _error_case;
	}
	while (dai_has_next)
	{
		String *tmp = NULL;
		if (next_ptr_dai(dai, (void **)&tmp))
		{
			error_code = STR_ERR_DYNAMIC_ARRAY;
			goto _error_case;
		}

		if (0 != tmp->length)
		{
			if (push_ptr_da(parts, (void *)tmp))
			{
				error_code = STR_ERR_DYNAMIC_ARRAY;
				goto _error_case;
			}
		}

		if (has_next_dai(dai, &dai_has_next))
		{
			error_code = STR_ERR_DYNAMIC_ARRAY;
			goto _error_case;
		}
	}

	// Free memory ONLY of empty Strings. apply_destructor_da would free ALL Strings.
	if (process_da(output_split, (void (*)(void *))_worker_free_empty_str))
	{
		error_code = STR_ERR_DYNAMIC_ARRAY;
		goto _error_case;
	}

_end_stage:
	*output = parts;
	free_iterator_da(&dai);
	free_dynamic_array(&output_split);
	return error_code;

_error_case:
	if (NULL != dai)
		free_iterator_da(&dai);
	if (NULL != output_split)
	{
		apply_destructor_da(output_split, (void (*)(void **))free_string);
		free_dynamic_array(&output_split);
	}
	if (NULL != parts)
		free_dynamic_array(&parts); // String pointers are copied to parts from output_split.

	return error_code;
}

enum StringError includes_str(
    const String *const str,
    const String *const str_search,
    int *const output)
{
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

	err_valid = _validate_nsl(str_search);
	if (err_valid)
		return err_valid;

	if (output == NULL)
		return STR_ERR_NULL_ARGUMENT;

	int result = 0;

	if (str->length == 0 ||
	    str_search->length == 0 ||
	    str->length < str_search->length)
		goto _end_stage;

	size_t index_str = 0;
_main_loop:
	while (index_str < str->length)
	{
		if ((str->str)[index_str] != (str_search->str)[0])
		{
			index_str++;
			goto _main_loop;
		}
		for (size_t i = 0; i < str_search->length; ++i)
		{
			if ((str->str)[index_str + i] != (str_search->str)[i])
			{
				index_str += i;
				goto _main_loop;
			}
		}
		result = 1;
		break;
	}

_end_stage:
	*output = result;
	return STR_SUCCESS;
}

enum StringError slice_str(
    const String *const str,
    const size_t index_start,
    const size_t index_end,
    String **const output)
{
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

	if (NULL == output)
		return STR_ERR_NULL_ARGUMENT;

	String *result_str = NULL;
	int err_result_str = new_string_nt(NULL, &result_str);
	if (err_result_str)
		return err_result_str;

	if (0 == str->length)
		goto _end_stage;

	if (index_start >= index_end || index_end > str->length)
	{
		free_string(&result_str);
		return STR_ERR_INVALID_ARGUMENT_DIMENTIONS;
	}

	int err_append = STR_SUCCESS;
	for (size_t i = index_start; i < index_end; ++i)
	{
		err_append = append_char(result_str, (str->str)[i]);
		if (err_append)
		{
			free_string(&result_str);
			return err_append;
		}
	}

_end_stage:
	*output = result_str;
	return STR_SUCCESS;
}

enum StringError starts_with_nt(
    const String *const str,
    const char *const part,
    int *const output)
{
	int err_valid = _validate_nsl(str);
	if (err_valid)
		return err_valid;

	if (NULL == output)
		return STR_ERR_NULL_ARGUMENT;

	int result = 1;

	size_t part_length = 0;
	int err_index = _index_of_nt(part, &part_length);
	if (err_index)
		return err_index;

	if (0 == part_length || part_length > str->length)
		goto _negative_result;

	for (size_t i = 0; i < part_length; ++i)
	{
		if ((str->str)[i] != part[i])
		{
			result = 0;
			break;
		}
	}

	*output = result;
	return STR_SUCCESS;

_negative_result:
	*output = 0;
	return STR_SUCCESS;
}