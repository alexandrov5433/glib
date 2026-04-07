#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>

#ifdef _WIN32
#ifdef GALXLIB_EXPORTS
#define GLIB_API __declspec(dllexport)
#else
#define GLIB_API __declspec(dllimport)
#endif
#else
#define GLIB_API
#endif

#ifndef DYNAMIC_ARRAY_INIT_CAPACITY
#define DYNAMIC_ARRAY_INIT_CAPACITY 200
#endif

enum DynamicArrayType
{
    INT = 0,
    CHAR = 1,
    FLOAT = 2,
    DOUBLE = 3,
    VOID_PTR = 4
};

typedef struct DynamicArray
{
    union
    {
        int *intArr;
        char *charArr;
        float *floatArr;
        double *doubleArr;
        void **voidArr;
    };
    size_t count;
    size_t capacity;
    size_t singleItemSize;
    enum DynamicArrayType type;
} DynamicArray;

/**
 * Creates a new DynamicArray.
 * @param type The type of values the array will hold, according to the DynamicArrayType enum.
 * If an invalid value is given, NULL is returned.
 * @returns The pointer to the new DynamicArray.
 */
GLIB_API DynamicArray *new_dynamic_array(enum DynamicArrayType const type);

/**
 * Frees the memory used by the DynamicArray. The items contained in the array are not freed.
 * @param da A pointer to the DynamicArray, which must be freed. If NULL, nothing is done.
 */
GLIB_API void free_dynamic_array(DynamicArray *const da);

/**
 * Adds the integer to the end of the DynamicArray.
 * @param item The integer to add.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. On failure: 
 * 
 * 1 if the pointer to the DynamicArray is NULL. 
 * 
 * 2 if memory could not be reallocated.
 * 
 * 3 if the type (enum) of the DynamicArray is not recognized or does not match the type of the item argument.
 */
GLIB_API int push_int_da(DynamicArray *const da, int const item);

/**
 * Adds the character to the end of the DynamicArray.
 * @param item The character to add.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. On failure: 
 * 
 * 1 if the pointer to the DynamicArray is NULL. 
 * 
 * 2 if memory could not be reallocated.
 * 
 * 3 if the type (enum) of the DynamicArray is not recognized or does not match the type of the item argument.
 */
GLIB_API int push_char_da(DynamicArray *const da, char const item);

/**
 * Adds the floater to the end of the DynamicArray.
 * @param item The floater to add.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. On failure: 
 * 
 * 1 if the pointer to the DynamicArray is NULL. 
 * 
 * 2 if memory could not be reallocated.
 * 
 * 3 if the type (enum) of the DynamicArray is not recognized or does not match the type of the item argument.
 */
GLIB_API int push_float_da(DynamicArray *const da, float const item);

/**
 * Adds the double to the end of the DynamicArray.
 * @param item The double to add.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. On failure: 
 * 
 * 1 if the pointer to the DynamicArray is NULL. 
 * 
 * 2 if memory could not be reallocated.
 * 
 * 3 if the type (enum) of the DynamicArray is not recognized or does not match the type of the item argument.
 */
GLIB_API int push_double_da(DynamicArray *const da, double const item);

/**
 * Adds the pointer to the end of the DynamicArray.
 * @param item The pointer to add.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. On failure: 
 * 
 * 1 if the pointer to the DynamicArray is NULL. 
 * 
 * 2 if memory could not be reallocated.
 * 
 * 3 if the type (enum) of the DynamicArray is not recognized or does not match the type of the item argument.
 */
GLIB_API int push_ptr_da(DynamicArray *const da, void *const item);

/**
 * Adds the given integer at the start (index 0) of the DynamicArray, after shifting the items to the right by one.
 * @param item The integer to add.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. On failure: 
 * 
 * 1 if the pointer to the DynamicArray is NULL. 
 * 
 * 2 if memory could not be reallocated.
 * 
 * 3 if the type (enum) of the DynamicArray is not recognized or does not match the type of the item argument.
 */
GLIB_API int unshift_int_da(DynamicArray *const da, int const item);

/**
 * Adds the given character at the start (index 0) of the DynamicArray, after shifting the items to the right by one.
 * @param item The character to add.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. On failure: 
 * 
 * 1 if the pointer to the DynamicArray is NULL. 
 * 
 * 2 if memory could not be reallocated.
 * 
 * 3 if the type (enum) of the DynamicArray is not recognized or does not match the type of the item argument.
 */
GLIB_API int unshift_char_da(DynamicArray *const da, char const item);

