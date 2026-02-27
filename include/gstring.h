#include <stdlib.h>

#ifndef STRING_H
#define STRING_H

#ifdef _WIN32
#ifdef GLIB_EXPORTS
#define GLIB_API __declspec(dllexport)
#else
#define GLIB_API __declspec(dllimport)
#endif
#else
#define GLIB_API
#endif

#ifndef STRING_LOOP_MAX_LIMIT
#define STRING_LOOP_MAX_LIMIT 2100000000L
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
 * @param str An array of characters.
 * @param length The number of characters.
 */
typedef struct String
{
    size_t length;
    char *str;
} String;

/**
 * Creates a new String.
 * @param charArr A pointer to a character array. If NULL, a String with 0 length will be created.
 * The content of the array is copied, therefore it can be freed.
 * @param length The number of characters, which will be copied to the String.
 * If 0, a String with 0 length will be created and nothing is copied.
 * @returns A pointer to the new String on success.
 * NULL on failure, because of failed memory allocation or failed character coping.
 */
GLIB_API String *new_string(const char *const charArr, size_t length);

/**
 * Frees the memory of the String.
 * @param str A pointer to the String, which must be freed. If NULL, nothing is done.
 */
GLIB_API void free_string(String *str);

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
GLIB_API int append_char(char c, String *const str);

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
GLIB_API int append_char_array(const char *const charArr, size_t copyCount, String *const str);

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
GLIB_API int append_nt(const char *const charArr, String *const str);

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
GLIB_API int append_str(const String *const source, String *const dest);

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
GLIB_API int prepend_char(char c, String *const str);

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
GLIB_API int prepend_char_array(const char *const charArr, size_t copyCount, String *const str);

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
GLIB_API int prepend_nt(const char *const charArr, String *const str);

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
GLIB_API int prepend_str(const String *const source, String *const dest);

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
GLIB_API int duplicate_str(const String *const source, String **const output);

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
GLIB_API int get_str(const String *const source, char **const output);

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
GLIB_API int get_str_nt(const String *const source, char **const output);

#endif