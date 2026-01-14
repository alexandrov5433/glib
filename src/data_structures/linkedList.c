#include <stdlib.h>

#include "../include/data_structures/linkedList.h"

/**
 * Creates a new LinkedList.
 * @return The pointer to the new LinkedList. If the creation failed because of unsuccessful memory allocation NULL is returned.
 */
LinkedList *newLinkedList()
{
    LinkedList *ll = (LinkedList *)malloc(sizeof(LinkedList));
    if (ll == NULL)
        return NULL;
    ll->start = NULL;
    ll->end = NULL;
    ll->nodesCount = 0;
    return ll;
}

/**
 * Creates a new Node.
 * @param value A pointer, which the new Node must contain, as a value.
 * @return The pointer to the new Node on success. NULL is returned, if memory allocation has failed.
 */
static Node *newNodeLL(void *value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return NULL;
    node->value = value;
    node->next = NULL;
    return node;
}

/**
 * Frees the memory used from a Node.
 * @param node A pointer to the Node, which must be freed. If NULL, nothing is done.
 */
void freeNodeLL(Node *node)
{
    if (node == NULL)
    {
        return;
    }
    free(node);
}

/**
 * Frees the memory used from a LinkedList and all of the Nodes contained in it. This function utilizes the freeNodeLL function.
 * @param ll A pointer to the LinkedList, which must be freed. If NULL, nothing is done.
 */
void freeLinkedList(LinkedList *ll)
{
    if (ll == NULL)
    {
        return;
    }

    Node *n = ll->start;
    while (1)
    {
        if (n == NULL)
        {
            break;
        }
        Node *temp = n->next;
        freeNodeLL(n);
        n = temp;
    }

    free(ll);
}

/**
 * Appends a new Node to the LinkedList.
 * @param value A pointer, which the new Node must contain, as a value.
 * @param ll A pointer to the LinkedList, to which the new Node must be appended.
 * @return 0 on success, else the error code (>= 1) as an integer.
 */
int appendToLL(void *value, LinkedList *ll)
{
    if (value == NULL || ll == NULL)
        return 1;
    Node *node = newNodeLL(value);
    if (node == NULL)
        return 2;

    if (ll->start == NULL)
    {
        // case: empty ll
        ll->start = node;
    }
    else if (ll->start != NULL && ll->end == NULL)
    {
        // case: one node
        ll->start->next = node;
        ll->end = node;
    }
    else
    {
        // case: multiple nodes
        ll->end->next = node;
        ll->end = node;
    }

    ll->nodesCount++;
    return 0;
}

/**
 * Prepends a new Node to the LinkedList.
 * @param value A pointer, which the new Node must contain, as a value.
 * @param ll A pointer to the LinkedList, to which the new Node must be prepended.
 * @return 0 on success, else the error code (>= 1) as an integer.
 */
int prependToLL(void *value, LinkedList *ll)
{
    if (value == NULL || ll == NULL)
        return 1;
    Node *node = newNodeLL(value);
    if (node == NULL)
        return 2;

    if (ll->start == NULL)
    {
        // case: empty ll
        ll->start = node;
    }
    else if (ll->start != NULL && ll->end == NULL)
    {
        // case: one node at start
        node->next = ll->start;
        ll->end = ll->start;
        ll->start = node;
    }
    else
    {
        // case: multiple nodes
        node->next = ll->start;
        ll->start = node;
    }

    ll->nodesCount++;
    return 0;
}

/**
 * Incerts the value as a new Node in the LinkedList, right before the first Node matched from the selector function. The search is done from left to right.
 * @param newValue The pointer, which the new Node must contain, as a value.
 * @param selector A pointer to a function, which will select the Node, before which the new Node must be incerted.
 * The selector funtion must receive a pointer (the value of the Node, which is currently under evaluation) as a single argument and must return 1 if this Node is the one. Any value other than 1 is treated as false. If NULL is give for this argument, NULL is returned.
 * @param ll A pointer to the LinkedList, in which the new Node must be incerted.
 * @return 0 on success, else the error code (>= 1) as an integer.
 */
