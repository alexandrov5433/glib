#include "../include/binary_tree.h"
#include "../../include/binary_tree.h"
#include "../../include/gstring.h"
#include <assert.h>
#include <stdio.h>

#define EXPECT_EQ(actual, expected) assert((actual) == (expected))
#define EXPECT_NOT_NULL(ptr) assert((ptr) != NULL)
#define EXPECT_NULL(ptr) assert((ptr) == NULL)

int comparator_bt(const void *const a, const void *const b)
{
        return *(int *)(a) < *(int *)(b) ? -1 : 0;
}

void test_binary_tree(void)
{
        puts("################## Test: BinaryTree ##################");
        printf("Running tests...\n");
        int first = 10;
        int arr[10] = {1, 234, 2, 6, 88, 45, 0, -1, 22, 4};

        // Creation
        // invalid
        BinaryTree *bt = NULL;
        int err_bt = new_binary_tree(NULL, comparator_bt, &bt);
        EXPECT_EQ(err_bt, BT_ERR_NULL_ARGUMENT);
        err_bt = new_binary_tree(&first, NULL, &bt);
        EXPECT_EQ(err_bt, BT_ERR_NULL_ARGUMENT);
        err_bt = new_binary_tree(&first, comparator_bt, NULL);
        EXPECT_EQ(err_bt, BT_ERR_NULL_ARGUMENT);
        // valid
        err_bt = new_binary_tree(&first, comparator_bt, &bt);
        EXPECT_EQ(err_bt, BT_SUCCESS);
        EXPECT_NOT_NULL(bt->root);
        EXPECT_NOT_NULL(bt->comparator);
        EXPECT_NOT_NULL(bt->root->data);
        EXPECT_NULL(bt->root->left);
        EXPECT_NULL(bt->root->right);

        // add_bt
        int err_add = BT_SUCCESS;
        for (size_t i = 0; i < 10; ++i)
        {
                err_add = add_bt(bt, (void *)(arr + i));
                EXPECT_EQ(err_add, BT_SUCCESS);
        }

        printf("All gregex tests passed!\n");
        puts(ANSI_COLOR_GREEN "Result: Success" ANSI_COLOR_RESET);
        puts("################## Test: BinaryTree ##################");
}