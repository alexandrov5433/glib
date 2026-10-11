/**
 * @file deque.h
 * @brief Header file for double-ended queque implementation.
 */

#ifndef GALXLIB_DEQUE_H
#define GALXLIB_DEQUE_H

#include <stddef.h>
#include "./error.h"

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

#else
#define GALXLIB_API

#endif

/**
 * @struct deque_node_t
 * @brief A structure describing a single node in the deque.
 */
typedef struct deque_node_t
{
    void *payload;                 /** The element of this deque_node_t, given by the user. */
    struct deque_node_t *previous; /**< A pointer to the previous element of the deque; on the left side of this element. */
    struct deque_node_t *next;     /**< A pointer to the next element of the deque; on the right side of this element. */
} deque_node_t;

/**
 * @struct deque_t
 * @brief A structure holding the general data of the deque.
 */
typedef struct deque_t
{
    struct deque_node_t *start_node; /**< A pointer to the first element of the deque. */
    struct deque_node_t *end_node;   /**< A pointer to the last element of the deque. */
    void (*destructor)(void **ptr);   /**< A function, which is invoked for each element of the deque, with the pointer of the element as an argument, when the deque structure is freed using the free_deque function. */
    size_t length;                   /**< The total number of elements (deque_node_t) currently in the deque. */
} deque_t;

/**
 * Creates a new deque_t structure.
 * @param output The address where the new structure will be placed.
 * @return A value of the @ref GalxlibError:
 *
 *  - GLX_SUCCESS
 *
 *  - GLX_ERR_NULL_ARGUMENT
 *
 *  - GLX_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum GalxlibError new_deque(deque_t **const restrict output);

/**
 * Creates a new deque_t structure.
 * @param destructor A function, which is invoked for each element of the deque, when the deque structure is freed using the free_deque function.
 * @param output The address where the new structure will be placed.
 * @return A value of the @ref GalxlibError:
 *
 *  - GLX_SUCCESS
 *
 *  - GLX_ERR_NULL_ARGUMENT
 *
 *  - GLX_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum GalxlibError new_deque_d(void (*destructor)(void **ptr), deque_t **const restrict output);

/**
 * Frees the memory of the deque. If a destructor function is present, it is invoked with each element as an argument.
 * The pointer of the deque is set to NULL - *deque = NULL. If the given pointer points to NULL - NULL == *deque -, nothing is done.
 * @param deque The deque to free.
 * @return A value of the @ref GalxlibError:
 *
 *  - GLX_SUCCESS
 *
 *  - GLX_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum GalxlibError free_deque(deque_t **const deque);

/**
 * Adds a new element to the start of the deque.
 * @param deque The deque, which must recieve the element.
 * @param element A pointer to the element to add. The pointer itself may NOT be NULL - NULL == element.
 * No NULL-checks - NULL == *element - are performed for its content; only for the pointer itself!
 * @return A value of the @ref GalxlibError:
 *
 *  - GLX_SUCCESS
 *
 *  - GLX_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum GalxlibError add_start_deque(deque_t *const restrict deque, void *element);

/**
 * Adds a new element to the end of the deque.
 * @param deque The deque, which must recieve the element.
 * @param element A pointer to the element to add. The pointer itself may NOT be NULL - NULL == element.
 * No NULL-checks - NULL == *element - are performed for its content; only for the pointer itself!
 * @return A value of the @ref GalxlibError:
 *
 *  - GLX_SUCCESS
 *
 *  - GLX_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum GalxlibError add_end_deque(deque_t *const restrict deque, void *element);

/**
 * Removes the element from the start of the deque.
 * @param deque The deque, from which to remove. If the deque is empty, GLX_ERR_ZERO_LENGTH is returned.
 * If the deque contains a destructor, it is invoked with the element as an argument.
 * @param output An optional pointer where the pointer to the element of the removed node will be placed.
 * If NULL, the function ignores the output.
 * @return A value of the @ref GalxlibError:
 *
 *  - GLX_SUCCESS
 *
 *  - GLX_ERR_NULL_ARGUMENT
 *
 *  - GLX_ERR_ZERO_LENGTH
 */
GALXLIB_API enum GalxlibError remove_start_deque(deque_t *const restrict deque, void **const output);

/**
 * Removes the element from the end of the deque.
 * @param deque The deque, from which to remove. If the deque is empty, GLX_ERR_ZERO_LENGTH is returned.
 * If the deque contains a destructor, it is invoked with the element as an argument.
 * @param output An optional pointer where the pointer to the element of the removed node will be placed.
 * If NULL, the function ignores the output.
 * @return A value of the @ref GalxlibError:
 *
 *  - GLX_SUCCESS
 *
 *  - GLX_ERR_NULL_ARGUMENT
 *
 *  - GLX_ERR_ZERO_LENGTH
 */
GALXLIB_API enum GalxlibError remove_end_deque(deque_t *const restrict deque, void **const output);

#endif