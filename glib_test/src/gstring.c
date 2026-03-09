#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/gstring.h"
#include "../include/gstring.h"

int filter_string(char c)
{
    if (c == '!')
        return 1;
    return 0;
}
int is_same_string(char *a, char *b, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}
void stringTest()
{
    puts("################## Test: String ##################");

    char original[4] = {'a', 'b', 'c', 'd'};
    char *originalNt = "abcd";
    String *testStr = new_string(original, 4);
    if (testStr == NULL)
    {
        puts(ANSI_COLOR_RED "String testStr was not created." ANSI_COLOR_RESET);
        goto _test_failed;
    }

    int err1 = append_char('!', testStr);
    if (err1)
    {
        printf(ANSI_COLOR_RED "Function append_char returned with error code: %d\n" ANSI_COLOR_RESET, err1);
        goto _test_failed;
    }
    if (is_same_string(testStr->str, "abcd!", 5) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after append_char. Expected: abcd!, received: %s\n" ANSI_COLOR_RESET, testStr->str);
        goto _test_failed;
    }
    puts("append_char: tested.");

    int err2 = append_char_array("|a_c_a|", 7, testStr);
    if (err2)
    {
        printf(ANSI_COLOR_RED "Function append_char_array returned with error code: %d\n" ANSI_COLOR_RESET, err2);
        goto _test_failed;
    }
    if (is_same_string(testStr->str, "abcd!|a_c_a|", 12) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after append_char_array. Expected: abcd!|a_c_a|, received: %s\n" ANSI_COLOR_RESET, testStr->str);
        goto _test_failed;
    }
    puts("append_char_array: tested.");

    int err3 = append_nt("|a_nt|", testStr);
    if (err3)
    {
        printf(ANSI_COLOR_RED "Function append_nt returned with error code: %d\n" ANSI_COLOR_RESET, err3);
        goto _test_failed;
    }
    if (is_same_string(testStr->str, "abcd!|a_c_a||a_nt|", 18) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after append_nt. Expected: abcd!|a_c_a||a_nt|, received: %s\n" ANSI_COLOR_RESET, testStr->str);
        goto _test_failed;
    }
    puts("append_nt: tested.");

    String *toCopyFrom = new_string("|a_str|", 7);
    if (toCopyFrom == NULL)
    {
        puts(ANSI_COLOR_RED "String toCopyFrom was not created." ANSI_COLOR_RESET);
        goto _test_failed;
    }

    int err4 = append_str(toCopyFrom, testStr);
    if (err4)
    {
        printf(ANSI_COLOR_RED "Function append_str returned with error code: %d\n" ANSI_COLOR_RESET, err4);
        goto _test_failed;
    }
    if (is_same_string(testStr->str, "abcd!|a_c_a||a_nt||a_str|", 25) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after append_str. Expected: abcd!|a_c_a||a_nt||a_str|, received: %s\n" ANSI_COLOR_RESET, testStr->str);
        goto _test_failed;
    }
    free_string(toCopyFrom);
    puts("append_str: tested.");

    int err5 = prepend_char('P', testStr);
    if (err5)
    {
        printf(ANSI_COLOR_RED "Function prepend_char returned with error code: %d\n" ANSI_COLOR_RESET, err5);
        goto _test_failed;
    }
    if (is_same_string(testStr->str, "Pabcd!|a_c_a||a_nt||a_str|", 26) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after prepend_char. Expected: Pabcd!|a_c_a||a_nt||a_str|, received: %s\n" ANSI_COLOR_RESET, testStr->str);
        goto _test_failed;
    }
    puts("prepend_char: tested.");

    int err6 = prepend_char_array("|p_c_a|", 7, testStr);
    if (err6)
    {
        printf(ANSI_COLOR_RED "Function prepend_char_array returned with error code: %d\n" ANSI_COLOR_RESET, err6);
        goto _test_failed;
    }
    if (is_same_string(testStr->str, "|p_c_a|Pabcd!|a_c_a||a_nt||a_str|", 26) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after prepend_char_array. Expected: |p_c_a|Pabcd!|a_c_a||a_nt||a_str|, received: %s\n" ANSI_COLOR_RESET, testStr->str);
        goto _test_failed;
    }
    puts("prepend_char_array: tested.");

    int err7 = prepend_nt("|p_nt|", testStr);
    if (err7)
    {
        printf(ANSI_COLOR_RED "Function prepend_nt returned with error code: %d\n" ANSI_COLOR_RESET, err7);
        goto _test_failed;
    }
    if (is_same_string(testStr->str, "|p_nt||p_c_a|Pabcd!|a_c_a||a_nt||a_str|", 32) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after prepend_nt. Expected: |p_nt||p_c_a|Pabcd!|a_c_a||a_nt||a_str|, received: %s\n" ANSI_COLOR_RESET, testStr->str);
        goto _test_failed;
    }
    puts("prepend_nt: tested.");

    String *strToCopyFrom = new_string("|p_str|", 7);
    if (strToCopyFrom == NULL)
    {
        puts(ANSI_COLOR_RED "String strToCopyFrom was not created." ANSI_COLOR_RESET);
        goto _test_failed;
    }
    int err8 = prepend_str(strToCopyFrom, testStr);
    if (err8)
    {
        printf(ANSI_COLOR_RED "Function prepend_str returned with error code: %d\n" ANSI_COLOR_RESET, err8);
        goto _test_failed;
    }
    if (is_same_string(testStr->str, "|p_str||p_nt||p_c_a|Pabcd!|a_c_a||a_nt||a_str|", 39) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after prepend_str. Expected: |p_str||p_nt||p_c_a|Pabcd!|a_c_a||a_nt||a_str|, received: %s\n" ANSI_COLOR_RESET, testStr->str);
        goto _test_failed;
    }
    free_string(strToCopyFrom);
    puts("prepend_str: tested.");

    String *duplicateStr = NULL;
    int err9 = duplicate_str(testStr, &duplicateStr);
    if (err9)
    {
        printf(ANSI_COLOR_RED "Function duplicate_str returned with error code: %d\n" ANSI_COLOR_RESET, err9);
        goto _test_failed;
    }
    if (is_same_string(testStr->str, duplicateStr->str, 39) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after duplicate_str. Expected: %s,\n received: %s\n" ANSI_COLOR_RESET, testStr->str, duplicateStr->str);
        goto _test_failed;
    }
    free_string(duplicateStr);
    puts("duplicate_str: tested.");

    char *getStrTest = NULL;
    int err10 = get_raw(testStr, &getStrTest);
    if (err10)
    {
        printf(ANSI_COLOR_RED "Function get_raw returned with error code: %d\n" ANSI_COLOR_RESET, err10);
        goto _test_failed;
    }
    if (is_same_string(testStr->str, getStrTest, 39) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after get_raw. Expected: %s,\n received: %s\n" ANSI_COLOR_RESET, testStr->str, getStrTest);
        goto _test_failed;
    }
    puts("get_raw: tested.");

    char *getStrNtTest = NULL;
    int err11 = get_raw_nt(testStr, &getStrNtTest);
    if (err11)
    {
        printf(ANSI_COLOR_RED "Function get_raw_nt returned with error code: %d\n" ANSI_COLOR_RESET, err11);
        goto _test_failed;
    }
    if (is_same_string("|p_str||p_nt||p_c_a|Pabcd!|a_c_a||a_nt||a_str|", getStrNtTest, 40) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after get_raw_nt. Expected: %s,\n received: %s\n" ANSI_COLOR_RESET, testStr->str, getStrNtTest);
        goto _test_failed;
    }
    puts("get_raw_nt: tested.");

    int err12 = filter_str(testStr, filter_string);
    if (err12)
    {
        printf(ANSI_COLOR_RED "Function filter_str returned with error code: %d\n" ANSI_COLOR_RESET, err12);
        goto _test_failed;
    }
    if (is_same_string("!", testStr->str, 1) == 0)
    {
        printf(ANSI_COLOR_RED "Incorrect String after filter_str. Expected: !,\n received: %s\n" ANSI_COLOR_RESET, testStr->str);
        goto _test_failed;
    }
    if (testStr->length != 1)
    {
        printf(ANSI_COLOR_RED "Incorrect String length after filter_str. Expected: 1,\n received: %d\n" ANSI_COLOR_RESET, testStr->length);
        goto _test_failed;
    }
    puts("filter_str: tested.");

    free_string(testStr);
    puts(ANSI_COLOR_GREEN "Result: Success" ANSI_COLOR_RESET);
    puts("################## Test: String ##################");
    return;

_test_failed:
    free_string(testStr);
    puts(ANSI_COLOR_RED "Result: Failed" ANSI_COLOR_RESET);
    puts("################## Test: String ##################");
}
