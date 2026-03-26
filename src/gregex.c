#include "../include/gregex.h"
#include "../include/gstring.h"
#include <regex.h>
#include <stdlib.h>

void free_regex_container(RegexContainer *container)
{
        if (container == NULL)
                return;

        if (container->regex != NULL)
                regfree(container->regex);

        if (container->groups != NULL)
                free(container->groups);

        free(container);
}

RegexContainer *new_regex_container(
    const char *const pattern,
    const size_t max_groups,
    const int flag,
    int *const output_status)
{
        int error_code = 0;
        if (pattern == NULL)
        {
                error_code = 1;
                goto _err;
        }

        RegexContainer *container = calloc(1, sizeof(RegexContainer));
        if (container == NULL)
        {
                error_code = 2;
                goto _err;
        }

        regex_t *regex = calloc(1, sizeof(regex_t));
        if (regex == NULL)
        {
                error_code = 3;
                goto _err;
        }

        regmatch_t *groups = calloc(max_groups, sizeof(regmatch_t));
        if (groups == NULL)
        {
                error_code = 4;
                goto _err;
        }

        if (regcomp(regex, pattern, flag))
        {
                error_code = 5;
                goto _err;
        }

        container->regex = regex;
        container->max_groups = max_groups;
        container->groups = groups;
        container->matched_input = NULL;
        container->isMatch = 0;

        if (output_status)
                *output_status = error_code;

        return container;

_err:
        if (error_code >= 3)
                free_regex_container(container);

        if (output_status)
                *output_status = error_code;

        return NULL;
}

int match(char *const input, RegexContainer *const container)
{
        if (input == NULL || container == NULL)
                return 1;

        /*
        If it finds a match, regexec() shall return 0; otherwise, it shall return non-zero indicating either
        no match or an error.
        */
        int status = regexec(container->regex, input, container->max_groups, container->groups, 0);
        container->isMatch = status == 0 ? 1 : 0;
        container->matched_input = status == 0 ? input : NULL;

        return 0;
}

char *get_group_value(const int n, const RegexContainer *const container, int *const output_match_length)
{
        if (container == NULL)
                return NULL;
        if (n < 0 || n >= container->max_groups)
                return NULL;
        if (container->isMatch == 0)
                return NULL;

        int length_no_nt = (container->groups)[n].rm_eo - (container->groups)[n].rm_so;
        char *value = calloc(length_no_nt, sizeof(char));
        for (int i = 0; i < length_no_nt; i++)
                value[i] = (container->matched_input)[(container->groups)[n].rm_so + i];

        if (output_match_length)
                *output_match_length = length_no_nt;

        return value;
}

char *get_group_value_nt(const int n, const RegexContainer *const container, int *const output_match_length)
{
        if (container == NULL)
                return NULL;
        if (n < 0 || n >= container->max_groups)
                return NULL;
        if (container->isMatch == 0)
                return NULL;

        int length_no_nt = (container->groups)[n].rm_eo - (container->groups)[n].rm_so;
        char *value = calloc(length_no_nt + 1, sizeof(char)); // +1 for the \0
        for (int i = 0; i < length_no_nt; i++)
                value[i] = (container->matched_input)[(container->groups)[n].rm_so + i];

        value[length_no_nt] = '\0'; // length_no_nt is now the last index

        if (output_match_length)
                *output_match_length = length_no_nt;

        return value;
}

String *get_group_value_str(const int n, const RegexContainer *const container)
{
        if (container == NULL)
                return NULL;
        if (n < 0 || n >= container->max_groups)
                return NULL;
        if (container->isMatch == 0)
                return NULL;

        int length = (container->groups)[n].rm_eo - (container->groups)[n].rm_so;
        String *str = new_string(NULL, 0);
        if (str == NULL)
                return NULL;

        for (int i = 0; i < length; i++)
                append_char((container->matched_input)[(container->groups)[n].rm_so + i], str);

        return str;
}