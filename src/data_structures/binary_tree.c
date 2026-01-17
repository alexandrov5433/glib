#include <stdlib.h>
#include <stdio.h>
#include "../include/data_structures/binary_tree.h"

static Node *newNode(void *const data)
{
    if (data == NULL)
    {
        return NULL;
    }

    Node *node = calloc(1, sizeof(Node));
    if (node == NULL)
    {
        return NULL;
    }

    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

BinaryTree *newBinaryTree(void *const ptr, int (*comparator)(const void *a, const void *b))
{
    if (ptr == NULL || comparator == NULL)
    {
        return NULL;
    }

    BinaryTree *bt = (BinaryTree *)malloc(sizeof(BinaryTree));
    if (bt == NULL)
    {
        return NULL;
    }

    Node *root = newNode(ptr);
    if (root == NULL)
    {
        return NULL;
    }

    bt->root = root;
    bt->comparator = comparator;

    return bt;
}

static void freeNode(Node *node)
{
    if (node == NULL)
    {
        return;
    }
    freeNode(node->left);
    freeNode(node->right);
    free(node);
}

int freeBT(BinaryTree *bt)
{
    if (bt == NULL)
    {
        return 1;
    }
    freeNode(bt->root);
    free(bt);
    return 0;
}

static void freeNodeAndData(Node *node, void (*releaser)(void *ptr))
{
    if (node == NULL)
    {
        return;
    }

    freeNodeAndData(node->left, releaser);
    freeNodeAndData(node->right, releaser);

    releaser(node->data);
    free(node);
}

int freeBTAndData(BinaryTree *bt, void (*releaser)(void *ptr))
{
    if (bt == NULL || releaser == NULL)
    {
        return 1;
    }
    freeNodeAndData(bt->root, releaser);
    free(bt);
    return 0;
}

int addToBT(BinaryTree *bt, void *const ptr)
{
    if (bt == NULL || ptr == NULL)
    {
        return 1;
    }

    Node *newChildNode = newNode(ptr);
    if (newChildNode == NULL)
    {
        return 2;
    }

    Node *node = bt->root;

    while (1)
    {
        int result = (bt->comparator)(node->data, ptr);
        //(<0) a comes BEFORE b. (0) a and b are EQUAL. (>0) a comes AFTER b.
        if (result < 0)
        {
            // less -> left child
            if (node->left == NULL)
            {
                // create Node from ptr; add as left child
                node->left = newChildNode;
                return 0;
            }
            // assign left child to node; continue cicle
            node = node->left;
        }
        else
        {
            // equal or bigger -> right child
            if (node->right == NULL)
            {
                // create Node from ptr; add as right child
                node->right = newChildNode;
                return 0;
            }
            // assign right child to node; continue cicle
            node = node->right;
        }
    }

    return 3;
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

static void *_BFS(Node *node, int (*selector)(void *const ptr))
{
    // TODO
}

void *findInBT(BinaryTree *bt, int searchAlg, int (*selector)(void *const ptr))
{
    if (bt == NULL || selector == NULL)
    {
        return NULL;
    }
    if (searchAlg <= 0)
    {
        return _DFS(bt->root, selector);
    }
    return _BFS(bt->root, selector);
}

static void _processBT(Node *node, void (*processor)(void *const ptr))
{
    if (node == NULL)
    {
        return;
    }
    _processBT(node->left, processor);
    _processBT(node->right, processor);
}

void processBT(BinaryTree *bt, void (*processor)(void *const ptr))
{
    if (bt == NULL || processor == NULL)
    {
        return;
    }
    _processBT(bt->root, processor);
}
