#include <stdlib.h>
#include "../include/gstring.h"

// ##################   static   ##################

static char *_new_char_array(size_t length)
{
    return (char *)malloc(sizeof(char) * length);
}

static int _extend(size_t extraCharsCount, String *const str)
{
    if (str == NULL)
        return 1;

    size_t newSize = extraCharsCount + str->length;
    if (newSize < str->length || newSize < 0)
        return 3;

    char *newStr = realloc(str->str, newSize);
    if (newStr == NULL)
        return 2;

    str->str = newStr;
    str->length = newSize;
    return 0;
}

static int _shrink(size_t charCountToRemove, String *const str)
{
    if (str == NULL)
        return 1;

    if (str->length < charCountToRemove)
        charCountToRemove = str->length;

    int newSize = str->length - charCountToRemove;

    char *newStr = realloc(str->str, newSize);
    if (newStr == NULL)
        return 2;

    str->str = newStr;
    str->length = newSize;
    return 0;
}

static int _shift_one_right(String *const str)
{
    if (str == NULL)
        return 1;

    int err = _extend(1, str);
    if (err)
        return err;

    for (int i = str->length - 1; i > 0; --i)
    {
        (str->str)[i] = (str->str)[i - 1];
    }

    return 0;
}

/**
 * Also extends memory and modifies the length property of String.
 */
static int _shift_count_right(size_t count, String *const str)
{
    if (str == NULL)
        return 1;

    size_t newLength = count + str->length;

    char *newArr = _new_char_array(newLength);
    if (newArr == NULL)
        return 2;

    for (size_t i = 0; i < count; ++i)
    {
        newArr[i] = '#';
    }

    size_t j = 0;
    for (size_t i = count; i < newLength; ++i)
    {
        newArr[i] = (str->str)[j++];
    }

    free(str->str);
    str->str = newArr;
    str->length = newLength;

    return 0;
}

static long _index_of_nt(const char *const charArr)
{
    for (long i = 0L; i <= STRING_LOOP_MAX_LIMIT; ++i)
    {
        if (charArr[i] == '\0')
            return i;
    }
    return -1L;
}

/**
 * Copies count characters from source to dest.
 * The process starts from index 0 for both source and dest.
 */
static int _copy_chars(const char *const source, char *const dest, size_t count)
{
    if (source == NULL || dest == NULL)
        return 1;

    for (size_t i = 0; i < count; ++i)
    {
        dest[i] = source[i];
    }
    return 0;
}

static int _duplicate_char_array(const char *const charArr, size_t length, char **const output)
{
    if (charArr == NULL)
        return 1;

    char *newArr = _new_char_array(length);
    if (newArr == NULL)
        return 2;

    if (_copy_chars(charArr, newArr, length))
    {
        free(newArr);
        return 1;
    }

    *output = newArr;
    return 0;
}

// ##################   public   ##################

String *new_string(const char *const charArr, size_t length)
{
    String *newString = malloc(sizeof(String));
    if (newString == NULL)
        return NULL;

    char *str = _new_char_array(length);
    if (str == NULL)
    {
        free(newString);
        return NULL;
    }

    if (str == NULL || length == 0)
    {
        newString->str = str;
        newString->length = 0;
        return newString;
    }

    if (_copy_chars(charArr, str, length))
    {
        free(newString);
        free(str);
        return NULL;
    }

    newString->str = str;
    newString->length = length;

    return newString;
}

void free_string(String *str)
{
    if (str == NULL)
        return;

    free(str->str);
    free(str);
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
    if (str == NULL)
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