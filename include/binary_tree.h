#ifndef GALXLIB_BINARY_TREE_H
#define GALXLIB_BINARY_TREE_H

#include <stddef.h>

#ifdef _WIN32

#ifdef GALXLIB_SHARED

#ifdef GALXLIB_EXPORTS
#define GALXLIB_API __declspec(dllexport)
#else
#define GALXLIB_API __declspec(dllimport)
#endif

#else
#define GALXLIB_API
#endif

#endif

#ifndef BINARY_TREE_MAX_DEPTH
#define BINARY_TREE_MAX_DEPTH SIZE_MAX - 1
#endif

/**
 * @enum BinaryTreeError
 * @brief The error codes returned by the @ref BinaryTree functions.
 */
enum BinaryTreeError
{
	BT_EMPTY = -2,		      /**< (-2) The binary tree does not include any items. */
	BT_ITEM_NOT_FOUND = -1,	      /**< (-1) The searched item was not found. */
	BT_SUCCESS = 0,		      /**< (0) Successful execution of the called function. */
	BT_ERR_NULL_ARGUMENT = 1,     /**< (1) One or more arguments are NULL. */
	BT_ERR_MEMORY_ALLOCATION = 2, /**< (2) Failed to allocate or reallocate memory. */
	BT_ERR_MAX_DEPTH = 3,	      /**< (3) The maximum depth limit, defined as BINARY_TREE_MAX_DEPTH, was reached. */
	BT_ERR_NULL_DATA = 4,	      /**< (4) The data member in a @ref Node is NULL. */
	BT_ERR_NULL_ROOT = 5,	      /**< (5) The root member of the @ref BinaryTree is NULL. */
};

/**
 * @struct Node
 * @brief The structure describing a single node in the @ref BinaryTree.
 */
typedef struct Node
{
	void *data;	    /**< The data contained in the Node. */
	struct Node *left;  /**< The left neighbouring Node. */
	struct Node *right; /**< The right neighbouring Node. */
} Node;

/**
 * @struct BinaryTree
 * @brief The structure holding the relevant data of the BinaryTree.
 */
typedef struct BinaryTree
{
	struct Node *root;					     /**< The first @ref Node of the BinaryTree. */
	int (*comparator)(const void *const a, const void *const b); /**< A function, which compares the values of each @ref Node in the BinaryTree against each other. */
} BinaryTree;

/**
 * Creates a new @ref BinaryTree.
 * @param data The data of the root @ref Node.
 * @param comparator A function pointer to the comparator function, which will be used to order the Nodes.
 * (<0) new_value comes BEFORE existing_value. (>=0) new_value comes AFTER existing_value.
 * @param output The output address for the new @ref BinaryTree.
 * @return A value of the @ref BinaryTreeError:
 *
 * - BT_SUCCESS
 *
 * - BT_ERR_NULL_ARGUMENT
 *
 * - BT_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum BinaryTreeError new_binary_tree(
    void *const data,
    int (*comparator)(const void *const existing_value, const void *const new_value),
    BinaryTree **const output);

/**
 * Frees the memory of the @ref BinaryTree. The content of the data member of each @ref Node is NOT freed.
 * @param bt The @ref BinaryTree, which must be freed.
 * @return A value of the @ref BinaryTreeError:
 *
 * - BT_SUCCESS
 *
 * - BT_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum BinaryTreeError free_bt(BinaryTree *const bt);

/**
 * Frees the memory of the @ref BinaryTree.
 * The content of the data member of each @ref Node is fed to the destructor, before freeing the @ref Node.
 * @param bt The @ref BinaryTree, which must be freed.
 * @param destructor A function, which will be applied to every pointer in each data member.
 * @return A value of the @ref BinaryTreeError:
 *
 * - BT_SUCCESS
 *
 * - BT_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum BinaryTreeError free_bt_and_data(BinaryTree *const bt, void (*destructor)(void *data));

/**
 * Adds a @ref Node to the @ref BinaryTree.
 * @param bt The @ref BinaryTree, to which the new @ref Node will be added.
 * @param data The data of the new @ref Node.
 * @return A value of the @ref BinaryTreeError:
 *
 * - BT_SUCCESS
 *
 * - BT_ERR_NULL_ARGUMENT
 *
 * - BT_ERR_MEMORY_ALLOCATION
 *
 * - BT_ERR_NULL_ROOT
 *
 * - BT_ERR_MAX_DEPTH
 */
GALXLIB_API enum BinaryTreeError add_bt(BinaryTree *const bt, void *const data);

/**
 * Gets the data of a @ref Node from the @ref BinaryTree.
 * @param bt The @ref BinaryTree, in which to search.
 * @param selector A function, which will be used to recognize the @ref Node containing the wanted data. 
 * Must return 1 if the currently processed data is the wanted one.
 * @param output The output address for the wanted data.
 * @return A value of the @ref BinaryTreeError:
 *
 * - BT_SUCCESS
 * 
 * - BT_ITEM_NOT_FOUND
 *
 * - BT_ERR_NULL_ARGUMENT
 *
 * - BT_ERR_NULL_ROOT
 *
 * - BT_ERR_MAX_DEPTH
 * 
 * - BT_ERR_NULL_DATA
 */
GALXLIB_API enum BinaryTreeError dfs_bt(
    const BinaryTree *const bt,
    int (*selector)(void *const data),
    void **const output);

/**
 * Applies a processor function to the data fo each @ref Node in the @ref BinaryTree.
 * @param bt The @ref BinaryTree, which is to be processed.
 * @param processor A function, which receives the data from each @ref Node.
 */
GALXLIB_API enum BinaryTreeError process_bt(BinaryTree *const bt, void (*processor)(void *const data));

#endif