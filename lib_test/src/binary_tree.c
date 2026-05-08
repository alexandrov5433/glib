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

void processor_bt_does_nothing(void *const data)
{
        return;
}

int selector_bt_89(void *const data)
{
        return *(int *)data == 89 ? 1 : 0;
}

int selector_bt_5(void *const data)
{
        return *(int *)data == 5 ? 1 : 0;
}

int selector_bt_invalid(void *const data)
{
        return *(int *)data == -50000 ? 1 : 0;
}

void test_binary_tree(void)
{
        puts("################## Test: BinaryTree ##################");
        printf("Running tests...\n");
        int first = 10;
        int arr[10] = {1, 234, 2, 6, 88, 45, 0, -1, 22, 4};

        // new_binary_tree
        BinaryTree *bt = NULL;
        int err_bt = new_binary_tree(NULL, &bt);
        EXPECT_EQ(err_bt, BT_ERR_NULL_ARGUMENT);
        err_bt = new_binary_tree(comparator_bt, NULL);
        EXPECT_EQ(err_bt, BT_ERR_NULL_ARGUMENT);

        err_bt = new_binary_tree(comparator_bt, &bt);
        EXPECT_EQ(err_bt, BT_SUCCESS);
        EXPECT_NULL(bt->root);
        EXPECT_NOT_NULL(bt->comparator);

        // Setup: BinaryTree with NULL root and comparator members
        BinaryTree *bt_null_root = NULL;
        int err_bt_null_root_init = new_binary_tree(comparator_bt, &bt_null_root);
        EXPECT_EQ(err_bt_null_root_init, BT_SUCCESS);
        EXPECT_NULL(bt_null_root->root);

        BinaryTree *bt_null_cmp = NULL;
        int err_bt_null_cpm_init = new_binary_tree(comparator_bt, &bt_null_cmp);
        EXPECT_EQ(err_bt_null_cpm_init, BT_SUCCESS);
        bt_null_cmp->comparator = NULL;
        EXPECT_NULL(bt_null_cmp->comparator);

        // add_bt
        int err_add = BT_SUCCESS;
        err_add = add_bt(NULL, (void *)(arr));
        EXPECT_EQ(err_add, BT_ERR_NULL_ARGUMENT);
        err_add = add_bt(bt, NULL);
        EXPECT_EQ(err_add, BT_ERR_NULL_ARGUMENT);
        err_add = add_bt(bt_null_cmp, (void *)(arr));
        EXPECT_EQ(err_add, BT_ERR_NULL_COMPARATOR);

        // Setup: BinaryTree with NULL data member of root Node
        BinaryTree *bt_null_root_data = NULL;
        int err_bt_null_root_data_init = new_binary_tree(comparator_bt, &bt_null_root_data);
        EXPECT_EQ(err_bt_null_root_data_init, BT_SUCCESS);
        err_add = add_bt(bt_null_root_data, (void *)(arr));
        EXPECT_EQ(err_add, BT_SUCCESS);
        EXPECT_NOT_NULL(bt_null_root_data->root);
        bt_null_root_data->root->data = NULL;
        EXPECT_NULL(bt_null_root_data->root->data);
        err_add = add_bt(bt_null_root_data, (void *)(arr));
        EXPECT_EQ(err_add, BT_ERR_NULL_DATA);

        err_add = add_bt(bt, &first);
        EXPECT_EQ(err_add, BT_SUCCESS);
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

        // BINARY_TREE_MAX_DEPTH test
        BinaryTree *bt_max_depth = NULL;
        int err_bt_max_depth_init = new_binary_tree(comparator_bt, &bt_max_depth);
        EXPECT_EQ(err_bt_max_depth_init, BT_SUCCESS);
        int err_add_max = BT_SUCCESS;
        // printf("Testing BINARY_TREE_MAX_DEPTH = %d\n", BINARY_TREE_MAX_DEPTH);
        for (size_t i = 1; i <= BINARY_TREE_MAX_DEPTH; ++i)
        {
                // printf("Adding: %d\r", i);
                err_add_max = add_bt(bt_max_depth, &i);
                EXPECT_EQ(err_add_max, BT_SUCCESS);
        }
        // puts("");
        size_t max_value = 100000;
        err_add_max = add_bt(bt_max_depth, &max_value);
        EXPECT_EQ(err_add_max, BT_ERR_MAX_DEPTH);

        // process_bt
        int err_process = process_bt(NULL, processor_bt);
        EXPECT_EQ(err_process, BT_ERR_NULL_ARGUMENT);
        err_process = process_bt(bt, NULL);
        EXPECT_EQ(err_process, BT_ERR_NULL_ARGUMENT);
        err_process = process_bt(bt_null_root, processor_bt);
        EXPECT_EQ(err_process, BT_ERR_NULL_ROOT);
        err_process = process_bt(bt_null_root_data, processor_bt);
        EXPECT_EQ(err_process, BT_ERR_NULL_DATA);
        err_process = process_bt(bt_max_depth, processor_bt_does_nothing);
        EXPECT_EQ(err_process, BT_ERR_MAX_DEPTH);

        err_process = process_bt(bt, processor_bt);
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

        // dfs_bt
        int *output_dfs = NULL;
        int err_dfs = dfs_bt(NULL, selector_bt_5, (void **)&output_dfs);
        EXPECT_EQ(err_dfs, BT_ERR_NULL_ARGUMENT);
        err_dfs = dfs_bt(bt, NULL, (void **)&output_dfs);
        EXPECT_EQ(err_dfs, BT_ERR_NULL_ARGUMENT);
        err_dfs = dfs_bt(bt, selector_bt_5, NULL);
        EXPECT_EQ(err_dfs, BT_ERR_NULL_ARGUMENT);
        err_dfs = dfs_bt(bt_null_root, selector_bt_5, (void **)&output_dfs);
        EXPECT_EQ(err_dfs, BT_ERR_NULL_ROOT);
        err_dfs = dfs_bt(bt_null_root_data, selector_bt_5, (void **)&output_dfs);
        EXPECT_EQ(err_dfs, BT_ERR_NULL_DATA);
        err_dfs = dfs_bt(bt_max_depth, selector_bt_invalid, (void **)&output_dfs);
        EXPECT_EQ(err_dfs, BT_ERR_MAX_DEPTH);

        err_dfs = dfs_bt(bt, selector_bt_5, (void **)&output_dfs);
        EXPECT_EQ(err_dfs, BT_SUCCESS);
        EXPECT_EQ(*output_dfs, 5);
        err_dfs = dfs_bt(bt, selector_bt_89, (void **)&output_dfs);
        EXPECT_EQ(err_dfs, BT_SUCCESS);
        EXPECT_EQ(*output_dfs, 89);
        err_dfs = dfs_bt(bt, selector_bt_invalid, (void **)&output_dfs);
        EXPECT_EQ(err_dfs, BT_ITEM_NOT_FOUND);
        EXPECT_EQ(*output_dfs, 89);

        // clean up
        int err_free = free_bt(bt);
        EXPECT_EQ(err_free, BT_SUCCESS);
        err_free = free_bt(bt_null_root);
        EXPECT_EQ(err_free, BT_SUCCESS);
        err_free = free_bt(bt_null_cmp);
        EXPECT_EQ(err_free, BT_SUCCESS);
        err_free = free_bt(bt_null_root_data);
        EXPECT_EQ(err_free, BT_SUCCESS);

        printf("All tests passed!\n");
        puts(ANSI_COLOR_GREEN "Result: Success" ANSI_COLOR_RESET);
        puts("################## Test: BinaryTree ##################");
}