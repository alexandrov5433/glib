#include <stdlib.h>
#include <stdio.h>
#include "../include/binary_tree.h"

// ##################   static   ##################

static enum BinaryTreeError _new_node(void *const data, Node **const output)
{
	if (data == NULL || output == NULL)
		return BT_ERR_NULL_ARGUMENT;

	Node *node = (Node *)malloc(sizeof(Node));
	if (node == NULL)
		return BT_ERR_MEMORY_ALLOCATION;

	node->data = data;
	node->left = NULL;
	node->right = NULL;
	*output = node;

	return BT_SUCCESS;
}

static enum BinaryTreeError _free_node(Node *const node)
{
	if (node == NULL)
		return BT_ERR_NULL_ARGUMENT;
	if (node->left != NULL)
		_free_node(node->left);
	if (node->right != NULL)
		_free_node(node->right);

	free(node);
	return BT_SUCCESS;
}

static enum BinaryTreeError _free_node_and_data(Node *const node, void (*destructor)(void *ptr))
{
	if (node == NULL)
		return BT_ERR_NULL_ARGUMENT;
	if (node->left != NULL)
		_free_node_and_data(node->left, destructor);
	if (node->right != NULL)
		_free_node_and_data(node->right, destructor);

	destructor(node->data);
	free(node);
	return BT_SUCCESS;
}

static enum BinaryTreeError _dfs(
    const Node *const node,
    int (*selector)(void *const data),
    size_t *const depth,
    void **const output)
{
	// depth first search; pre-order
	if (node == NULL || selector == NULL || depth == NULL || output == NULL)
		return BT_ERR_NULL_ARGUMENT;

	if (*depth > BINARY_TREE_MAX_DEPTH)
		return BT_ERR_MAX_DEPTH;
	(*depth)++;

	if (node->data == NULL)
		return BT_ERR_NULL_DATA;

	if (selector(node->data) == 1)
	{
		*output = node->data;
		return BT_SUCCESS;
	}

	// End of branch reached. Item not found.
	if (node->left == NULL && node->right == NULL)
		return BT_ITEM_NOT_FOUND;

	int err_left, err_right = BT_ITEM_NOT_FOUND;

	if (node->left != NULL)
		err_left = _dfs(node->left, selector, depth, output);
	if (err_left == BT_SUCCESS)
		return err_left;
	else if (err_left == BT_ITEM_NOT_FOUND)
		goto _right;
	return err_left; // actual error
_right:
	if (node->right != NULL)
		err_right = _dfs(node->right, selector, depth, output);
	return err_right; // success, not_found or actual error
}

enum BinaryTreeError _process_bt(
    Node *const node,
    void (*processor)(void *const data),
    size_t *const depth)
{
	if (node == NULL || processor == NULL || depth == NULL)
		return BT_ERR_NULL_ARGUMENT;

	if (*depth > BINARY_TREE_MAX_DEPTH)
		return BT_ERR_MAX_DEPTH;
	(*depth)++;

	if (node->data == NULL)
		return BT_ERR_NULL_DATA;
	processor(node->data);

	int err_left, err_right = BT_SUCCESS;

	if (node->left != NULL)
		err_left = _process_bt(node->left, processor, depth);
	if (err_left)
		return err_left;

	if (node->right != NULL)
		err_right = _process_bt(node->right, processor, depth);
	if (err_right)
		return err_right;

	return BT_SUCCESS;
}

// ##################   public   ##################

enum BinaryTreeError new_binary_tree(
    void *const data,
    int (*comparator)(const void *const a, const void *const b),
    BinaryTree **const output)
{
	if (data == NULL || comparator == NULL)
		return BT_ERR_NULL_ARGUMENT;

	BinaryTree *bt = (BinaryTree *)malloc(sizeof(BinaryTree));
	if (bt == NULL)
		return BT_ERR_MEMORY_ALLOCATION;

	Node *root = NULL;
	int err_root_init = _new_node(data, &root);
	if (err_root_init)
		return err_root_init;

	bt->root = root;
	bt->comparator = comparator;
	*output = bt;

	return BT_SUCCESS;
}

enum BinaryTreeError free_bt(BinaryTree *const bt)
{
	if (bt == NULL)
		return BT_ERR_NULL_ARGUMENT;

	_free_node(bt->root);
	free(bt);
	return BT_SUCCESS;
}

enum BinaryTreeError free_bt_and_data(BinaryTree *const bt, void (*destructor)(void *data))
{
	if (bt == NULL || destructor == NULL)
		return BT_ERR_NULL_ARGUMENT;

	_free_node_and_data(bt->root, destructor);
	free(bt);
	return BT_SUCCESS;
}

enum BinaryTreeError add_bt(BinaryTree *const bt, void *const data)
{
	if (bt == NULL || data == NULL)
		return BT_ERR_NULL_ARGUMENT;

	if (bt->root == NULL)
		BT_ERR_NULL_ROOT;

	Node *new_node = NULL;
	int err_new_node = _new_node(data, &new_node);
	if (err_new_node)
		return err_new_node;

	Node *node = bt->root;
	size_t depth = 0;
	while (1)
	{
		if (depth++ >= BINARY_TREE_MAX_DEPTH)
			break;

		int result = (bt->comparator)(node->data, data);
		//(<0) a comes BEFORE b. (0) a and b are EQUAL. (>0) a comes AFTER b.
		if (result < 0)
		{
			// less -> left child
			if (node->left == NULL)
			{
				// create Node from data; add as left child
				node->left = new_node;
				return BT_SUCCESS;
			}
			// assign left child to node; continue cicle
			node = node->left;
		}
		else
		{
			// equal or bigger -> right child
			if (node->right == NULL)
			{
				// create Node from data; add as right child
				node->right = new_node;
				return BT_SUCCESS;
			}
			// assign right child to node; continue cicle
			node = node->right;
		}
	}
	_free_node(new_node);
	return BT_ERR_MAX_DEPTH;
}

enum BinaryTreeError dfs_bt(
    const BinaryTree *const bt,
    int (*selector)(void *const data),
    void **const output)
{
	if (bt == NULL || selector == NULL || output == NULL)
		return BT_ERR_NULL_ARGUMENT;

	if (bt->root == NULL)
		BT_ERR_NULL_ROOT;

	size_t depth = 0;
	return _dfs(bt->root, selector, &depth, output);
}

enum BinaryTreeError process_bt(BinaryTree *const bt, void (*processor)(void *const data))
{
	if (bt == NULL || processor == NULL)
		return BT_ERR_NULL_ARGUMENT;

	if (bt->root == NULL)
		return BT_ERR_NULL_ROOT;

	size_t depth = 0;
	return _process_bt(bt->root, processor, &depth);
}

// void processBT(BinaryTree *bt, void (*processor)(void *const ptr))
// {
//     if (bt == NULL || processor == NULL)
//     {
//         return;
//     }
//     _processBT(bt->root, processor);
// }