int incertBeforeLL(void *newValue, int (*selector)(void *value), LinkedList *ll)
{
    if (newValue == NULL || ll == NULL)
        return 1;

    Node *before = NULL;
    Node *n = ll->start;
    if (n == NULL)
    {
        return 10; // error code: Empty LinkedList.
    }

    for (int i = 0; i < ll->nodesCount; i++)
    {
        if (n == NULL)
        {
            return 404;
        }
        if (selector(n->value) != 1)
        {
            before = n;
            n = n->next;
            continue;
        }

        Node *newNode = newNodeLL(newValue);
        if (newNode == NULL)
            return 2;

        if (before == NULL)
        {
            // case: incert before start
            newNode->next = n;
            ll->start = newNode;
        }
        else
        {
            // case: incert before middle node or before end
            before->next = newNode;
            newNode->next = n;
        }
        (ll->nodesCount)++;
        return 0;
    }
}

/**
 * Incerts the value as a new Node in the LinkedList, right after the first found Node with a matching value. The search is done from left to right.
 * @param newValue The pointer, which the new Node must contain, as a value.
 * @param after The pointer contained in the Node, after which the new Node will be incerted.
 * @param ll A pointer to the LinkedList, in which the new Node must be incerted.
 * @return 0 on success, else the error code (>= 1) as an integer.
 */
int incertAfterLL(void *newValue, void *after, LinkedList *ll)
{
    if (newValue == NULL || after == NULL || ll == NULL)
        return 1;

    Node *currentNode = ll->start;
    if (currentNode == NULL)
    {
        return 10; // error code: Empty LinkedList.
    }

    for (int i = 0; i < ll->nodesCount; i++)
    {
        if (currentNode->value != after)
        {
            if (currentNode->next == NULL)
            {
                return 404; // error code: Node not found in LinkedList.
            }
            currentNode = currentNode->next;
            continue;
        }
        else
        {
            // after found
            Node *node = newNodeLL(newValue);
            if (node == NULL)
                return 2;

            if (currentNode == ll->start && ll->end == NULL)
            {
                // case: one node at start
                node->next == ll->start;
                ll->end = ll->start;
                ll->start = node;
            }
            else if (currentNode == ll->end)
            {
                // case: after is the end node
                ll->end->next = node;
                ll->end = node;
            }
            else
            {
                // case: after is between start and end
                node->next = currentNode->next;
                currentNode->next = node;
            }
            ll->nodesCount++;
            return 0;
        }
    }
}

Node *findLL(int (*selector)(void *value), LinkedList *ll)
{
    if (selector == NULL || ll == NULL)
        return NULL;

    Node *n = ll->start;
    int iterations = 0;
    while (1)
    {
        if (n == NULL || iterations >= ll->nodesCount)
        {
            return NULL;
        }

        if (selector(n->value) == 1)
        {
            return n;
        }

        n = n->next;
        iterations++;
    }
    return NULL;
}
/**
 * Removes one Node from the LinkedList.
 * @param selector A pointer to a function, which will select the Node for removal.
 * The selector funtion must receive a pointer (the value of the Node, which is currently under evaluation) as a single argument and must return 1 if this Node must be removed. Any value other than 1 is treated as false. If NULL is give for this argument, NULL is returned.
 * @param ll A pointer to the LinkedList, in which the Node for removal should be contained. If NULL, NULL is returned.
 * @return On success: a pointer to the removed Node. NULL is returned if no Node was removed.
 *
 */
Node *removeNodeLL(int (*selector)(void *value), LinkedList *ll)
{
    if (selector == NULL || ll == NULL)
        return NULL;

    Node *before = NULL;
    Node *n = ll->start;
    int iterations = 0;
    while (1)
    {
        if (n == NULL || iterations >= ll->nodesCount)
        {
            return NULL;
        }

        Node *after = n->next;

        if (selector(n->value) != 1)
        {
            before = n;
            n = n->next;
            iterations++;
            continue;
        }

        if (before == NULL)
        {
            // case: remove start; loop has just started
            if (after == NULL)
            {
                // case: ll containst only start node
                ll->start = NULL;
                ll->nodesCount = 0;
                return n;
            }
            else if (after != NULL)
            {
                // case: ll contains min 2 nodes (start and end), or more
                if (after == ll->end)
                {
                    // case: ll contains only 2 nodes
                    ll->start = after;
                    ll->end = NULL;
                    ll->nodesCount = 1;
                    return n;
                }
                else
                {
                    // case: ll contains more than 2 nodes
                    ll->start = after;
                    (ll->nodesCount)--;
                    return n;
                }
            }
        }
        else
        {
            // case: remove middle or end
            before->next = after;
            if (after == NULL)
            {
                // case: remove end
                ll->end = before;
            }
            (ll->nodesCount)--;
            return n;
        }
    }
    return NULL;
}