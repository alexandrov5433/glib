#ifndef GALXLIB_GREGEX_H
#define GALXLIB_GREGEX_H

#include <stddef.h>
#include <regex.h>
#include "./gstring.h"

#ifdef _WIN32
#ifdef GALXLIB_EXPORTS
#define GALXLIB_API __declspec(dllexport)
#else
#define GALXLIB_API __declspec(dllimport)
#endif
#else
#define GALXLIB_API
#endif

/*
Documentation for regex.h
https://pubs.opengroup.org/onlinepubs/009696899/functions/regcomp.html
*/

/**
 * @enum RegexError
 * @brief The error codes returned by the RegexContainer functions.
 */
enum RegexContainerError
{
	RC_SUCCESS = 0,				/**< (0) Successful execution of the called function. */
	RC_ERR_NULL_ARGUMENT = 1,		/**< (1) One or more arguments are NULL. */
	RC_ERR_MEMORY_ALLOCATION = 2,		/**< (2) Failed to allocate or reallocate memory. */
	RC_ERR_PATTERN_COMPILATION = 3,		/**< (3) Failed to compile the regex pattern using the regcomp function. */
	RC_ERR_INVALID_ARGUMENT_DIMENTIONS = 4, /**< (4) The dimentions of one or more arguments, either alone or in their combination, do not match the expectations of the function. */
	RC_ERR_NO_MATCH = 5,			/**< (5) The given RegexContainer has not matched a string (character array or String). */
};

/**
 * @struct RegexContainer
 * @brief A container for all regex related data.
 */
typedef struct RegexContainer
{
	regex_t *regex;	     /**< A ponter to the compiled regex. */
	size_t max_groups;   /**< The number of gpoups, which the regex will hold. */
	regmatch_t *groups;  /**< A ponter to the groups specified in the regex pattern. The groups count will equal max_groups. The groups counting stats from 0 - index based. The first group (index 0) indicates the whole matched_input string. */
	char *matched_input; /**< A pointer to the input string, against which the compiled regex was matched. Initialized as NULL. */
	int is_match;	     /**< 1 if a match was made, 0 otherwise. Initialized as 0. The matched_input and is_match properties are changed by the match function. */
} RegexContainer;

/**
 * Creates a new RegexContainer.
 * @param pattern A pointer to the string pattern, which will be compiled into a regex. If NULL, NULL is returned.
 * @param max_groups The maximum count of groups, which will be present in the regex.
 * The first (index 0) is always the whole matched string.
 * @param flag A regex flag to use in the regcomp function.
 * Example REG_EXTENDED (int 1) for compiling an Extended Regular Expression.
 * @return A value of the @ref RegexContainerError:
 *
 * - RC_SUCCESS
 *
 * - RC_ERR_NULL_ARGUMENT
 *
 * - RC_ERR_MEMORY_ALLOCATION
 *
 * - RC_ERR_PATTERN_COMPILATION
 */
GALXLIB_API enum RegexContainerError new_regex_container(
    const char *const pattern,
    const size_t max_groups,
    const int flag,
    RegexContainer **const output);

/**
 * Frees the memory for the RegexContainer.
 * @param container The pointer to the RegexContainer, which is to be freed.
 * @return A value of the @ref RegexContainerError:
 *
 * - RC_SUCCESS
 *
 * - RC_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum RegexContainerError free_regex_container(RegexContainer *container);

/**
 * Attempts to match the given character array against the regex in the RegexContainer.
 * @param input The null-terminated character array to match.
 * @param container The RegexContainer holding all regex related data, including the compiled regex pattern.
 * @return A value of the @ref RegexContainerError:
 *
 * - RC_SUCCESS
 *
 * - RC_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum RegexContainerError match(RegexContainer *const container, char *const input);

/**
 * Attempts to match the given String against the regex in the RegexContainer.
 * @param str The String to match.
 * @param container The RegexContainer holding all regex related data, including the compiled regex pattern.
 * @return A value of the @ref RegexContainerError:
 *
 * - RC_SUCCESS
 *
 * - RC_ERR_NULL_ARGUMENT
 *
 * - RC_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum RegexContainerError match_str(RegexContainer *const container, const String *const str);

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
 * - RC_SUCCESS
 *
 * - RC_ERR_NULL_ARGUMENT
 *
 * - RC_ERR_INVALID_ARGUMENT_DIMENTIONS
 *
 * - RC_ERR_NO_MATCH
 *
 * - RC_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum RegexContainerError get_group_value(
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
 * - RC_SUCCESS
 *
 * - RC_ERR_NULL_ARGUMENT
 *
 * - RC_ERR_INVALID_ARGUMENT_DIMENTIONS
 *
 * - RC_ERR_NO_MATCH
 *
 * - RC_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum RegexContainerError get_group_value_nt(
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
 * - RC_SUCCESS
 *
 * - RC_ERR_NULL_ARGUMENT
 *
 * - RC_ERR_INVALID_ARGUMENT_DIMENTIONS
 *
 * - RC_ERR_NO_MATCH
 *
 * - RC_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum RegexContainerError get_group_value_str(
    const RegexContainer *const container,
    const size_t group_index,
    String **const output);

#endif