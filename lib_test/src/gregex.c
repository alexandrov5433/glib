#include <stdlib.h>
#include <stdio.h>
#include "../include/gregex.h"
#include "../../include/gregex.h"
#include "../../include/gstring.h"
#include <string.h>
#include <assert.h>

void test_gregex()
{
	puts("################## Test: Gregex ##################");
	printf("Running tests...\n");

	// new_regex_container
	RegexContainer *rc = NULL;
	assert(new_regex_container(NULL, 1, REG_EXTENDED, &rc) == GLX_ERR_NULL_ARGUMENT);

	assert(new_regex_container("([a-z]+)", 2, REG_EXTENDED, NULL) == GLX_ERR_NULL_ARGUMENT);

	assert(new_regex_container("([a-z]+)", 2, REG_EXTENDED, &rc) == GLX_SUCCESS);
	assert(rc != NULL);

	// match
	assert(match(NULL, "test", 0) == GLX_ERR_NULL_ARGUMENT);
	assert(match(rc, NULL, 0) == GLX_ERR_NULL_ARGUMENT);

	assert(match(rc, "hello", 0) == GLX_SUCCESS);
	assert(rc->is_match == 1);

	// get_group_value
	char *group = NULL;
	size_t group_len = 0;

	assert(get_group_value(NULL, 0, &group, &group_len) == GLX_ERR_NULL_ARGUMENT);
	assert(get_group_value(rc, 0, NULL, &group_len) == GLX_ERR_NULL_ARGUMENT);
	assert(get_group_value(rc, 0, &group, NULL) == GLX_ERR_NULL_ARGUMENT);
	assert(get_group_value(rc, 999, &group, &group_len) == GLX_ERR_INVALID_ARGUMENT_DIMENTIONS);

	assert(get_group_value(rc, 0, &group, &group_len) == GLX_SUCCESS);
	assert(group != NULL);
	assert(group_len > 0);
	free(group);

	// get_group_value_nt
	char *group_nt = NULL;
	size_t group_nt_len = 0;

	assert(get_group_value_nt(NULL, 0, &group_nt, &group_nt_len) == GLX_ERR_NULL_ARGUMENT);
	assert(get_group_value_nt(rc, 0, NULL, &group_nt_len) == GLX_ERR_NULL_ARGUMENT);
	assert(get_group_value_nt(rc, 0, &group_nt, NULL) == GLX_ERR_NULL_ARGUMENT);
	assert(get_group_value_nt(rc, 0, &group_nt, &group_nt_len) == GLX_SUCCESS);
	assert(group_nt != NULL);
	assert(strlen(group_nt) == group_nt_len);
	free(group_nt);

	// get_group_value_str
	String *group_str = NULL;
	assert(get_group_value_str(NULL, 0, &group_str) == GLX_ERR_NULL_ARGUMENT);
	assert(get_group_value_str(rc, 0, NULL) == GLX_ERR_NULL_ARGUMENT);
	assert(get_group_value_str(rc, 0, &group_str) == GLX_SUCCESS);
	assert(group_str != NULL);
	assert(group_str->length > 0);

	free_string(&group_str);

	// match_str
	String *test_str = NULL;
	assert(new_string("world", 5, &test_str) == GLX_SUCCESS);

	assert(match_str(NULL, test_str, 0) == GLX_ERR_NULL_ARGUMENT);
	assert(match_str(rc, NULL, 0) == GLX_ERR_NULL_ARGUMENT);

	assert(match_str(rc, test_str, 0) == GLX_SUCCESS);

	free_string(&test_str);

	// free_regex_container
	assert(free_regex_container(NULL) == GLX_ERR_NULL_ARGUMENT);
	assert(free_regex_container(&rc) == GLX_SUCCESS);
	assert(rc == NULL);

	printf("All gregex tests passed!\n");
	puts(ANSI_COLOR_GREEN "Result: Success" ANSI_COLOR_RESET);
	puts("################## Test: Gregex ##################");
}