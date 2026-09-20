#include "../include/gregex.h"
#include "../include/gstring.h"
#include <regex.h>
#include <stdlib.h>

enum RegexContainerError free_regex_container(RegexContainer **container)
{
        if (NULL == container)
                return RC_ERR_NULL_ARGUMENT;

        if (NULL == (*container))
                return RC_SUCCESS;

        if ((*container)->regex != NULL)
                regfree((*container)->regex);

        if ((*container)->groups != NULL)
                free((*container)->groups);

        if ((*container)->matched_input != NULL)
                free((*container)->matched_input);

        free(*container);
        *container = NULL;

        return RC_SUCCESS;
}

enum RegexContainerError new_regex_container(
    const char *const pattern,
    const size_t max_groups,
    const int compilation_flags,
    RegexContainer **const output)
{
        if (NULL == pattern || NULL == output)
                return RC_ERR_NULL_ARGUMENT;

        enum RegexContainerError rv = RC_SUCCESS;

        RegexContainer *container = malloc(sizeof(RegexContainer));
        if (container == NULL)
                return RC_ERR_MEMORY_ALLOCATION;

        regex_t *regex = malloc(sizeof(regex_t));
        if (NULL == regex)
        {
                rv = RC_ERR_MEMORY_ALLOCATION;
                goto _err;
        }

        regmatch_t *groups = malloc(max_groups * sizeof(regmatch_t));
        if (NULL == groups)
        {
                rv = RC_ERR_MEMORY_ALLOCATION;
                goto _err;
        }

        if (regcomp(regex, pattern, compilation_flags))
        {
                rv = RC_ERR_PATTERN_COMPILATION;
                goto _err;
        }

        container->regex = regex;
        container->max_groups = max_groups;
        container->groups = groups;
        container->matched_input = NULL;
        container->is_match = 0;

        *output = container;

        return rv;

_err:
        free_regex_container(&container);

        return rv;
}

enum RegexContainerError match(
        RegexContainer *const container, 
        char *const input, 
        const int execution_flags)
{
        if (NULL == container || NULL == input)
                return RC_ERR_NULL_ARGUMENT;
        if (NULL == container->regex)
                return RC_ERR_MISSING_COMPILED_PATTERN_BUFFER;
        if (NULL == container->groups)
                return RC_ERR_MISSING_MATCH_GROUPS;

        /**
         * The regexec() function compares the null-terminated string specified by string with the compiled
         * regular expression preg initialized by a previous call to regcomp().
         * If it finds a match, regexec() shall return 0; otherwise, it shall return non-zero indicating
         * either no match or an error. The eflags argument is the bitwise-inclusive
         * OR of zero or more of the following flags, which are defined in the <regex.h> header:
         * Returns: 0 on success, REG_NOMATCH (== _REG_NOMATCH == 1) on failure and an error code on error.
         */
        int status = regexec(container->regex, input, container->max_groups, container->groups, execution_flags);
        if (0 == status)
        {
                container->is_match = 1;
                container->matched_input = status == 0 ? input : NULL;
        }
        else if (REG_NOMATCH == status)
        {
                container->is_match = 0;
        }
        else
        {
                return RC_ERR_EXECUTION;
        }

        return RC_SUCCESS;
}

enum RegexContainerError match_str(
        RegexContainer *const container, 
        const String *const str, 
        const int execution_flags)
{
        if (str == NULL || container == NULL)
                return RC_ERR_NULL_ARGUMENT;
        /**
         * The regexec() function compares the null-terminated string specified by string with the compiled
         * regular expression preg initialized by a previous call to regcomp().
         * If it finds a match, regexec() shall return 0; otherwise, it shall return non-zero indicating
         * either no match or an error. The eflags argument is the bitwise-inclusive
         * OR of zero or more of the following flags, which are defined in the <regex.h> header:
         */
        char *str_nt = NULL;
        int err_str = get_raw_nt(str, &str_nt);
        if (err_str == STR_ERR_NULL_ARGUMENT)
                return RC_ERR_NULL_ARGUMENT;
        else if (err_str == STR_ERR_MEMORY_ALLOCATION)
                return RC_ERR_MEMORY_ALLOCATION;

        int status = regexec(container->regex, str_nt, container->max_groups, container->groups, execution_flags);
        if (0 == status)
        {
                container->is_match = 1;
                container->matched_input = status == 0 ? str_nt : NULL;
        }
        else if (REG_NOMATCH == status)
        {
                container->is_match = 0;
        }
        else
        {
                return RC_ERR_EXECUTION;
        }

        return RC_SUCCESS;
}

