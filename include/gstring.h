#ifndef GALXLIB_STRING_H
#define GALXLIB_STRING_H

#include <stddef.h>
#include <stdint.h>

#ifdef _WIN32
#ifdef GALXLIB_EXPORTS
#define GALXLIB_API __declspec(dllexport)
#else
#define GALXLIB_API __declspec(dllimport)
#endif
#else
#define GALXLIB_API
#endif

#ifndef GSTRING_LOOP_MAX_LIMIT
#define GSTRING_LOOP_MAX_LIMIT 2000000000UL
#endif

#ifndef ANSI_COLOR_RED
#define ANSI_COLOR_RED "\x1b[31m"
#endif

#ifndef ANSI_COLOR_GREEN
#define ANSI_COLOR_GREEN "\x1b[32m"
#endif

#ifndef ANSI_COLOR_YELLOW
#define ANSI_COLOR_YELLOW "\x1b[33m"
#endif

#ifndef ANSI_COLOR_BLUE
#define ANSI_COLOR_BLUE "\x1b[34m"
#endif

#ifndef ANSI_COLOR_MAGENTA
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#endif

#ifndef ANSI_COLOR_CYAN
#define ANSI_COLOR_CYAN "\x1b[36m"
#endif

#ifndef ANSI_COLOR_RESET
#define ANSI_COLOR_RESET "\x1b[0m"
#endif

/**
 * @enum StringError
 * @brief The error codes returned by the String functions.
 */
enum StringError
{
    STR_SUCCESS = 0,                         /**< (0) Successful execution of the called function. */
    STR_ERR_NULL_ARGUMENT = 1,               /**< (1) One or more arguments are NULL. */
    STR_ERR_MEMORY_ALLOCATION = 2,           /**< (2) Failed to allocate or reallocate memory. */
    STR_ERR_INDEX_OUT_OF_BOUNDS = 3,         /**< (3) The targeted index is outside of the boundaries of the String. */
    STR_ERR_INVALID_ARGUMENT_DIMENTIONS = 4, /**< (4) The dimentions of one or more arguments, either alone or in their combination, do not match the expectations of the function. */
    STR_ERR_LOOP_MAX_LIMIT = 5,              /**< (5) A null-terminator character '\0' was not found among the first GSTRING_LOOP_MAX_LIMIT characters of the given character array. */
};

/**
 * @struct String
 * @brief The structure containing the actual character array and number of elements contained in it.
 */
typedef struct String
{
    size_t length; /**< The number of characters in the array. */
    char *str;     /**< The character array. */
} String;

/**
 * Creates a new String.
 * @param char_arr A pointer to a character array. The content of the character array is copied, therefore it can be freed.
 * If NULL, length argument must be 0 and a String with 0 length will be created.
 * @param length The number of characters, which will be copied to the String.
 * If 0, char_arr argument must be NULL and a String with 0 length will be created.
 * @return A value of the @ref StringError:
 *
 * - STR_SUCCESS
 *
 * - STR_ERR_INVALID_ARGUMENT_DIMENTIONS
 *
 * - STR_ERR_MEMORY_ALLOCATION
 *
 * - STR_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum StringError new_string(const char *const char_arr, size_t length, String **const output);

/**
 * Frees the memory of the String.
 * @param str A pointer to the String, which must be freed.
 * @return A value of the @ref StringError:
 *
 * - STR_SUCCESS
 *
 * - STR_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum StringError free_string(String *str);

/**
 * Appends the character to the end of the String.
 * @param c The character to append.
 * @param str A pointer to the String, to which the character must be appended.
 * @returns 0 on success. On failure:
 *
 * 1 if the str argument is NULL.
 *
 * 2 if memory could not be allocated.
 *
 * 3 if the updated size of the String is not sufficient.
 */
GALXLIB_API int append_char(char c, String *const str);

/**
 * Appends a given count of the characters of a character array to the String.
 * @param charArr A character array containig the characters, which will be appended.
 * @param copyCount This is the number of characters, which will be copied, starting from index 0.
 * @param str A pointer to the String, to which the characters will be appended.
 * @returns 0 on success. On failure:
 *
 * 1 if either of the charArr or str arguments is NULL.
 *
 * 2 if memory could not be allocated.
 *
 * 3 if the updated size of the String is not sufficient.
 */
GALXLIB_API int append_char_array(const char *const charArr, size_t copyCount, String *const str);

/**
 * Appends all the characters of a character array, until a null-terminator '\0', to the end of the String.
 * @param charArr A character array containig the characters, which will be appended.
 * The charArr must be null-terminated.
 * If a null-terminator '\0' is not found among the first STRING_LOOP_MAX_LIMIT characters, error code 4 is returned.
 * @param str A pointer to the String, to which the characters will be appended.
 * @returns 0 on success. On failure:
 *
 * 1 if either of the arguments is NULL.
 *
 * 2 if memory could not be allocated.
 *
 * 3 if the updated size of the String is not sufficient.
 *
 * 4 if a null-teminator '\0' was not found among the first STRING_LOOP_MAX_LIMIT characters.
 */
GALXLIB_API int append_nt(const char *const charArr, String *const str);

