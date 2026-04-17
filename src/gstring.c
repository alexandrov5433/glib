#include <stdlib.h>
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

	int err = _extend(str, 1);
	if (err)
		return err;

	for (size_t i = str->length - 1; i > 0; --i)
		(str->str)[i] = (str->str)[i - 1];

	return 0;
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
static enum StringError _copy_chars(const char *const source, char *const dest, size_t const length)
{
	if (source == NULL || dest == NULL)
		return STR_ERR_NULL_ARGUMENT;

	for (size_t i = 0; i < length; ++i)
		dest[i] = source[i];

	return STR_SUCCESS;
}

static int _duplicate_char_array(const char *const char_arr, size_t const length, char **const output)
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

int append_char(char c, String *const str)
{
	if (str == NULL)
		return 1;

	int err = _extend(1, str);
	if (err)
		return err;

	(str->str)[str->length - 1] = c;

	return 0;
}

int append_char_array(const char *const charArr, size_t copyCount, String *const str)
{
	if (charArr == NULL || str == NULL)
		return 1;

	int err = _extend(copyCount, str);
	if (err)
		return err;

	for (size_t i = 0; i < copyCount; ++i)
	{
		(str->str)[str->length - copyCount + i] = charArr[i];
	}

	return 0;
}

int append_nt(const char *const charArr, String *const str)
{
	if (charArr == NULL || str == NULL)
		return 1;

	long ntIndex = _index_of_nt(charArr);
	if (ntIndex < 0)
		return 4;

	// ntIndex is the length of the actual characters, because it is the index of the null-terminator
	size_t extraSpace = ntIndex;
	int err = _extend(extraSpace, str);
	if (err)
		return err;

	for (size_t i = 0; i < ntIndex; ++i)
	{
		(str->str)[str->length - ntIndex + i] = charArr[i];
	}

	return 0;
}

int append_str(const String *const source, String *const dest)
{
	if (source == NULL || dest == NULL)
		return 1;

	size_t oldDestLength = dest->length;

	int err = _extend(source->length, dest);
	if (err)
		return err;

	for (size_t i = 0; i < source->length; ++i)
	{
		(dest->str)[oldDestLength + i] = (source->str)[i];
	}

	return 0;
}

int prepend_char(char c, String *const str)
{
	if (str == NULL)
		return 1;

	int err = _shift_one_right(str);
	if (err)
		return err;

	(str->str)[0] = c;

	return 0;
}

int prepend_char_array(const char *const charArr, size_t copyCount, String *const str)
{
	if (charArr == NULL || str == NULL)
		return 1;

	// TODO: on error revert and restore before returning
	int err = _shift_count_right(copyCount, str);
	if (err)
		return err;

	_copy_chars(charArr, str->str, copyCount);

	return 0;
}

int prepend_nt(const char *const charArr, String *const str)
{
	if (charArr == NULL || str == NULL)
		return 1;

	long ntIndex = _index_of_nt(charArr);
	if (ntIndex < 0)
		return 4;

	// ntIndex is the length of the actual characters, because it is the index of the null-terminator
	size_t copyLength = ntIndex;
	int err = _shift_count_right(copyLength, str);
	if (err)
		return err;

	_copy_chars(charArr, str->str, copyLength);

	return 0;
}

int prepend_str(const String *const source, String *const dest)
{
	if (source == NULL || dest == NULL)
		return 1;

	int err = _shift_count_right(source->length, dest);
	if (err)
		return err;

	_copy_chars(source->str, dest->str, source->length);

	return 0;
}

int duplicate_str(const String *const source, String **const output)
{
	if (source == NULL)
		return 1;

	String *newStr = malloc(sizeof(String));
	if (newStr == NULL)
		return 2;

	char *str = NULL;
	int err = _duplicate_char_array(source->str, source->length, &str);
	if (err)
	{
		free(newStr);
		return err;
	}

	newStr->str = str;
	newStr->length = source->length;
	*output = newStr;

	return 0;
}

int get_raw(const String *const source, char **const output)
{
	if (source == NULL)
		return 1;

	char *result = NULL;
	int duplicationErr = _duplicate_char_array(source->str, source->length, &result);
	if (duplicationErr)
		return duplicationErr;

	*output = result;

	return 0;
}

int get_raw_nt(const String *const source, char **const output)
{
	if (source == NULL)
		return 1;

	String *tempCopy = NULL;
	int err = duplicate_str(source, &tempCopy);
	if (err)
		return err;

	int appendErr = append_char('\0', tempCopy);
	if (appendErr)
	{
		free_string(tempCopy);
		return appendErr;
	}

	char *result = NULL;
	int duplicationErr = _duplicate_char_array(tempCopy->str, tempCopy->length, &result);
	if (duplicationErr)
	{
		free_string(tempCopy);
		return duplicationErr;
	}

	*output = result;

	free_string(tempCopy);
	return 0;
}

int filter_str(String *const str, int (*filter)(char c))
{
	if (str == NULL || filter == NULL)
		return 1;

	char *wantedChars = _new_char_array(str->length);
	size_t wantedCount = 0;
	if (wantedChars == NULL)
		return 2;

	// filter return: 1 -> stay; 0 -> remove
	for (size_t i = 0; i < str->length; ++i)
	{
		char c = (str->str)[i];
		if (filter(c))
		{
			// filter returned 1; char is wanted
			wantedChars[wantedCount++] = c;
		}
	}

	// wantedCount is now the length, because of the last post-incrementation
	char *resizedWantedChars = realloc(wantedChars, sizeof(char) * wantedCount);
	if (resizedWantedChars == NULL)
		return 2;

	free(str->str);
	str->str = resizedWantedChars;
	str->length = wantedCount;

	return 0;
}

int replace_char(String *const str, const char to_replace, const char replacement)
{
	if (str == NULL)
		return 1;

	if (str->length == 0)
		return 0;

	for (size_t i = 0; i < str->length; ++i)
	{
		if ((str->str)[i] == to_replace)
			(str->str)[i] = replacement;
	}

	return 0;
}

int remove_char(String *const str, const char to_remove)
{
	if (str == NULL)
		return 1;

	if (str->length == 0)
		return 0;

	char *tmp_str = _new_char_array(str->length);
	if (tmp_str == NULL)
		return 2;

	size_t tmp_str_length = 0;
	for (size_t i = 0; i < str->length; ++i)
	{
		if ((str->str)[i] != to_remove)
			tmp_str[tmp_str_length++] = (str->str)[i];
	}

	char *new_str = realloc(tmp_str, sizeof(char) * tmp_str_length);
	if (new_str == NULL)
		return 3;

	free(str->str);
	str->str = new_str;
	str->length = tmp_str_length;

	return 0;
}