#include <stdlib.h>
#include <stdio.h>
#include "../include/binary_tree.h"

// ##################   static   ##################

static enum BinaryTreeError _new_node(void *const data, Node **const output)
{
	if (data == NULL)
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

enum BinaryTreeError free_bt_and_data(BinaryTree *const bt, void (*destructor)(void *ptr))
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

	return BT_ERR_MAX_DEPTH;
}

static void *_DFS(Node *node, int (*selector)(void *const ptr))
{
	// depth first search; pre-order
	if (node == NULL)
	{
		return NULL;
	}
	if ((selector)(node->data) == 1)
	{
		return node->data;
	}
	// TODO: void* cast
	/*
	$ "D:/Program Files/CMake/bin/cmake.exe" --build build
    [1/3] Building C object CMakeFiles/glib.dir/src/data_structures/binary_tree.c.obj
    D:/Code Files/C/glib/src/data_structures/binary_tree.c: In function '_DFS':
    D:/Code Files/C/glib/src/data_structures/binary_tree.c:156:12: warning: cast to pointer from integer of differen
    t size [-Wint-to-pointer-cast]
      156 |     return (void*)(_DFS(node->left, selector) || _DFS(node->right, selector));
	  |            ^
    [3/3] Linking C executable glib_test.exe

	*/
	return (void *)(_DFS(node->left, selector) || _DFS(node->right, selector));
}

// static void *_BFS(Node *node, int (*selector)(void *const ptr))
// {
//     // TODO
// }

// void *findInBT(BinaryTree *bt, int searchAlg, int (*selector)(void *const ptr))
// {
//     if (bt == NULL || selector == NULL)
//     {
//         return NULL;
//     }
//     if (searchAlg <= 0)
//     {
//         return _DFS(bt->root, selector);
//     }
//     return _BFS(bt->root, selector);
// }

// static void _processBT(Node *node, void (*processor)(void *const ptr))
// {
//     if (node == NULL)
//     {
//         return;
//     }
//     _processBT(node->left, processor);
//     _processBT(node->right, processor);
// }

// void processBT(BinaryTree *bt, void (*processor)(void *const ptr))
// {
//     if (bt == NULL || processor == NULL)
//     {
//         return;
//     }
//     _processBT(bt->root, processor);
// }