/**
 * Adds the given floater at the start (index 0) of the DynamicArray, after shifting the items to the right by one.
 * @param item The floater to add.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. On failure: 
 * 
 * 1 if the pointer to the DynamicArray is NULL. 
 * 
 * 2 if memory could not be reallocated.
 * 
 * 3 if the type (enum) of the DynamicArray is not recognized or does not match the type of the item argument.
 */
GLIB_API int unshift_float_da(DynamicArray *const da, float const item);

/**
 * Adds the given double at the start (index 0) of the DynamicArray, after shifting the items to the right by one.
 * @param item The double to add.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. On failure: 
 * 
 * 1 if the pointer to the DynamicArray is NULL. 
 * 
 * 2 if memory could not be reallocated.
 * 
 * 3 if the type (enum) of the DynamicArray is not recognized or does not match the type of the item argument.
 */
GLIB_API int unshift_double_da(DynamicArray *const da, double const item);

/**
 * Adds the given pointer at the start (index 0) of the DynamicArray, after shifting the items to the right by one.
 * @param item The pointer to add.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. On failure: 
 * 
 * 1 if the pointer to the DynamicArray is NULL. 
 * 
 * 2 if memory could not be reallocated.
 * 
 * 3 if the type (enum) of the DynamicArray is not recognized or does not match the type of the item argument.
 */
GLIB_API int unshift_ptr_da(DynamicArray *const da, void *const item);

/**
 * Places the last item (index == count - 1) from the DynamicArray at the address of the output pointer. The item is removed from the array.
 * The item is cast to the type of the DynamicArray.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output An int pointer, at the address of which the value of the item will be placed.
 * @returns 0 on success. 1 if the type of the DynamicArray is empty. 3 if the type in the DynamicArray is not recognized.
 */
GLIB_API int pop_da(DynamicArray *const da, void *const output);

/**
 * Places the first item (index == 0) from the DynamicArray at the address of the output pointer. The item is removed from the array.
 * The item is cast to the type of the DynamicArray.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output An int pointer, at the address of which the value of the item will be placed.
 * @returns 0 on success. 1 if the type of the DynamicArray is empty. 3 if the type of the DynamicArray is not recognized.
 */
GLIB_API int shift_da(DynamicArray *const da, void *const output);

/**
 * Removes the item at the given index from the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param index The index of the target which is to be removed.
 * @returns 0 on success. On failure:
 *
 * 1 if either of the arguments is NULL.
 *
 * 2 if the removal failed due to memory allocation failure - new_dynamic_array returned NULL.
 *
 * 3 if the removal failed due to an error status retuned from push_da.
 */
GLIB_API int remove_at_da(DynamicArray *const da, const size_t index);

/**
 * Removes the first matched item from the DynamicArray. The search is done from left to right.
 * @param da A pointer to the DynamicArray.
 * @param target A pointer to the target which is to be removed.
 * If the DynamicArray is of type VOID_PTR the target is used as it is, otherwise it is dereferenced.
 * @returns 0 on success - the item was removed. -1 if no match was found. On failure:
 *
 * 1 if either of the arguments is NULL.
 *
 * 2 if the removal failed due to memory allocation failure - new_dynamic_array returned NULL.
 *
 * 3 if the removal failed due to an error status retuned from push_da.
 */
GLIB_API int remove_first_da(DynamicArray *const da, void *const target);

/**
 * Applies a function to the item at the given index in the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param index The index at which the item can be found. The function will be applied to the item at this index.
 * @param worker The function which will be applied to the item.
 * @returns 0 on success. On failure:
 * 
 * 1 if either of the pointer arguments - da, worker - is NULL.
 * 
 * 2 if the item on the given index could not be accessed due to type missmatch (enum DynamicArrayType type) of the DynamicArray. 
 */
GLIB_API int apply_at_da(const DynamicArray *const da, const size_t index, const void (*worker)(void *item_ptr));

/**
 * Applies a processor function to every item in the DynamicArray, from left to right.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param processor A function pointer to the function, which will process the items. If NULL, 1 is returned.
 * If the type of the array is VOID_PTR, the processor receives the item directly.
 * For any other type, the processor receives a pointer to the item, which is cast to void pointer, regardless of the type of the array.
 * The processor also receives a loopBreakTrigger (int *), which breaks the loop when it's value is set to 1.
 * @returns 0 on success. 2 if the pointer to the item in the array could not be retrieved.
 */
GLIB_API int process_da(DynamicArray *const da, void (*processor)(void *item_ptr));

