#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../include/gstring.h"
#include "../include/gstring.h"

/* Simple test macros */
#define EXPECT_EQ(actual, expected) assert((actual) == (expected))
#define EXPECT_STR_EQ(a, b, len) assert(memcmp((a), (b), (len)) == 0)
#define EXPECT_NOT_NULL(ptr) assert((ptr) != NULL)
#define EXPECT_NULL(ptr) assert((ptr) == NULL)

/* Sample filter: keep only digits */
int keep_digits(char c)
{
	return (c >= '0' && c <= '9');
}

void test_gstring(void)
{
	puts("################## Test: String ##################");
	printf(ANSI_COLOR_CYAN "Running String tests...\n" ANSI_COLOR_RESET);

	// new_string
	String *s1 = NULL;
	int err_s1_init = new_string(NULL, 3, &s1);
	EXPECT_EQ(err_s1_init, STR_ERR_INVALID_ARGUMENT_DIMENTIONS);
	err_s1_init = new_string("abc", 0, &s1);
	EXPECT_EQ(err_s1_init, STR_ERR_INVALID_ARGUMENT_DIMENTIONS);
	err_s1_init = new_string("abc", 3, NULL);
	EXPECT_EQ(err_s1_init, STR_ERR_NULL_ARGUMENT);

	err_s1_init = new_string("abc", 3, &s1);
	EXPECT_EQ(err_s1_init, STR_SUCCESS);
	EXPECT_NOT_NULL(s1);
	EXPECT_EQ(s1->length, 3);
	EXPECT_STR_EQ(s1->str, "abc", 3);

	String *empty = NULL;
	int err_empty_init = new_string(NULL, 0, &empty);
	EXPECT_EQ(err_empty_init, STR_SUCCESS);
	EXPECT_NOT_NULL(empty);
	EXPECT_NULL(empty->str);
	EXPECT_EQ(empty->length, 0);

	String *str_null_str = NULL;
	int err_null_str = new_string("124", 3, &str_null_str);
	EXPECT_EQ(err_null_str, STR_SUCCESS);
	EXPECT_NOT_NULL(str_null_str->str);
	EXPECT_EQ(str_null_str->length, 3);
	free(str_null_str->str);
	str_null_str->str = NULL;
	EXPECT_NULL(str_null_str->str);

	/* =========================
	   append_char
	   ========================= */
	int err_ac_init = new_string("ab", 2, &s1);
	EXPECT_EQ(append_char(NULL, 'x'), 1);
	EXPECT_EQ(err_ac_init, 0);
	EXPECT_EQ(append_char(s1, 'c'), 0);
	EXPECT_EQ(s1->length, 3);
	EXPECT_STR_EQ(s1->str, "abc", 3);

	/* =========================
	   append_char_array
	   ========================= */
	EXPECT_EQ(append_char_array(NULL, "nothing", 3), 1);
	EXPECT_EQ(append_char_array(s1, NULL, 3), 1);
	EXPECT_EQ(append_char_array(s1, "def", 3), 0);
	EXPECT_EQ(s1->length, 6);
	EXPECT_STR_EQ(s1->str, "abcdef", 6);

	/* =========================
	   append_nt
	   ========================= */
	EXPECT_EQ(append_nt(NULL, "nothing"), 1);
	EXPECT_EQ(append_nt(s1, NULL), 1);
	EXPECT_EQ(append_nt(s1, "ghi"), 0);
	EXPECT_EQ(s1->length, 9);
	EXPECT_STR_EQ(s1->str, "abcdefghi", 9);

	/* =========================
	   append_str
	   ========================= */
	String *s2 = NULL;
	int err_s2_init = new_string("XYZ", 3, &s2);

	EXPECT_EQ(append_str(s1, NULL), 1);
	EXPECT_EQ(append_str(NULL, s1), 1);
	EXPECT_EQ(append_str(s2, s1), 0);
	EXPECT_EQ(s1->length, 12);
	EXPECT_STR_EQ(s1->str, "abcdefghiXYZ", 12);

	/* =========================
	   prepend_char
	   ========================= */
	EXPECT_EQ(prepend_char(NULL, 'x'), 1);
	EXPECT_EQ(prepend_char(s1, '!'), 0);
	EXPECT_EQ(s1->str[0], '!');
	EXPECT_EQ(s1->length, 13);
	EXPECT_STR_EQ(s1->str, "!abcdefghiXYZ", s1->length);

	/* =========================
	   prepend_char_array
	   ========================= */
	EXPECT_EQ(prepend_char_array(NULL, "nothing", 2), 1);
	EXPECT_EQ(prepend_char_array(s1, NULL, 2), 1);
	EXPECT_EQ(prepend_char_array(s1, ">>", 2), 0);
	EXPECT_EQ(s1->length, 15);
	EXPECT_STR_EQ(s1->str, ">>!abcdefghiXYZ", s1->length);

	/* =========================
	   prepend_nt
	   ========================= */
	EXPECT_EQ(prepend_nt(NULL, "nothing"), 1);
	EXPECT_EQ(prepend_nt(s1, NULL), 1);
	EXPECT_EQ(prepend_nt(s1, "START:"), 0);
	EXPECT_EQ(s1->length, 21);
	EXPECT_STR_EQ(s1->str, "START:>>!abcdefghiXYZ", s1->length);

	/* =========================
	   prepend_str
	   ========================= */
	EXPECT_EQ(prepend_str(s1, NULL), 1);
	EXPECT_EQ(prepend_str(NULL, s1), 1);
	EXPECT_EQ(prepend_str(s2, s1), 0);
	EXPECT_STR_EQ(s1->str, "XYZSTART:>>!abcdefghiXYZ", s1->length);

	/* =========================
	   duplicate_str
	   ========================= */
	String *dup = NULL;
	EXPECT_EQ(duplicate_str(NULL, &dup), 1);
	EXPECT_EQ(duplicate_str(s1, NULL), 1);
	EXPECT_EQ(duplicate_str(s1, &dup), 0);
	EXPECT_NOT_NULL(dup);
	EXPECT_EQ(dup->length, s1->length);
	EXPECT_STR_EQ(dup->str, s1->str, s1->length);

	/* =========================
	   get_raw
	   ========================= */
	char *raw = NULL;
	EXPECT_EQ(get_raw(NULL, &raw), 1);
	EXPECT_EQ(get_raw(s1, NULL), 1);
	EXPECT_EQ(get_raw(s1, &raw), 0);
	EXPECT_NOT_NULL(raw);
	EXPECT_STR_EQ(raw, s1->str, s1->length);
	free(raw);

	/* =========================
	   get_raw_nt
	   ========================= */
	char *raw_nt = NULL;
	EXPECT_EQ(get_raw_nt(NULL, &raw_nt), 1);
	EXPECT_EQ(get_raw_nt(s1, NULL), 1);
	EXPECT_EQ(get_raw_nt(s1, &raw_nt), 0);
	EXPECT_NOT_NULL(raw_nt);
	EXPECT_EQ(raw_nt[s1->length], '\0');
	EXPECT_STR_EQ(raw_nt, s1->str, s1->length);
	free(raw_nt);

	/* =========================
	   filter_str
	   ========================= */
	String *num = NULL;
	int err_num_init = new_string("a1b2c3", 6, &num);
	EXPECT_EQ(err_num_init, 0);
	EXPECT_EQ(filter_str(NULL, keep_digits), 1);
	EXPECT_EQ(filter_str(num, NULL), 1);
	EXPECT_EQ(filter_str(num, keep_digits), 0);
	EXPECT_EQ(num->length, 3);
	EXPECT_STR_EQ(num->str, "123", 3);

	/* =========================
	   replace_char
	   ========================= */
	EXPECT_EQ(replace_char(NULL, 'a', 'b'), 1);
	EXPECT_EQ(replace_char(num, '1', '9'), 0);
	EXPECT_STR_EQ(num->str, "923", 3);

	/* =========================
	   remove_char
	   ========================= */
	EXPECT_EQ(remove_char(NULL, 'x'), 1);
	EXPECT_EQ(remove_char(num, '2'), 0);
	EXPECT_EQ(num->length, 2);
	EXPECT_STR_EQ(num->str, "93", 2);

	/* =========================
	   concat
	   ========================= */
	String *con_1 = NULL;
	String *con_2 = NULL;
	String *con_result = NULL;
	int err_con_1_init = new_string("abc", 3, &con_1);
	EXPECT_EQ(err_con_1_init, 0);
	int err_con_2_init = new_string("def", 3, &con_2);
	EXPECT_EQ(err_con_2_init, 0);
	EXPECT_EQ(concat(NULL, 2, con_1, con_2), 1);
	EXPECT_EQ(concat(&con_result, 2, NULL, con_2), 1);
	EXPECT_EQ(concat(&con_result, 2, con_1, NULL), 1);
	EXPECT_EQ(concat(&con_result, 2, con_1, con_2), 0);
	EXPECT_STR_EQ(con_result->str, "abcdef", 6);

	// trim
	String *str_trim = NULL;
	String *str_trim_left = NULL;
	String *str_trim_right = NULL;
	String *str_trim_all = NULL;
	String *str_trim_not_middle = NULL;
	String *str_trim_no_trim = NULL;
	String *str_trim_not_middle_no_trim = NULL;
	int err_trim_init = new_string("   asdf   ", 10, &str_trim);
	EXPECT_EQ(err_trim_init, STR_SUCCESS);
	err_trim_init = new_string("   asdf", 7, &str_trim_left);
	EXPECT_EQ(err_trim_init, STR_SUCCESS);
	err_trim_init = new_string("asdf   ", 7, &str_trim_right);
	EXPECT_EQ(err_trim_init, STR_SUCCESS);
	err_trim_init = new_string("      ", 6, &str_trim_all);
	EXPECT_EQ(err_trim_init, STR_SUCCESS);
	err_trim_init = new_string("   as df   ", 11, &str_trim_not_middle);
	EXPECT_EQ(err_trim_init, STR_SUCCESS);
	err_trim_init = new_string("asdf", 4, &str_trim_no_trim);
	EXPECT_EQ(err_trim_init, STR_SUCCESS);
	err_trim_init = new_string("as df", 5, &str_trim_not_middle_no_trim);
	EXPECT_EQ(err_trim_init, STR_SUCCESS);

	int err_trim = trim(NULL);
	EXPECT_EQ(err_trim, STR_ERR_NULL_ARGUMENT);
	err_trim = trim(empty);
	EXPECT_EQ(err_trim, STR_SUCCESS);
	err_trim = trim(str_null_str);
	EXPECT_EQ(err_trim, STR_ERR_NULL_STR);

	err_trim = trim(str_trim);
	EXPECT_EQ(err_trim, STR_SUCCESS);
	EXPECT_STR_EQ(str_trim->str, "asdf", 4);

	err_trim = trim(str_trim_left);
	EXPECT_EQ(err_trim, STR_SUCCESS);
	EXPECT_STR_EQ(str_trim_left->str, "asdf", 4);

	err_trim = trim(str_trim_right);
	EXPECT_EQ(err_trim, STR_SUCCESS);
	EXPECT_STR_EQ(str_trim_right->str, "asdf", 4);

	err_trim = trim(str_trim_all);
	EXPECT_EQ(err_trim, STR_SUCCESS);
	EXPECT_NULL(str_trim_all->str);
	EXPECT_EQ(str_trim_all->length, 0);

	err_trim = trim(str_trim_not_middle);
	EXPECT_EQ(err_trim, STR_SUCCESS);
	EXPECT_STR_EQ(str_trim_not_middle->str, "as df", 5);

	err_trim = trim(str_trim_no_trim);
	EXPECT_EQ(err_trim, STR_SUCCESS);
	EXPECT_STR_EQ(str_trim_no_trim->str, "asdf", 4);

	err_trim = trim(str_trim_not_middle_no_trim);
	EXPECT_EQ(err_trim, STR_SUCCESS);
	EXPECT_STR_EQ(str_trim_not_middle_no_trim->str, "as df", 5);

	/* =========================
	   Cleanup
	   ========================= */
	free_string(empty);
	free_string(s1);
	free_string(num);
	free_string(dup);
	free_string(s2);
	free_string(con_1);
	free_string(con_2);
	free_string(con_result);
	free_string(str_null_str);
	free_string(str_trim);
	free_string(str_trim_left);
	free_string(str_trim_right);
	free_string(str_trim_all);
	free_string(str_trim_not_middle);
	free_string(str_trim_no_trim);
	free_string(str_trim_not_middle_no_trim);

	printf(ANSI_COLOR_GREEN "All tests passed!\n" ANSI_COLOR_RESET);
	puts("################## Test: String ##################");
}