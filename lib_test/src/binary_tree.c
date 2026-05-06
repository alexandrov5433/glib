#include "../include/binary_tree.h"
#include "../../include/binary_tree.h"
#include "../../include/gstring.h"
#include <assert.h>
#include <stdio.h>

#define EXPECT_EQ(actual, expected) assert((actual) == (expected))
#define EXPECT_NOT_NULL(ptr) assert((ptr) != NULL)
#define EXPECT_NULL(ptr) assert((ptr) == NULL)

int comparator_bt(const void *const existing_value, const void *const new_value)
{
        return *(int *)(existing_value) > *(int *)(new_value) ? -1 : 0;
}

void processor_bt(void *const data)
{
        (*(int *)data)++;
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
        EXPECT_NOT_NULL(bt->root->left);
        EXPECT_EQ(*(int *)(bt->root->left->data), 1);

        EXPECT_NOT_NULL(bt->root->right);
        EXPECT_EQ(*(int *)(bt->root->right->data), 234);

        EXPECT_NOT_NULL(bt->root->left->right->data);
        EXPECT_EQ(*(int *)(bt->root->left->right->data), 2);

        EXPECT_NOT_NULL(bt->root->left->right->right);
        EXPECT_EQ(*(int *)(bt->root->left->right->right->data), 6);

        EXPECT_NOT_NULL(bt->root->right->left);
        EXPECT_EQ(*(int *)(bt->root->right->left->data), 88);

        EXPECT_NOT_NULL(bt->root->right->left->left);
        EXPECT_EQ(*(int *)(bt->root->right->left->left->data), 45);

        EXPECT_NOT_NULL(bt->root->left->left);
        EXPECT_EQ(*(int *)(bt->root->left->left->data), 0);

        EXPECT_NOT_NULL(bt->root->left->left->left);
        EXPECT_EQ(*(int *)(bt->root->left->left->left->data), -1);

        EXPECT_NOT_NULL(bt->root->right->left->left->left);
        EXPECT_EQ(*(int *)(bt->root->right->left->left->left->data), 22);

        EXPECT_NOT_NULL(bt->root->left->right->right->left);
        EXPECT_EQ(*(int *)(bt->root->left->right->right->left->data), 4);

        // process_bt
        int err_process = process_bt(bt, processor_bt);
        EXPECT_EQ(err_process, BT_SUCCESS);

        EXPECT_NOT_NULL(bt->root->left);
        EXPECT_EQ(*(int *)(bt->root->left->data), 2);

        EXPECT_NOT_NULL(bt->root->right);
        EXPECT_EQ(*(int *)(bt->root->right->data), 235);

        EXPECT_NOT_NULL(bt->root->left->right->data);
        EXPECT_EQ(*(int *)(bt->root->left->right->data), 3);

        EXPECT_NOT_NULL(bt->root->left->right->right);
        EXPECT_EQ(*(int *)(bt->root->left->right->right->data), 7);

        EXPECT_NOT_NULL(bt->root->right->left);
        EXPECT_EQ(*(int *)(bt->root->right->left->data), 89);

        EXPECT_NOT_NULL(bt->root->right->left->left);
        EXPECT_EQ(*(int *)(bt->root->right->left->left->data), 46);

        EXPECT_NOT_NULL(bt->root->left->left);
        EXPECT_EQ(*(int *)(bt->root->left->left->data), 1);

        EXPECT_NOT_NULL(bt->root->left->left->left);
        EXPECT_EQ(*(int *)(bt->root->left->left->left->data), 0);

        EXPECT_NOT_NULL(bt->root->right->left->left->left);
        EXPECT_EQ(*(int *)(bt->root->right->left->left->left->data), 23);

        EXPECT_NOT_NULL(bt->root->left->right->right->left);
        EXPECT_EQ(*(int *)(bt->root->left->right->right->left->data), 5);

        printf("All gregex tests passed!\n");
        puts(ANSI_COLOR_GREEN "Result: Success" ANSI_COLOR_RESET);
        puts("################## Test: BinaryTree ##################");
}