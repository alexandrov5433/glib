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

void gstring_init_zero_length(String **const str)
{
	EXPECT_EQ(new_string("124", 3, str), STR_SUCCESS);
	EXPECT_NOT_NULL((*str)->str);
	EXPECT_EQ((*str)->length, 3);
	(*str)->length = 0;
	EXPECT_EQ((*str)->length, 0);
}

static void _test_split_str(
    String *const str,
    String *const pattern,
    char *const raw_str_result,
    size_t const result_length,
    size_t const split_count)
{
	DynamicArray *da_split_output = NULL;

	EXPECT_EQ(split_str(str, pattern, &da_split_output), STR_SUCCESS);
	EXPECT_NOT_NULL(da_split_output);
	EXPECT_EQ(da_split_output->count, split_count);

	String *str_concat = NULL;
	EXPECT_EQ(concat_str_da(&str_concat, da_split_output), DA_SUCCESS);
	EXPECT_NOT_NULL(str_concat);
	EXPECT_NOT_NULL(str_concat->str);
	EXPECT_EQ(str_concat->length, result_length);
	EXPECT_STR_EQ(str_concat->str, raw_str_result, result_length);

	free_string(str_concat);

	process_da(da_split_output, (void (*)(void *))free_string);
	free_dynamic_array(da_split_output);
}

