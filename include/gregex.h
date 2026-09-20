/**
 * @file gregex.h
 * @brief Header file for regex implementation.
 */

#ifndef GALXLIB_GREGEX_H
#define GALXLIB_GREGEX_H

#include <stddef.h>
#include <regex.h>
#include "./gstring.h"
#include "./error.h"

#ifdef _WIN32

#ifdef GALXLIB_SHARED

#ifdef GALXLIB_EXPORTS
#define GALXLIB_API __declspec(dllexport)
#else
#define GALXLIB_API __declspec(dllimport)
#endif

#else
#define GALXLIB_API
#endif

#else
#define GALXLIB_API

#endif

/*
Documentation for regex.h
https://pubs.opengroup.org/onlinepubs/009696899/functions/regcomp.html
*/

/**
 * @struct RegexContainer
 * @brief A container for all regex related data.
 */
typedef struct RegexContainer
{
    regex_t *regex;      /**< A ponter to the compiled regex_t structure, containing the compiled pattern buffer of the regex. */
    size_t max_groups;   /**< The number of gpoups, which the regex will hold. The group with index 0 is the whole matched string. */
    regmatch_t *groups;  /**< A ponter to the groups specified in the regex pattern. The groups count will equal max_groups. The groups counting stats from 0 - index based. The first group (index 0) indicates the whole matched_input string. */
    char *matched_input; /**< A pointer to the input string, against which the compiled regex was matched. Initialized as NULL. */
    int is_match;        /**< 1 if a match was made, 0 otherwise. Initialized as 0. The matched_input and is_match properties are changed by the match function. */
} RegexContainer;

/**
 * Creates a new RegexContainer.
 * @param pattern A pointer to the string pattern, which will be compiled into a regex. If NULL, NULL is returned.
 * @param max_groups The maximum count of groups, which will be present in the regex.
 * The first (index 0) is always the whole matched string. Must be 1 or greater.
 * @param compilation_flags The flags to use in the regcomp function. The biwise OR of zero or more regex flags.
 * The flags are defined in the standard C library documentaion under regex: REG_EXTENDED, REG_ICASE, REG_NOSUB and REG_NEWLINE.
 * @return A value of the @ref RegexContainerError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_MEMORY_ALLOCATION
 *
 * - GLX_ERR_PATTERN_COMPILATION
 */
GALXLIB_API enum GalxlibError new_regex_container(
    const char *const pattern,
    const size_t max_groups,
    const int compilation_flags,
    RegexContainer **const output);

/**
 * Frees the memory for the RegexContainer.
 * @param container The pointer to the RegexContainer, which is to be freed. The rerefenced pointer is set to NULL.
 * @return A value of the @ref RegexContainerError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum GalxlibError free_regex_container(RegexContainer **container);

/**
 * Attempts to match the given null-terminated character array against the previously compiled pattern buffer (regex) in the @ref RegexContainer.
 * @param container The RegexContainer holding all regex-related data, including the compiled pattern buffer (regex).
 * @param input The null-terminated character array to match against the pattern.
 * @param execution_flags The flag to give to the regexec function. The biwise OR of zero or more regex flags.
 * The flags are defined in the standard C library documentaion under regex: REG_NOTBOL, REG_NOTEOL and REG_STARTEND.
 * @return A value of the @ref RegexContainerError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum GalxlibError match(RegexContainer *const container, char *const input, const int execution_flags);

/**
 * Attempts to match the given String against the regex in the RegexContainer.
 * @param container The RegexContainer holding all regex related data, including the compiled regex pattern.
 * @param str The @ref String to match.
 * @param execution_flags The flag to give to the regexec function. The biwise OR of zero or more regex flags.
 * The flags are defined in the standard C library documentaion under regex: REG_NOTBOL, REG_NOTEOL and REG_STARTEND.
 * @return A value of the @ref RegexContainerError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum GalxlibError match_str(RegexContainer *const container, const String *const str, const int execution_flags);

/**
 * Get the matched character array from a group. This is the raw match, without adding a null-terminator.
 * @param container Pointer to the RegexContainer with the relevant data.
 * The is_match property of the RegexContainer must be equal to 1.
 * @param group_index The index of the group indicating the wanted value, from the groups property of the RegexContainer.
 * Must be 0 or less than the max_groups property of the RegexContainer.
 * @param output A pointer to where the matched character array will be placed.
 * @param output_length A pointer to where the length of the matched character array will be placed.
 * @return A value of the @ref RegexContainerError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_INVALID_ARGUMENT_DIMENTIONS
 *
 * - GLX_ERR_NO_MATCH
 *
 * - GLX_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum GalxlibError get_group_value(
    const RegexContainer *const container,
    const size_t group_index,
    char **const output,
    size_t *const output_length);

/**
 * Get the matched string from a group as a null-terminated character array.
 * @param container Pointer to the RegexContainer with the relevant data.
 * The is_match property of the RegexContainer must be equal to 1.
 * @param group_index The index of the group indicating the wanted value, from the groups property of the RegexContainer.
 * Must be 0 or less than the max_groups property of the RegexContainer.
 * @param output A pointer to where the matched character array will be placed.
 * @param output_length A pointer to where the length of the matched character array will be placed.
 * @return A value of the @ref RegexContainerError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_INVALID_ARGUMENT_DIMENTIONS
 *
 * - GLX_ERR_NO_MATCH
 *
 * - GLX_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum GalxlibError get_group_value_nt(
    const RegexContainer *const container,
    const size_t group_index,
    char **const output,
    size_t *const output_length);

/**
 * Get the matched character array from a group as a String.
 * @param container Pointer to the RegexContainer with the relevant data.
 * The is_match property of the RegexContainer must be equal to 1.
 * @param group_index The index of the group indicating the wanted value, from the groups property of the RegexContainer.
 * Must be 0 or less than the max_groups property of the RegexContainer.
 * @param output A pointer to where the String will be placed.
 * @return A value of the @ref RegexContainerError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_INVALID_ARGUMENT_DIMENTIONS
 *
 * - GLX_ERR_NO_MATCH
 *
 * - GLX_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum GalxlibError get_group_value_str(
    const RegexContainer *const container,
    const size_t group_index,
    String **const output);

#endif