/**
 * Appends all the characters from one String to the other.
 * @param source A pointer to the String, from which the characters will copied.
 * @param dest A pointer to the String, to which the characters will be appended.
 * @returns 0 on success. On failure:
 *
 * 1 if either of the arguments is NULL.
 *
 * 2 if memory could not be allocated.
 *
 * 3 if the updated size of the String is not sufficient.
 */
GALXLIB_API int append_str(const String *const source, String *const dest);

/**
 * Prepends the character to the start of the String.
 * @param c The character to prepend.
 * @param str A pointer to the String, to which the character must be prepended.
 * @returns 0 on success. On failure:
 *
 * 1 if the str argument is NULL.
 *
 * 2 if memory could not be allocated.
 *
 * 3 if the updated size of the String is not sufficient.
 */
GALXLIB_API int prepend_char(char c, String *const str);

/**
 * Prepends a given count of the characters of a character array to the start of the String.
 * @param charArr A character array containig the characters, which will be prepended.
 * @param copyCount This is the number of characters, which will be copied, starting from index 0.
 * @param str A pointer to the String, to which the characters will be prepended.
 * @returns 0 on success. On failure:
 *
 * 1 if either of the charArr or str arguments is NULL.
 *
 * 2 if memory could not be allocated.
 */
GALXLIB_API int prepend_char_array(const char *const charArr, size_t copyCount, String *const str);

/**
 * Prepends all the characters of a character array, until a null-terminator '\0, to the start of the String.
 * @param charArr A character array containig the characters, which will be prepended.
 * The charArr must be null-terminated.
 * If a null-terminator '\0' is not found among the first STRING_LOOP_MAX_LIMIT characters, error code 4 is returned.
 * @param str A pointer to the String, to which the characters will be prepended.
 * @returns 0 on success. On failure:
 *
 * 1 if either of the arguments is NULL.
 *
 * 2 if memory could not be allocated.
 *
 * 4 if a null-teminator '\0' was not found among the first STRING_LOOP_MAX_LIMIT characters.
 */
GALXLIB_API int prepend_nt(const char *const charArr, String *const str);

/**
 * Prepends all the characters from one String to the start of the other.
 * @param source A pointer to the String, from which the characters will copied.
 * @param dest A pointer to the String, to which the characters will be prepended.
 * @returns 0 on success. On failure:
 *
 * 1 if either of the arguments is NULL.
 *
 * 2 if memory could not be allocated.
 */
GALXLIB_API int prepend_str(const String *const source, String *const dest);

/**
 * Duplicates a String.
 * @param source A pointer to the String, which will be duplicated.
 * @param output A double String pointer, where the address of the duplicate will be placed.
 * @returns 0 on success. On failure:
 *
 * 1 if the source argument is NULL.
 *
 * 2 if memory could not be allocated.
 */
GALXLIB_API int duplicate_str(const String *const source, String **const output);

/**
 * Gives a copy of the character array found in the String.
 * @param source A pointer to the String, from which the character array will be copied.
 * @param output A double character array pointer, where the copy will be placed.
 * @returns 0 on success. On failure:
 *
 * 1 if the source argument is NULL.
 *
 * 2 if memory could not be allocated.
 */
GALXLIB_API int get_raw(const String *const source, char **const output);

/**
 * Gives a null-terminated copy of the character array found in the String. String->str = "abc"; output = "abc\0"
 * @param source A pointer to the String, from which the character array will be copied.
 * @param output A double character array pointer, where the copy will be placed.
 * @returns 0 on success. On failure:
 *
 * 1 if the source argument is NULL.
 *
 * 2 if memory could not be allocated.
 *
 * 3 if the updated size of the String is not sufficient.
 */
GALXLIB_API int get_raw_nt(const String *const source, char **const output);

/**
 * Filters the characters in the String.
 * @param str A pointer to the String, which must be filtered.
 * @param filter A function pointer to the function, which will select which characters are to stay.
 * The filter returns 1 if the character is to STAY, 0 otherwise.
 * @returns 0 on success. On failure:
 *
 * 1 if the source argument is NULL.
 *
 * 2 if memory could not be allocated.
 */
GALXLIB_API int filter_str(String *const str, int (*filter)(char c));

/**
 * Replaces all characters in the String, which match, with the given character.
 * If the length property of the String is 0, nothing is done an 0 is returned.
 * The length property of the String is not changed.
 * @param str A pointer to the String, which must be processed.
 * @param to_replace The character which must be replaced.
 * @param replacement The character which will be placed on the index of the character to_replace.
 * @returns 0 on success. On failure:
 *
 * 1 if the str argument is NULL.
 */
GALXLIB_API int replace_char(String *const str, const char to_replace, const char replacement);

/**
 * Removes all instances of the character from the String.
 * If the length property of the String is 0, nothing is done an 0 is returned.
 * The new length property of the String is equal to the original length minus the count of the removed character instances.
 * @param str A pointer to the String, which must be processed.
 * @param to_remove The character which must be removed. All instances are removed.
 * @returns 0 on success. On failure:
 *
 * 1 if the str argument is NULL.
 *
 * 2 if memory for a new character array could not be allocated (malloc).
 *
 * 3 if memory could not be reallocated (realloc).
 */
GALXLIB_API int remove_char(String *const str, const char to_remove);

#endif