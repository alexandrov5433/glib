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
	/* printf("Running gregex tests...\n");

	int status = 0;

	// =========================
	// 1. Creation tests
	// =========================
	RegexContainer *rc = new_regex_container(
	    "([a-zA-Z]+)-([0-9]+)",
	    3,
	    REG_EXTENDED,
	    &status);

	assert(rc != NULL);
	assert(status == 0);
	assert(rc->isMatch == 0);
	assert(rc->matched_input == NULL);

	// NULL pattern
	RegexContainer *rc_null = new_regex_container(NULL, 2, REG_EXTENDED, &status);
	assert(rc_null == NULL);

	// =========================
	// 2. Match success
	// =========================
	int ret = match("test-1234", rc);
	assert(ret == 0);
	assert(rc->isMatch == 1);
	assert(rc->matched_input != NULL);
	assert(strcmp(rc->matched_input, "test-1234") == 0);

	// =========================
	// 3. Group extraction (raw)
	// =========================
	int len = 0;

	char *g0 = get_group_value(0, rc, &len);
	assert(g0 != NULL);
	assert(len == (int)strlen("test-1234"));
	free(g0);

	char *g1 = get_group_value(1, rc, &len);
	assert(g1 != NULL);
	assert(len == 4);
	assert(strncmp(g1, "test", len) == 0);
	free(g1);

	char *g2 = get_group_value(2, rc, &len);
	assert(g2 != NULL);
	assert(len == 4);
	assert(strncmp(g2, "1234", len) == 0);
	free(g2);

	// =========================
	// 4. Group extraction (null-terminated)
	// =========================
	char *g1_nt = get_group_value_nt(1, rc, &len);
	assert(g1_nt != NULL);
	assert(strcmp(g1_nt, "test") == 0);
	free(g1_nt);

	// =========================
	// 5. Group extraction (String)
	// =========================
	String *g2_str = get_group_value_str(2, rc);
	assert(g2_str != NULL);
	assert(g2_str->length == 4);
	assert(strncmp(g2_str->str, "1234", 4) == 0);
	free_string(g2_str);

	// =========================
	// 6. Invalid group index
	// =========================
	assert(get_group_value(5, rc, NULL) == NULL);
	assert(get_group_value_nt(5, rc, NULL) == NULL);
	assert(get_group_value_str(5, rc) == NULL);

	// =========================
	// 7. Match failure
	// =========================
	ret = match("no-match-here", rc);
	assert(ret == 0);
	assert(rc->isMatch == 0);
	assert(rc->matched_input == NULL);

	// After failed match, group access should fail
	assert(get_group_value(0, rc, NULL) == NULL);

	// =========================
	// 8. NULL argument handling
	// =========================
	assert(match(NULL, rc) == 1);
	assert(match("abc-123", NULL) == 1);

	assert(get_group_value(0, NULL, NULL) == NULL);
	assert(get_group_value_nt(0, NULL, NULL) == NULL);
	assert(get_group_value_str(0, NULL) == NULL);

	// =========================
	// 9. Cleanup
	// =========================
	free_regex_container(rc);
	free_regex_container(NULL); // should be safe
    */

	printf("Running tests...\n");

	/* =========================
	   new_regex_container
	========================= */
	RegexContainer *rc = NULL;
	assert(new_regex_container(NULL, 1, REG_EXTENDED, &rc) == RC_ERR_NULL_ARGUMENT);

	assert(new_regex_container("([a-z]+)", 2, REG_EXTENDED, NULL) == RC_ERR_NULL_ARGUMENT);

	assert(new_regex_container("([a-z]+)", 2, REG_EXTENDED, &rc) == RC_SUCCESS);
	assert(rc != NULL);

	/* =========================
	   match
	========================= */
	assert(match(NULL, "test") == RC_ERR_NULL_ARGUMENT);
	assert(match(rc, NULL) == RC_ERR_NULL_ARGUMENT);

	assert(match(rc, "hello") == RC_SUCCESS);
	assert(rc->is_match == 1);

	/* =========================
	   get_group_value
	========================= */
	char *group = NULL;
	size_t group_len = 0;

	assert(get_group_value(NULL, 0, &group, &group_len) == RC_ERR_NULL_ARGUMENT);
	assert(get_group_value(rc, 999, &group, &group_len) == RC_ERR_INVALID_ARGUMENT_DIMENTIONS);

	assert(get_group_value(rc, 0, &group, &group_len) == RC_SUCCESS);
	assert(group != NULL);
	assert(group_len > 0);
	free(group);

	/* =========================
	   get_group_value_nt
	========================= */
	char *group_nt = NULL;
	size_t group_nt_len = 0;

	assert(get_group_value_nt(rc, 0, &group_nt, &group_nt_len) == RC_SUCCESS);
	assert(group_nt != NULL);
	assert(strlen(group_nt) == group_nt_len);
	free(group_nt);

	/* =========================
	   get_group_value_str
	========================= */
	String *group_str = NULL;
	assert(get_group_value_str(rc, 0, &group_str) == RC_SUCCESS);
	assert(group_str != NULL);
	assert(group_str->length > 0);

	free_string(group_str);

	/* =========================
	   match_str
	========================= */
	String *test_str = NULL;
	assert(new_string("world", 5, &test_str) == STR_SUCCESS);

	assert(match_str(NULL, test_str) == RC_ERR_NULL_ARGUMENT);
	assert(match_str(rc, NULL) == RC_ERR_NULL_ARGUMENT);

	assert(match_str(rc, test_str) == RC_SUCCESS);

	free_string(test_str);

	/* =========================
	   free_regex_container
	========================= */
	assert(free_regex_container(NULL) == RC_ERR_NULL_ARGUMENT);
	assert(free_regex_container(rc) == RC_SUCCESS);

	printf("All gregex tests passed!\n");
	puts(ANSI_COLOR_GREEN "Result: Success" ANSI_COLOR_RESET);
	puts("################## Test: Gregex ##################");
}