/**
 * Filters the given DynamicArray, leaving only the items selected by the filter function.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param filter A function pointer to the function, which will select the wanted items. If NULL, 1 is returned.
 * If the type of the array is VOID_PTR, the filter receives the item directly.
 * For any other type, the filter receives a pointer to the item, which is cast to void pointer, regardless of the type of the array. The filter must return 1 if the item is to stay in the array. Any other value will lead to the removal of the item.
 * @returns 0 on success. 2 if the pointer to the item in the array could not be retrieved.
 */
GLIB_API int filter_da(DynamicArray *const da, int (*filter)(void *item_ptr));

/**
 * Gets the item at a given index, without modifing the DynamicArray.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param output A double void pointer, which will receive the address of the item.
 * If the type of the array is VOID_PTR, the item it self (which is a void pointer) is placed in the output.
 * For any other type, the address of the item, cast to void pointer regardless of the type of the array, is placed in output.
 * @returns 0 on success. 1 if the given index is out of bounds.
 */
GLIB_API int at_da(DynamicArray *const da, size_t index, void **output);

/**
 * Finds a specific item in the DynamicArray, without modifing the array. The search is done from left to right.
 * The search stops as soon as the selector chooses one item.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param output A double void pointer, which will receive the address of the item. If NULL, 1 is returned.
 * If the type of the array is VOID_PTR, the item it self (which is a void pointer) is placed in the output.
 * @param selector A function pointer to the function, which will select the item. If NULL, 1 is returned.
 * If the type of the array is VOID_PTR, the selector receives the item directly.
 * For any other type, the selector receives a pointer to the item, which is cast to void pointer, regardless of the type of the array. The selector must return 1 to indicate the desired item. Any other value is treated as false.
 * @returns 0 on success. -1 if the item was not found.
 */
GLIB_API int find_da(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr));

/**
 * Finds a specific item in the DynamicArray, without modifing the array. The search is done from right to left.
 * The search stops as soon as the selector chooses one item.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param output A double void pointer, which will receive the address of the item. If NULL, 1 is returned.
 * If the type of the array is VOID_PTR, the item it self (which is a void pointer) is placed in the output.
 * @param selector A function pointer to the function, which will select the item. If NULL, 1 is returned.
 * If the type of the array is VOID_PTR, the selector receives the item directly.
 * For any other type, the selector receives a pointer to the item, which is cast to void pointer, regardless of the type of the array. The selector must return 1 to indicate the desired item. Any other value is treated as false.
 * @returns 0 on success. -1 if the item was not found.
 */
GLIB_API int find_last_da(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr));

/**
 * Finds the index of a specific item in the DynamicArray, without modifing the array.
 * The search is done from left to right.
 * The search stops as soon as the selector chooses one item.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param output An int pointer, which will receive the index of the item. If NULL, 1 is returned.
 * @param selector A function pointer to the function, which will select the item. If NULL, 1 is returned.
 * If the type of the array is VOID_PTR, the selector receives the item directly.
 * For any other type, the selector receives a pointer to the item, which is cast to void pointer, regardless of the type of the array. The selector must return 1 to indicate the desired item. Any other value is treated as false.
 * @returns 0 on success. -1 if the item was not found.
 */
GLIB_API int find_index_da(DynamicArray *const da, int *const output, int (*selector)(void *itemPtr));

/**
 * Finds the index of a specific item in the DynamicArray, without modifing the array.
 * The search is done from right to left.
 * The search stops as soon as the selector chooses one item.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param output An int pointer, which will receive the index of the item. If NULL, 1 is returned.
 * @param selector A function pointer to the function, which will select the item. If NULL, 1 is returned.
 * If the type of the array is VOID_PTR, the selector receives the item directly.
 * For any other type, the selector receives a pointer to the item, which is cast to void pointer, regardless of the type of the array. The selector must return 1 to indicate the desired item. Any other value is treated as false.
 * @returns 0 on success. -1 if the item was not found.
 */
GLIB_API int find_last_index_da(DynamicArray *const da, int *const output, int (*selector)(void *itemPtr));

/**
 * Finds the index of a specific item in the DynamicArray, without modifing the array, by directly comparing items to the given value.
 * The search is done from left to right and stops as soon as there is a match.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param output An int pointer, which will receive the index of the item.
 * @param value A void pointer to the value, against which the items will be compared. If NULL, 1 is returned.
 * @returns 0 on success. -1 if the item was not found.
 */
GLIB_API int index_of_da(DynamicArray *const da, int *const output, void *const value);

#endif