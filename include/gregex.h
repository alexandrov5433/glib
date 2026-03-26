#ifndef GALXLIB_GREGEX_H
#define GALXLIB_GREGEX_H

#include <stddef.h>
#include <regex.h>
#include "./gstring.h"

/*
Documentation for regex.h
https://pubs.opengroup.org/onlinepubs/009696899/functions/regcomp.html
*/

/**
 * A container for all regex related data.
 * @param regex A ponter to the compiled regex.
 * @param max_groups The number of gpoups, which the regex will hold.
 * @param groups A ponter to the groups specified in the regex pattern. The groups count will equal max_groups.
 * The groups counting stats from 0 - index based. The first group (index 0) indicates the whole matched_input string.
 * @param matched_input A pointer to the input string, against which the compiled regex was matched. Initialized as NULL.
 * @param isMatch 1 if a match was made, 0 otherwise. Initialized as 0.
 *
 * The matched_input and isMatched properties are changed by the match function.
 */
typedef struct RegexContainer
{
	regex_t *regex;
	size_t max_groups;
	regmatch_t *groups;
	char *matched_input;
	int isMatch;
} RegexContainer;

/**
 * Creates a new RegexContainer.
 * @param pattern A pointer to the string pattern, which will be compiled into a regex. If NULL, NULL is returned.
 * @param max_groups The maximum count of groups, which will be present in the regex.
 * The first (index 0) is always the whole matched string.
 * @param flag A regex flag to use in the regcomp function.
 * Example REG_EXTENDED (int 1) for compiling an Extended Regular Expression.
 * @param output_status An integer pointer where an eventual error code will be placed. If NULL, it is not used.
 * @return A pointer to the new RegexContainer.
 */
RegexContainer *new_regex_container(
    const char *const pattern,
    const size_t max_groups,
    const int flag,
    int *const output_status);

/**
 * Frees the memory for the RegexContainer.
 * @param container The pointer to the RegexContainer, which is to be freed. If a NULL pointer is given, nothing is done.
 */
void free_regex_container(RegexContainer *container);

/**
 * Attempts to match the given string against the ragex in the RegexContainer.
 * @param input The string to match.
 * @param container The RegexContainer holding all regex related data, including the compiled regex pattern.
 * @returns 0 on success, 1 if either of the arguments is NULL.
 *
 * On success (returned 0) and if the attempt to match is successful:
 *
 * 1. the isMatch property of the container is set to 1.
 *
 * 2. the matched_input property of the container is set to the given input.
 *
 * On success (returned 0) and on failure to match:
 *
 * 1. the isMatch property of the container is set to 0.
 *
 * 2. the matched_input property of the container is set to NULL.
 */
int match(char *const input, RegexContainer *const container);

/**
 * Get the matched string from a group as a character array. This is the raw match, without adding a null-terminator.
 * @param n The index of the group indicating the wanted value, from the groups property of the RegexContainer.
 * Must be 0 or less than the max_groups property of the RegexContainer, or else NULL is returned.
 * @param container Pointer to the RegexContainer with the relevant data. If NULL, NULL is returned.
 * If the isMatch property of the container is 0, NULL is returned.
 * @param output_match_length A pointer to an integer, where the length of the matched string will be placed.
 * If NULL, the output_match_length is ignored.
 * @returns The pointer to the string, selected by the group.
 * The returned pointer may be used in the free function, as it's memory is manualy allocated.
 */
char *get_group_value(const int n, const RegexContainer *const container, int *const output_match_length);

/**
 * Get the matched string from a group as a null-terminated character array.
 * @param n The index of the group indicating the wanted value, from the groups property of the RegexContainer.
 * Must be 0 or less than the max_groups property of the RegexContainer, or else NULL is returned.
 * @param container Pointer to the RegexContainer with the relevant data. If NULL, NULL is returned.
 * If the isMatch property of the container is 0, NULL is returned.
 * @param output_match_length A pointer to an integer, where the length of the matched string will be placed.
 * If NULL, the output_match_length is ignored. The null-terminator is at index equal to output_match_length.
 * @returns The pointer to the string, selected by the group.
 * The returned pointer may be used in the free function, as it's memory is manualy allocated.
 */
char *get_group_value_nt(const int n, const RegexContainer *const container, int *const output_match_length);

/**
 * Get the matched string from a group as a String (from gstring.h).
 * @param n The index of the group indicating the wanted value, from the groups property of the RegexContainer.
 * Must be 0 or less than the max_groups property of the RegexContainer, or else NULL is returned.
 * @param container Pointer to the RegexContainer with the relevant data. If NULL, NULL is returned.
 * If the isMatch property of the container is 0, NULL is returned.
 * @returns The pointer to the String containing the string, selected by the group.
 * If a new String instance could not be created (new_string returned NULL), NULL is returned.
 */
String *get_group_value_str(const int n, const RegexContainer *const container);

#endif