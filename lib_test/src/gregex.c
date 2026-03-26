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
        printf("Running gregex tests...\n");

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

        printf("All gregex tests passed!\n");
        puts(ANSI_COLOR_GREEN "Result: Success" ANSI_COLOR_RESET);
        puts("################## Test: Gregex ##################");
}