static void test_split_str(String *str_null_str, String *str_zero_length, String *empty)
{
	String *pattern = NULL;
	String *str_split = NULL;
	String *str_split_pattern_end = NULL;
	String *str_split_pattern_at_start = NULL;
	String *str_split_pattern_at_start_and_end = NULL;
	String *str_split_pattern_at_end_partial = NULL;
	EXPECT_EQ(new_string("PATTERN", 7, &pattern), STR_SUCCESS);
	EXPECT_EQ(new_string("abcPATTERNdePATTERNfg", 21, &str_split), STR_SUCCESS);
	EXPECT_EQ(new_string("abcPATTERNdePATTERNfgPATTERN", 28, &str_split_pattern_end), STR_SUCCESS);
	EXPECT_EQ(new_string("PATTERNabcPATTERNdePATTERNfg", 28, &str_split_pattern_at_start), STR_SUCCESS);
	EXPECT_EQ(new_string("PATTERNabcPATTERNdePATTERNfgPATTERN", 35, &str_split_pattern_at_start_and_end), STR_SUCCESS);
	EXPECT_EQ(new_string("PATTERNabcPATTERNdePATTERNfgPAT", 31, &str_split_pattern_at_end_partial), STR_SUCCESS);

	DynamicArray *da_split_output = NULL;

	EXPECT_EQ(split_str(NULL, pattern, &da_split_output), STR_ERR_NULL_ARGUMENT);
	EXPECT_NULL(da_split_output);
	EXPECT_EQ(split_str(str_split, NULL, &da_split_output), STR_ERR_NULL_ARGUMENT);
	EXPECT_NULL(da_split_output);
	EXPECT_EQ(split_str(str_split, pattern, NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_NULL(da_split_output);

	EXPECT_EQ(split_str(str_null_str, pattern, &da_split_output), STR_ERR_NULL_STR);
	EXPECT_NULL(da_split_output);
	EXPECT_EQ(split_str(str_split, str_null_str, &da_split_output), STR_ERR_NULL_STR);
	EXPECT_NULL(da_split_output);

	EXPECT_EQ(split_str(str_zero_length, pattern, &da_split_output), STR_ERR_ZERO_LENGTH);
	EXPECT_NULL(da_split_output);
	EXPECT_EQ(split_str(str_split, str_zero_length, &da_split_output), STR_ERR_ZERO_LENGTH);
	EXPECT_NULL(da_split_output);

	EXPECT_EQ(split_str(empty, pattern, &da_split_output), STR_SUCCESS);
	EXPECT_NOT_NULL(da_split_output);
	EXPECT_EQ(da_split_output->count, 0);
	free_dynamic_array(da_split_output);

	EXPECT_EQ(split_str(str_split, empty, &da_split_output), STR_SUCCESS);
	EXPECT_NOT_NULL(da_split_output);
	EXPECT_EQ(da_split_output->count, 0);
	free_dynamic_array(da_split_output);

	_test_split_str(str_split, pattern, "abcdefg", 7, 3);
	_test_split_str(str_split_pattern_end, pattern, "abcdefg", 7, 3);
	_test_split_str(str_split_pattern_at_start, pattern, "abcdefg", 7, 3);
	_test_split_str(str_split_pattern_at_start_and_end, pattern, "abcdefg", 7, 3);
	_test_split_str(str_split_pattern_at_end_partial, pattern, "abcdefgPAT", 10, 3);

	free_string(pattern);
	free_string(str_split);
	free_string(str_split_pattern_end);
	free_string(str_split_pattern_at_start);
	free_string(str_split_pattern_at_start_and_end);
	free_string(str_split_pattern_at_end_partial);
}

static void test_concat_str_da(String *const empty)
{
	String *s1 = NULL;
	String *s2 = NULL;
	String *s3 = NULL;
	String *s4 = NULL;
	String *str_output = NULL;
	EXPECT_EQ(new_string("abc", 3, &s1), STR_SUCCESS);
	EXPECT_EQ(new_string("def", 3, &s2), STR_SUCCESS);
	EXPECT_EQ(new_string("ghijklmnopqrstuvgxyz", 20, &s3), STR_SUCCESS);
	EXPECT_EQ(new_string("1234", 4, &s4), STR_SUCCESS);

	DynamicArray *da_strings = NULL;
	EXPECT_EQ(new_dynamic_array(VOID_PTR, &da_strings), DA_SUCCESS);
	EXPECT_EQ(push_ptr_da(da_strings, (void *)s1), DA_SUCCESS);
	EXPECT_EQ(push_ptr_da(da_strings, (void *)s2), DA_SUCCESS);
	EXPECT_EQ(push_ptr_da(da_strings, (void *)s3), DA_SUCCESS);
	EXPECT_EQ(push_ptr_da(da_strings, (void *)s4), DA_SUCCESS);

	DynamicArray *da_strings_with_empty = NULL;
	EXPECT_EQ(new_dynamic_array(VOID_PTR, &da_strings_with_empty), DA_SUCCESS);
	EXPECT_EQ(push_ptr_da(da_strings_with_empty, (void *)s1), DA_SUCCESS);
	EXPECT_EQ(push_ptr_da(da_strings_with_empty, (void *)s2), DA_SUCCESS);
	EXPECT_EQ(push_ptr_da(da_strings_with_empty, empty), DA_SUCCESS);
	EXPECT_EQ(push_ptr_da(da_strings_with_empty, (void *)s3), DA_SUCCESS);
	EXPECT_EQ(push_ptr_da(da_strings_with_empty, (void *)s4), DA_SUCCESS);

	DynamicArray *da_empty = NULL;
	EXPECT_EQ(new_dynamic_array(VOID_PTR, &da_empty), DA_SUCCESS);

	DynamicArray *da_invalid_type = NULL;
	EXPECT_EQ(new_dynamic_array(DOUBLE, &da_invalid_type), DA_SUCCESS);
	EXPECT_EQ(push_double_da(da_invalid_type, 1234.123), DA_SUCCESS);

	EXPECT_EQ(concat_str_da(NULL, da_strings), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(concat_str_da(&str_output, NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(concat_str_da(&str_output, da_invalid_type), STR_ERR_INVALID_ARGUMENT_DIMENTIONS);

	EXPECT_EQ(concat_str_da(&str_output, da_empty), STR_SUCCESS);
	EXPECT_NOT_NULL(str_output);
	EXPECT_NULL(str_output->str);
	free_string(str_output);
	str_output = NULL;

	EXPECT_EQ(concat_str_da(&str_output, da_strings), STR_SUCCESS);
	EXPECT_NOT_NULL(str_output);
	EXPECT_NOT_NULL(str_output->str);
	EXPECT_EQ(str_output->length, 30);
	EXPECT_STR_EQ(str_output->str, "abcdefghijklmnopqrstuvgxyz1234", str_output->length);
	free_string(str_output);
	str_output = NULL;

	EXPECT_EQ(concat_str_da(&str_output, da_strings_with_empty), STR_SUCCESS);
	EXPECT_NOT_NULL(str_output);
	EXPECT_NOT_NULL(str_output->str);
	EXPECT_EQ(str_output->length, 30);
	EXPECT_STR_EQ(str_output->str, "abcdefghijklmnopqrstuvgxyz1234", str_output->length);
	free_string(str_output);
	str_output = NULL;

	free_string(s1);
	free_string(s2);
	free_string(s3);
	free_string(s4);
	free_string(str_output);

	free_dynamic_array(da_strings);
	free_dynamic_array(da_strings_with_empty);
	free_dynamic_array(da_empty);
	free_dynamic_array(da_invalid_type);
}

void test_gstring(void)
{
	puts("################## Test: String ##################");
	printf(ANSI_COLOR_CYAN "Running String tests...\n" ANSI_COLOR_RESET);

	// new_string
	String *s1 = NULL;
	EXPECT_EQ(new_string(NULL, 3, &s1), STR_ERR_INVALID_ARGUMENT_DIMENTIONS);
	EXPECT_EQ(new_string("abc", 0, &s1), STR_ERR_INVALID_ARGUMENT_DIMENTIONS);
	EXPECT_EQ(new_string("abc", 3, NULL), STR_ERR_NULL_ARGUMENT);

	EXPECT_EQ(new_string("abc", 3, &s1), STR_SUCCESS);
	EXPECT_NOT_NULL(s1);
	EXPECT_EQ(s1->length, 3);
	EXPECT_STR_EQ(s1->str, "abc", 3);

	String *empty = NULL;
	EXPECT_EQ(new_string(NULL, 0, &empty), STR_SUCCESS);
	EXPECT_NOT_NULL(empty);
	EXPECT_NULL(empty->str);
	EXPECT_EQ(empty->length, 0);

	String *str_null_str = NULL;
	EXPECT_EQ(new_string("124", 3, &str_null_str), STR_SUCCESS);
	EXPECT_NOT_NULL(str_null_str->str);
	EXPECT_EQ(str_null_str->length, 3);
	free(str_null_str->str);
	str_null_str->str = NULL;
	EXPECT_NULL(str_null_str->str);

	String *str_zero_length = NULL;
	gstring_init_zero_length(&str_zero_length);

	// append_char
	EXPECT_EQ(append_char(NULL, 'd'), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(append_char(str_null_str, 'd'), STR_ERR_NULL_STR);
	EXPECT_EQ(append_char(str_zero_length, 'd'), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(append_char(s1, 'd'), STR_SUCCESS);
	EXPECT_EQ(s1->length, 4);
	EXPECT_STR_EQ(s1->str, "abcd", 4);

	// append_char_array
	EXPECT_EQ(append_char_array(NULL, "nothing", 7), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(append_char_array(s1, NULL, 7), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(append_char_array(str_null_str, "nothing", 7), STR_ERR_NULL_STR);
	EXPECT_EQ(append_char_array(str_zero_length, "nothing", 7), STR_ERR_ZERO_LENGTH);
	EXPECT_EQ(append_char_array(s1, "nothing", 0), STR_ERR_INVALID_ARGUMENT_DIMENTIONS);

	EXPECT_EQ(append_char_array(s1, "efg", 3), STR_SUCCESS);
	EXPECT_EQ(s1->length, 7);
	EXPECT_STR_EQ(s1->str, "abcdefg", 7);

	// append_nt
	EXPECT_EQ(append_nt(NULL, "nothing"), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(append_nt(s1, NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(append_nt(str_null_str, "nothing"), STR_ERR_NULL_STR);
	EXPECT_EQ(append_nt(str_zero_length, "nothing"), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(append_nt(s1, "hi"), STR_SUCCESS);
	EXPECT_EQ(s1->length, 9);
	EXPECT_STR_EQ(s1->str, "abcdefghi", 9);

	// append_str
	String *s2 = NULL;
	EXPECT_EQ(new_string("XYZ", 3, &s2), STR_SUCCESS);

	EXPECT_EQ(append_str(s1, NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(append_str(NULL, s1), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(append_str(str_null_str, s1), STR_ERR_NULL_STR);
	EXPECT_EQ(append_str(str_zero_length, s1), STR_ERR_ZERO_LENGTH);
	EXPECT_EQ(append_str(s1, str_null_str), STR_ERR_NULL_STR);
	EXPECT_EQ(append_str(s1, str_zero_length), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(append_str(s2, s1), STR_SUCCESS);
	EXPECT_EQ(s1->length, 12);
	EXPECT_STR_EQ(s1->str, "abcdefghiXYZ", 12);

	// prepend_char
	EXPECT_EQ(prepend_char(NULL, 'x'), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(prepend_char(str_null_str, 'x'), STR_ERR_NULL_STR);
	EXPECT_EQ(prepend_char(str_zero_length, 'x'), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(prepend_char(s1, '!'), STR_SUCCESS);
	EXPECT_EQ(s1->str[0], '!');
	EXPECT_EQ(s1->length, 13);
	EXPECT_STR_EQ(s1->str, "!abcdefghiXYZ", s1->length);

	// prepend_char_array
	EXPECT_EQ(prepend_char_array(NULL, ">>", 2), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(prepend_char_array(s1, NULL, 2), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(prepend_char_array(str_null_str, ">>", 2), STR_ERR_NULL_STR);
	EXPECT_EQ(prepend_char_array(str_zero_length, ">>", 2), STR_ERR_ZERO_LENGTH);
	EXPECT_EQ(prepend_char_array(s1, ">>", 0), STR_ERR_INVALID_ARGUMENT_DIMENTIONS);

	EXPECT_EQ(prepend_char_array(s1, ">>", 2), STR_SUCCESS);
	EXPECT_EQ(s1->length, 15);
	EXPECT_STR_EQ(s1->str, ">>!abcdefghiXYZ", s1->length);

	// prepend_nt
	EXPECT_EQ(prepend_nt(NULL, "nothing"), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(prepend_nt(s1, NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(prepend_nt(str_null_str, "nothing"), STR_ERR_NULL_STR);
	EXPECT_EQ(prepend_nt(str_zero_length, "nothing"), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(prepend_nt(s1, "START:"), STR_SUCCESS);
	EXPECT_EQ(s1->length, 21);
	EXPECT_STR_EQ(s1->str, "START:>>!abcdefghiXYZ", s1->length);

	// prepend_str
	EXPECT_EQ(prepend_str(s1, NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(prepend_str(NULL, s1), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(prepend_str(str_null_str, s1), STR_ERR_NULL_STR);
	EXPECT_EQ(prepend_str(str_zero_length, s1), STR_ERR_ZERO_LENGTH);
	EXPECT_EQ(prepend_str(s1, str_null_str), STR_ERR_NULL_STR);
	EXPECT_EQ(prepend_str(s1, str_zero_length), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(prepend_str(s2, s1), STR_SUCCESS);
	EXPECT_EQ(s2->length, 3);
	EXPECT_EQ(s1->length, 24);
	EXPECT_STR_EQ(s1->str, "XYZSTART:>>!abcdefghiXYZ", s1->length);

	// duplicate_str
	String *dup = NULL;
	EXPECT_EQ(duplicate_str(NULL, &dup), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(duplicate_str(s1, NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(duplicate_str(str_null_str, &dup), STR_ERR_NULL_STR);
	EXPECT_EQ(duplicate_str(str_zero_length, &dup), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(duplicate_str(s1, &dup), STR_SUCCESS);
	EXPECT_NOT_NULL(dup);
	EXPECT_EQ(s1->length, 24);
	EXPECT_EQ(dup->length, s1->length);
	EXPECT_STR_EQ(dup->str, s1->str, s1->length);

	// get_raw
	char *raw = NULL;
	EXPECT_EQ(get_raw(NULL, &raw), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(get_raw(s1, NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(get_raw(str_null_str, &raw), STR_ERR_NULL_STR);
	EXPECT_EQ(get_raw(str_zero_length, &raw), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(get_raw(s1, &raw), STR_SUCCESS);
	EXPECT_NOT_NULL(raw);
	EXPECT_EQ(s1->length, 24);
	EXPECT_STR_EQ(raw, s1->str, s1->length);
	free(raw);

	// get_raw_nt
	char *raw_nt = NULL;
	EXPECT_EQ(get_raw_nt(NULL, &raw_nt), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(get_raw_nt(s1, NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(get_raw_nt(str_null_str, &raw_nt), STR_ERR_NULL_STR);
	EXPECT_EQ(get_raw_nt(str_zero_length, &raw_nt), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(get_raw_nt(s1, &raw_nt), STR_SUCCESS);
	EXPECT_NOT_NULL(raw_nt);
	EXPECT_EQ(s1->length, 24);
	EXPECT_EQ(raw_nt[s1->length], '\0');
	EXPECT_STR_EQ(raw_nt, s1->str, s1->length);
	free(raw_nt);

	// filter_str
	String *num = NULL;
	EXPECT_EQ(new_string("a1b2c3", 6, &num), STR_SUCCESS);
	EXPECT_EQ(filter_str(NULL, keep_digits), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(filter_str(num, NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(filter_str(str_null_str, keep_digits), STR_ERR_NULL_STR);
	EXPECT_EQ(filter_str(str_zero_length, keep_digits), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(filter_str(empty, keep_digits), STR_SUCCESS);
	EXPECT_NULL(empty->str);
	EXPECT_EQ(empty->length, 0);

	EXPECT_EQ(filter_str(num, keep_digits), STR_SUCCESS);
	EXPECT_EQ(num->length, 3);
	EXPECT_STR_EQ(num->str, "123", 3);

	// replace_char
	EXPECT_EQ(replace_char(NULL, 'a', 'b'), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(replace_char(str_null_str, 'a', 'b'), STR_ERR_NULL_STR);
	EXPECT_EQ(replace_char(str_zero_length, 'a', 'b'), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(replace_char(empty, '1', '9'), STR_SUCCESS);
	EXPECT_NULL(empty->str);
	EXPECT_EQ(empty->length, 0);

	EXPECT_EQ(replace_char(num, '1', '9'), STR_SUCCESS);
	EXPECT_STR_EQ(num->str, "923", 3);

	// remove_char
	EXPECT_EQ(remove_char(NULL, 'x'), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(remove_char(str_null_str, 'x'), STR_ERR_NULL_STR);
	EXPECT_EQ(remove_char(str_zero_length, 'x'), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(remove_char(empty, '2'), STR_SUCCESS);
	EXPECT_NULL(empty->str);
	EXPECT_EQ(empty->length, 0);

	EXPECT_EQ(remove_char(num, '2'), STR_SUCCESS);
	EXPECT_EQ(num->length, 2);
	EXPECT_STR_EQ(num->str, "93", 2);

	// concat
	String *con_1 = NULL;
	String *con_2 = NULL;
	String *con_result = NULL;
	String *con_empty = NULL;
	EXPECT_EQ(new_string("abc", 3, &con_1), STR_SUCCESS);
	EXPECT_EQ(new_string("def", 3, &con_2), STR_SUCCESS);

	EXPECT_EQ(concat(NULL, 2, con_1, con_2), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(concat(&con_result, 2, NULL, con_2), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(concat(&con_result, 2, con_1, NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(concat(&con_result, 2, str_null_str, con_2), STR_ERR_NULL_STR);
	EXPECT_EQ(concat(&con_result, 2, str_zero_length, con_2), STR_ERR_ZERO_LENGTH);
	EXPECT_EQ(concat(&con_result, 2, con_1, str_null_str), STR_ERR_NULL_STR);
	EXPECT_EQ(concat(&con_result, 2, con_1, str_zero_length), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(concat(&con_result, 2, con_1, con_2), STR_SUCCESS);
	EXPECT_NOT_NULL(con_result);
	EXPECT_NOT_NULL(con_result->str);
	EXPECT_EQ(con_result->length, 6);
	EXPECT_STR_EQ(con_result->str, "abcdef", 6);
	EXPECT_EQ(concat(&con_empty, 2, con_result, empty), STR_SUCCESS);
	EXPECT_NOT_NULL(con_empty->str);
	EXPECT_EQ(con_empty->length, 6);
	EXPECT_STR_EQ(con_empty->str, "abcdef", 6);

	// concat_str_da
	test_concat_str_da(empty);

	// trim
	String *str_trim = NULL;
	String *str_trim_left = NULL;
	String *str_trim_right = NULL;
	String *str_trim_all = NULL;
	String *str_trim_not_middle = NULL;
	String *str_trim_no_trim = NULL;
	String *str_trim_not_middle_no_trim = NULL;
	EXPECT_EQ(new_string("   asdf   ", 10, &str_trim), STR_SUCCESS);
	EXPECT_EQ(new_string("   asdf", 7, &str_trim_left), STR_SUCCESS);
	EXPECT_EQ(new_string("asdf   ", 7, &str_trim_right), STR_SUCCESS);
	EXPECT_EQ(new_string("      ", 6, &str_trim_all), STR_SUCCESS);
	EXPECT_EQ(new_string("   as df   ", 11, &str_trim_not_middle), STR_SUCCESS);
	EXPECT_EQ(new_string("asdf", 4, &str_trim_no_trim), STR_SUCCESS);
	EXPECT_EQ(new_string("as df", 5, &str_trim_not_middle_no_trim), STR_SUCCESS);

	EXPECT_EQ(trim(NULL), STR_ERR_NULL_ARGUMENT);
	EXPECT_EQ(trim(str_null_str), STR_ERR_NULL_STR);
	EXPECT_EQ(trim(str_zero_length), STR_ERR_ZERO_LENGTH);

	EXPECT_EQ(trim(empty), STR_SUCCESS);

	EXPECT_EQ(trim(str_trim), STR_SUCCESS);
	EXPECT_NOT_NULL(str_trim->str);
	EXPECT_STR_EQ(str_trim->str, "asdf", 4);

	EXPECT_EQ(trim(str_trim_left), STR_SUCCESS);
	EXPECT_NOT_NULL(str_trim_left->str);
	EXPECT_STR_EQ(str_trim_left->str, "asdf", 4);

	EXPECT_EQ(trim(str_trim_right), STR_SUCCESS);
	EXPECT_NOT_NULL(str_trim_right->str);
	EXPECT_STR_EQ(str_trim_right->str, "asdf", 4);

	EXPECT_EQ(trim(str_trim_all), STR_SUCCESS);
	EXPECT_NULL(str_trim_all->str);
	EXPECT_EQ(str_trim_all->length, 0);

	EXPECT_EQ(trim(str_trim_not_middle), STR_SUCCESS);
	EXPECT_NOT_NULL(str_trim_not_middle->str);
	EXPECT_STR_EQ(str_trim_not_middle->str, "as df", 5);

	EXPECT_EQ(trim(str_trim_no_trim), STR_SUCCESS);
	EXPECT_NOT_NULL(str_trim_no_trim->str);
	EXPECT_STR_EQ(str_trim_no_trim->str, "asdf", 4);

	EXPECT_EQ(trim(str_trim_not_middle_no_trim), STR_SUCCESS);
	EXPECT_NOT_NULL(str_trim_not_middle_no_trim->str);
	EXPECT_STR_EQ(str_trim_not_middle_no_trim->str, "as df", 5);

	// split_str
	test_split_str(str_null_str, str_zero_length, empty);

	// free_string
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
	free_string(str_zero_length);

	printf(ANSI_COLOR_GREEN "All tests passed!\n" ANSI_COLOR_RESET);
	puts("################## Test: String ##################");
}