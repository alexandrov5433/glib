#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#ifdef _WIN32
#ifdef GLIB_EXPORTS
#define GLIB_API __declspec(dllexport)
#else
#define GLIB_API __declspec(dllimport)
#endif
#else
#define GLIB_API
#endif

typedef struct Node
{
    void *data;
    struct Node *left, *right;
} Node;

typedef struct BinaryTree
{
    struct Node *root;
    int (*comparator)(const void *a, const void *b);
} BinaryTree;

/**
 * @param ptr The pointer, which will be the data of the root Node. Can not be a NULL pointer.
 * @param comparator A function pointer to the comparator function, which will be used to order the Nodes. (<0) a comes BEFORE b. (0) a and b are EQUAL. (>0) a comes AFTER b. Can not be a NULL pointer.
 * @returns A pointer to the new BinaryTree.
 */
GLIB_API BinaryTree *newBinaryTree(void *const ptr, int (*comparator)(const void *a, const void *b));

/**
 * Frees the memory for the BinaryTree. The data at the adsresses of the data pointers in the Nodes is not manipulated.
 * @param bt A pointer to the BinaryTree, which must be freed. Can not be a NULL pointer.
 * @returns 0 on succes. 1 the argument bt is NULL.
 */
GLIB_API int freeBT(BinaryTree *bt);

/**
 * Frees the memory for the BinaryTree and appies a releaser function on every data pointer in every Node.
 * @param bt A pointer to the BinaryTree, which must be freed. Can not be a NULL pointer.
 * @param releaser A function pointer to the releaser function, which will be applied to every data pointer. Can not be a NULL pointer, otherwise returns NULL.
 * @returns 0 on succes. 1 if one of the arguments is NULL.
 */
GLIB_API int freeBTAndData(BinaryTree *bt, void (*releaser)(void *ptr));

/**
 * @param bt A pointer to the BinaryTree, in which the new pointer must be added. Can not be a NULL pointer.
 * @param ptr A pointer, which will be the the data of the newly added Node. Can not be a NULL pointer.
 * @returns 0 on success. 1 when a NULL pointer was received as an argument. 2 when a new Node could not be created from the ptr argument. 3 on unknown error; the function should not be reaching this return statement.
 */
GLIB_API int addToBT(BinaryTree *bt, void *const ptr);

/**
 * @param bt A pointer to the BinaryTree, in which the new pointer must be added. Can not be a NULL pointer.
 * @param searchAlg <=0 for Depth First Search (pre-order) and >0 for Breadth First Search.
 * @param selector A function pointer to the comparator function, which will be used to find the Node containing the wanted data. Must return 1 if the currently processed data is the wanted one. Can not be a NULL pointer, otherwise returns NULL.
 * @returns A pointer to the wanted data or NULL if nothing was found.
 */
GLIB_API void *findInBT(BinaryTree *bt, int searchAlg, int (*selector)(void *const ptr));

/**
 * Applies a processor function to every data pointer of every Node in the BinaryTree.
 * @param bt A pointer to the BinaryTree, in which the new pointer must be added. Can not be a NULL pointer.
 * @param selector A function pointer to the processor function. Can not be a NULL pointer, otherwise returns.
 */
GLIB_API void processBT(BinaryTree *bt, void (*processor)(void *const ptr));

#endif