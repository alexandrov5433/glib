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

	/* =========================
	   new_string / free_string
	   ========================= */
	String *s1 = new_string("abc", 3);
	EXPECT_NOT_NULL(s1);
	EXPECT_EQ(s1->length, 3);
	EXPECT_STR_EQ(s1->str, "abc", 3);

	String *empty = new_string(NULL, 0);
	EXPECT_NOT_NULL(empty);
	EXPECT_EQ(empty->length, 0);

	free_string(empty);
	free_string(s1);

	/* =========================
	   append_char
	   ========================= */
	s1 = new_string("ab", 2);
	EXPECT_EQ(append_char('c', s1), 0);
	EXPECT_EQ(s1->length, 3);
	EXPECT_STR_EQ(s1->str, "abc", 3);

	EXPECT_EQ(append_char('x', NULL), 1);

	/* =========================
	   append_char_array
	   ========================= */
	EXPECT_EQ(append_char_array("def", 3, s1), 0);
	EXPECT_EQ(s1->length, 6);
	EXPECT_STR_EQ(s1->str, "abcdef", 6);

	EXPECT_EQ(append_char_array(NULL, 3, s1), 1);

	/* =========================
	   append_nt
	   ========================= */
	EXPECT_EQ(append_nt("ghi", s1), 0);
	EXPECT_EQ(s1->length, 9);
	EXPECT_STR_EQ(s1->str, "abcdefghi", 9);

	EXPECT_EQ(append_nt(NULL, s1), 1);

	/* =========================
	   append_str
	   ========================= */
	String *s2 = new_string("XYZ", 3);
	EXPECT_EQ(append_str(s2, s1), 0);
	EXPECT_EQ(s1->length, 12);
	EXPECT_STR_EQ(s1->str, "abcdefghiXYZ", 12);

	EXPECT_EQ(append_str(NULL, s1), 1);

	/* =========================
	   prepend_char
	   ========================= */
	EXPECT_EQ(prepend_char('!', s1), 0);
	EXPECT_EQ(s1->str[0], '!');

	EXPECT_EQ(prepend_char('x', NULL), 1);

	/* =========================
	   prepend_char_array
	   ========================= */
	EXPECT_EQ(prepend_char_array(">>", 2, s1), 0);
	EXPECT_STR_EQ(s1->str, ">>!abcdefghiXYZ", s1->length);

	EXPECT_EQ(prepend_char_array(NULL, 2, s1), 1);

	/* =========================
	   prepend_nt
	   ========================= */
	EXPECT_EQ(prepend_nt("START:", s1), 0);
	EXPECT_STR_EQ(s1->str, "START:>>!abcdefghiXYZ", s1->length);

	EXPECT_EQ(prepend_nt(NULL, s1), 1);

	/* =========================
	   prepend_str
	   ========================= */
	EXPECT_EQ(prepend_str(s2, s1), 0);
	EXPECT_STR_EQ(s1->str, "XYZSTART:>>!abcdefghiXYZ", s1->length);

	EXPECT_EQ(prepend_str(NULL, s1), 1);

	/* =========================
	   duplicate_str
	   ========================= */
	String *dup = NULL;
	EXPECT_EQ(duplicate_str(s1, &dup), 0);
	EXPECT_NOT_NULL(dup);
	EXPECT_EQ(dup->length, s1->length);
	EXPECT_STR_EQ(dup->str, s1->str, s1->length);

	EXPECT_EQ(duplicate_str(NULL, &dup), 1);

	/* =========================
	   get_raw
	   ========================= */
	char *raw = NULL;
	EXPECT_EQ(get_raw(s1, &raw), 0);
	EXPECT_NOT_NULL(raw);
	EXPECT_STR_EQ(raw, s1->str, s1->length);
	free(raw);

	EXPECT_EQ(get_raw(NULL, &raw), 1);

	/* =========================
	   get_raw_nt
	   ========================= */
	char *raw_nt = NULL;
	EXPECT_EQ(get_raw_nt(s1, &raw_nt), 0);
	EXPECT_NOT_NULL(raw_nt);
	EXPECT_EQ(raw_nt[s1->length], '\0');
	EXPECT_STR_EQ(raw_nt, s1->str, s1->length);
	free(raw_nt);

	EXPECT_EQ(get_raw_nt(NULL, &raw_nt), 1);

	/* =========================
	   filter_str
	   ========================= */
	String *num = new_string("a1b2c3", 6);
	EXPECT_EQ(filter_str(num, keep_digits), 0);
	EXPECT_EQ(num->length, 3);
	EXPECT_STR_EQ(num->str, "123", 3);

	EXPECT_EQ(filter_str(NULL, keep_digits), 1);

	/* =========================
	   replace_char
	   ========================= */
	EXPECT_EQ(replace_char(num, '1', '9'), 0);
	EXPECT_STR_EQ(num->str, "923", 3);

	EXPECT_EQ(replace_char(NULL, 'a', 'b'), 1);

	/* =========================
	   remove_char
	   ========================= */
	EXPECT_EQ(remove_char(num, '2'), 0);
	EXPECT_EQ(num->length, 2);
	EXPECT_STR_EQ(num->str, "93", 2);

	EXPECT_EQ(remove_char(NULL, 'x'), 1);

	/* =========================
	   Cleanup
	   ========================= */
	free_string(num);
	free_string(dup);
	free_string(s2);
	free_string(s1);

	printf(ANSI_COLOR_GREEN "All tests passed!\n" ANSI_COLOR_RESET);
	puts("################## Test: String ##################");
}