enum RegexContainerError get_group_value(
    const RegexContainer *const container,
    const size_t group_index,
    char **const output,
    size_t *const output_length)
{
        if (container == NULL || output == NULL || output_length == NULL)
                return RC_ERR_NULL_ARGUMENT;
        if (group_index < 0 || group_index >= container->max_groups)
                return RC_ERR_INVALID_ARGUMENT_DIMENTIONS;
        if (container->is_match == 0)
                return RC_ERR_NO_MATCH;

        size_t length_no_nt = (container->groups)[group_index].rm_eo - (container->groups)[group_index].rm_so;
        char *value = malloc(length_no_nt * sizeof(char));
        if (value == NULL)
                return RC_ERR_MEMORY_ALLOCATION;
        for (size_t i = 0; i < length_no_nt; ++i)
                value[i] = (container->matched_input)[(container->groups)[group_index].rm_so + i];

        *output = value;
        *output_length = length_no_nt;

        return RC_SUCCESS;
}

enum RegexContainerError get_group_value_nt(
    const RegexContainer *const container,
    const size_t group_index,
    char **const output,
    size_t *const output_length)
{
        if (container == NULL || output == NULL || output_length == NULL)
                return RC_ERR_NULL_ARGUMENT;
        if (group_index < 0 || group_index >= container->max_groups)
                return RC_ERR_INVALID_ARGUMENT_DIMENTIONS;
        if (container->is_match == 0)
                return RC_ERR_NO_MATCH;

        size_t length_no_nt = (container->groups)[group_index].rm_eo - (container->groups)[group_index].rm_so;
        char *value = malloc((length_no_nt + 1) * sizeof(char)); // +1 for the \0
        if (value == NULL)
                return RC_ERR_MEMORY_ALLOCATION;
        for (size_t i = 0; i < length_no_nt; i++)
                value[i] = (container->matched_input)[(container->groups)[group_index].rm_so + i];

        value[length_no_nt] = '\0'; // length_no_nt is now the last index

        *output = value;
        *output_length = length_no_nt;

        return RC_SUCCESS;
}

enum RegexContainerError get_group_value_str(
    const RegexContainer *const container,
    const size_t group_index,
    String **const output)
{
        if (container == NULL || output == NULL)
                return RC_ERR_NULL_ARGUMENT;
        if (group_index < 0 || group_index >= container->max_groups)
                return RC_ERR_INVALID_ARGUMENT_DIMENTIONS;
        if (container->is_match == 0)
                return RC_ERR_NO_MATCH;

        size_t length = (container->groups)[group_index].rm_eo - (container->groups)[group_index].rm_so;
        char *tmp = malloc(length * sizeof(char));
        if (tmp == NULL)
                return RC_ERR_MEMORY_ALLOCATION;
        for (size_t i = 0; i < length; i++)
                tmp[i] = (container->matched_input)[(container->groups)[group_index].rm_so + i];

        String *str = NULL;
        switch (new_string(tmp, length, &str))
        {
        case STR_ERR_INVALID_ARGUMENT_DIMENTIONS:
                return RC_ERR_INVALID_ARGUMENT_DIMENTIONS;
        case STR_ERR_MEMORY_ALLOCATION:
                return RC_ERR_MEMORY_ALLOCATION;
        case STR_ERR_NULL_ARGUMENT:
                return RC_ERR_NULL_ARGUMENT;
        }

        *output = str;
        free(tmp);

        return RC_SUCCESS;
}