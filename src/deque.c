#include "../include/deque.h"
#include <stdlib.h>

// #######################    static    #######################
/** Creates a new deque_node_t using calloc. */
static inline enum GalxlibError _new_deque_node(deque_node_t **const restrict output)
{
    if (NULL == output)
        return GLX_ERR_NULL_ARGUMENT;

    deque_node_t *node = calloc(1, sizeof(deque_node_t));
    if (NULL == node)
        return GLX_ERR_MEMORY_ALLOCATION;

    /*
    https://stackoverflow.com/questions/5857588/calloc-with-structure-with-pointers-in-c#5857706
    Calloc sets all bits to 0, but a NULL-pointer may not be represented on
    all systems as only 0 bits. There are exceptions.
    Therefore, the pointers are set to NULL explicitly.
     */

    node->payload = NULL;
    node->previous = NULL;
    node->next = NULL;

    *output = node;
    return GLX_SUCCESS;
}

static inline enum GalxlibError _free_deque_node(deque_node_t *node)
{
    if (NULL == node)
        return GLX_ERR_NULL_ARGUMENT;

    free(node);

    return GLX_SUCCESS;
}

static inline enum GalxlibError _free_deque_node_d(deque_node_t *node, void (*destructor)(void **ptr))
{
    if (NULL == node || NULL == destructor)
        return GLX_ERR_NULL_ARGUMENT;

    destructor(&(node->payload));
    free(node);

    return GLX_SUCCESS;
}

// #######################    public    #######################
enum GalxlibError new_deque(deque_t **const restrict output)
{
    if (NULL == output)
        return GLX_ERR_NULL_ARGUMENT;

    deque_t *d = calloc(1, sizeof(deque_t));
    if (NULL == d)
        return GLX_ERR_MEMORY_ALLOCATION;

    d->start_node = NULL;
    d->end_node = NULL;
    d->destructor = NULL;
    d->length = 0;

    *output = d;

    return GLX_SUCCESS;
}

enum GalxlibError new_deque_d(void (*destructor)(void **ptr), deque_t **const restrict output)
{
    if (NULL == output || NULL == destructor)
        return GLX_ERR_NULL_ARGUMENT;

    deque_t *d = NULL;
    enum GalxlibError err = new_deque(&d);
    if (err)
        return err;

    d->destructor = destructor;

    *output = d;

    return GLX_SUCCESS;
}

enum GalxlibError free_deque(deque_t **const deque)
{
    if (NULL == deque)
        return GLX_ERR_NULL_ARGUMENT;
    if (NULL == *deque)
        return GLX_SUCCESS;

    deque_node_t *node = (*deque)->start_node;
    if (NULL == (*deque)->destructor)
    {
        for (size_t i = 0; i < (*deque)->length; ++i)
        {
            deque_node_t *tmp = node->next;
            _free_deque_node(node);
            node = tmp;
        }
    }
    else
    {
        for (size_t i = 0; i < (*deque)->length; ++i)
        {
            deque_node_t *tmp = node->next;
            _free_deque_node_d(node, (*deque)->destructor);
            node = tmp;
        }
    }

    *deque = NULL;

    return GLX_SUCCESS;
}

enum GalxlibError add_start_deque(deque_t *const restrict deque, void *element)
{
    if (NULL == deque || NULL == element)
        return GLX_ERR_NULL_ARGUMENT;

    deque_node_t *node = NULL;
    enum GalxlibError err_node = _new_deque_node(&node);
    if (err_node)
        return err_node;

    node->payload = element;
    if (deque->length > 0)
    {
        deque->start_node->previous = node;
        node->next = deque->start_node;
        deque->start_node = node;
    }
    else
    {
        // this node is the first one
        deque->start_node = node;
        deque->end_node = node;
    }

    deque->length++;

    return GLX_SUCCESS;
}

enum GalxlibError add_end_deque(deque_t *const restrict deque, void *element)
{
    if (NULL == deque || NULL == element)
        return GLX_ERR_NULL_ARGUMENT;

    deque_node_t *node = NULL;
    enum GalxlibError err_node = _new_deque_node(&node);
    if (err_node)
        return err_node;

    node->payload = element;
    if (deque->length > 0)
    {
        deque->end_node->next = node;
        node->previous = deque->end_node;
        deque->end_node = node;
    }
    else
    {
        // this node is the first one
        deque->start_node = node;
        deque->end_node = node;
    }

    deque->length++;

    return GLX_SUCCESS;
}

enum GalxlibError remove_start_deque(deque_t *const restrict deque, void **const output)
{
    if (NULL == deque)
        return GLX_ERR_NULL_ARGUMENT;

    if (deque->length == 0)
        return GLX_ERR_ZERO_LENGTH;

    void *element = deque->start_node->payload;

    if (deque->length == 1)
    {
        if (NULL == deque->destructor)
            _free_deque_node(deque->start_node);
        else
            _free_deque_node_d(deque->start_node, deque->destructor);

        deque->start_node = NULL;
        deque->end_node = NULL;
    }
    else
    {
        // there are 2 or more nodes
        deque_node_t *node_to_free = deque->start_node;
        deque->start_node = deque->start_node->next;
        deque->start_node->previous = NULL;

        if (NULL == deque->destructor)
            _free_deque_node(node_to_free);
        else
            _free_deque_node_d(node_to_free, deque->destructor);
    }

    if (NULL != output)
        *output = element;

    deque->length--;

    return GLX_SUCCESS;
}

enum GalxlibError remove_end_deque(deque_t *const restrict deque, void **const output)
{
    if (NULL == deque)
        return GLX_ERR_NULL_ARGUMENT;

    if (deque->length == 0)
        return GLX_ERR_ZERO_LENGTH;

    void *element = deque->end_node->payload;

    if (deque->length == 1)
    {
        if (NULL == deque->destructor)
            _free_deque_node(deque->end_node);
        else
            _free_deque_node_d(deque->end_node, deque->destructor);

        deque->start_node = NULL;
        deque->end_node = NULL;
    }
    else
    {
        // there are 2 or more nodes
        deque_node_t *node_to_free = deque->end_node;
        deque->end_node = deque->end_node->previous;
        deque->end_node->next = NULL;

        if (NULL == deque->destructor)
            _free_deque_node(node_to_free);
        else
            _free_deque_node_d(node_to_free, deque->destructor);
    }

    if (NULL != output)
        *output = element;

    deque->length--;

    return GLX_SUCCESS;
}