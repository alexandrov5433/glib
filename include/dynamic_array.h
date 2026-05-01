#ifndef GALXLIB_DYNAMIC_ARRAY_H
#define GALXLIB_DYNAMIC_ARRAY_H

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

#ifndef DYNAMIC_ARRAY_INIT_CAPACITY
#define DYNAMIC_ARRAY_INIT_CAPACITY 200
#endif

/**
 * @enum DynamicArrayError
 * @brief The error codes returned by the DynamicArray functions.
 */
enum DynamicArrayError
{
	DA_ARRAY_EMPTY = -2,		   /**< (-2) The array does not include any items. */
	DA_ITEM_NOT_FOUND = -1,		   /**< (-1) The searched item was not found among the items in the array. */
	DA_SUCCESS = 0,			   /**< (0) Successful execution of the called function. */
	DA_ERR_NULL_ARGUMENT = 1,	   /**< (1) One or more arguments are NULL. */
	DA_ERR_MEMORY_ALLOCATION = 2,	   /**< (2) Failed to allocate or reallocate memory. */
	DA_ERR_TYPE_MISMATCH = 3,	   /**< (3) The type (DynamicArrayType) of the DynamicArray does not match the type, which the called function processes. */
	DA_ERR_TYPE_UNKNOWN = 4,	   /**< (4) The type (DynamicArrayType) is unknown and not supported. */
	DA_ERR_INDEX_OUT_OF_BOUNDS = 5,	   /**< (5) The targeted index is outside of the boundaries of the DynamicArray. */
	DA_ERR_ITEM_SIZE_DETERMINATION = 6 /**< (6) The size in bytes of a single item could not be determined, based on the given type argument. */
};

/**
 * @enum DynamicArrayType
 * @brief The enumeration of the types, which are allowed in the array.
 */
enum DynamicArrayType
{
	INT = 0,
	CHAR = 1,
	FLOAT = 2,
	DOUBLE = 3,
	VOID_PTR = 4
};

/**
 * @struct DynamicArray
 * @brief The stucture containing the actual array and all other required data.
 */
typedef struct DynamicArray
{
	union
	{
		int *int_arr;
		char *char_arr;
		float *float_arr;
		double *double_arr;
		void **void_arr;
	};
	size_t count;		    /**< The current number of items in the array. */
	size_t capacity;	    /**< The total number of places (indexes) in the array. This is not the total allocated memory. */
	size_t single_item_size;    /**< The size in bytes of a singe item contained in the array. */
	enum DynamicArrayType type; /**< Indicates the type of items contained in the array. */
} DynamicArray;

/**
 * @struct DynamicArrayiterator
 * @brief A stucture used to securely iterate through the element of the @ref DynamicArray.
 */
typedef struct DynamicArrayIterator
{
	struct DynamicArray *da; /**< A pointer to the @ref DynamicArray for iteration. */
	size_t current_index;	 /**< The index, keeping track of the progression of the iteration process. */
} DynamicArrayIterator;

/**
 * Creates a new DynamicArray.
 * @param type The type of values the array will hold, according to the DynamicArrayType enum.
 * @param output A pointer, which will be updated with the adress of the new DynamicArray.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_ITEM_SIZE_DETERMINATION
 */
GALXLIB_API enum DynamicArrayError new_dynamic_array(enum DynamicArrayType const type, DynamicArray **const output);

/**
 * Frees the memory used by the DynamicArray. The items contained in the array are not freed.
 * @param da A pointer to the DynamicArray, which must be freed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError free_dynamic_array(DynamicArray *const da);

/**
 * Adds the integer to the end of the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param item The integer to add.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError push_int_da(DynamicArray *const da, int const item);

/**
 * Adds the character to the end of the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param item The character to add.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError push_char_da(DynamicArray *const da, char const item);

/**
 * Adds the float to the end of the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param item The float to add.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError push_float_da(DynamicArray *const da, float const item);

/**
 * Adds the double to the end of the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param item The double to add.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError push_double_da(DynamicArray *const da, double const item);

/**
 * Adds the pointer to the end of the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param item The pointer to add.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError push_ptr_da(DynamicArray *const da, void *const item);

/**
 * Adds the given integer at the start (index 0) of the DynamicArray, after shifting the items to the right by one.
 * @param da A pointer to the DynamicArray.
 * @param item The integer to add.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError unshift_int_da(DynamicArray *const da, int const item);

/**
 * Adds the given character at the start (index 0) of the DynamicArray, after shifting the items to the right by one.
 * @param da A pointer to the DynamicArray.
 * @param item The character to add.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError unshift_char_da(DynamicArray *const da, char const item);

/**
 * Adds the given float at the start (index 0) of the DynamicArray, after shifting the items to the right by one.
 * @param da A pointer to the DynamicArray.
 * @param item The float to add.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError unshift_float_da(DynamicArray *const da, float const item);

/**
 * Adds the given double at the start (index 0) of the DynamicArray, after shifting the items to the right by one.
 * @param da A pointer to the DynamicArray.
 * @param item The double to add.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError unshift_double_da(DynamicArray *const da, double const item);

/**
 * Adds the given pointer at the start (index 0) of the DynamicArray, after shifting the items to the right by one.
 * @param da A pointer to the DynamicArray.
 * @param item The pointer to add.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError unshift_ptr_da(DynamicArray *const da, void *const item);

/**
 * Removes the last item from the DynamicArray and places it at the address of the output pointer. The item is removed from the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param output A pointer where the item will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError pop_int_da(DynamicArray *const da, int *const output);

/**
 * Removes the last item from the DynamicArray and places it at the address of the output pointer. The item is removed from the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param output A pointer where the item will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError pop_char_da(DynamicArray *const da, char *const output);

/**
 * Removes the last item from the DynamicArray and places it at the address of the output pointer. The item is removed from the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param output A pointer where the item will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError pop_float_da(DynamicArray *const da, float *const output);

/**
 * Removes the last item from the DynamicArray and places it at the address of the output pointer. The item is removed from the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param output A pointer where the item will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError pop_double_da(DynamicArray *const da, double *const output);

/**
 * Removes the last item from the DynamicArray and places it at the address of the output pointer. The item is removed from the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param output A pointer where the item will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError pop_ptr_da(DynamicArray *const da, void **const output);

/**
 * Gets the first item from the DynamicArray and places it at the address of the output pointer. The item is removed from the array.
 * @param da A pointer to the DynamicArray.
 * @param output A pointer the item will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError shift_int_da(DynamicArray *const da, int *const output);

/**
 * Gets the first item from the DynamicArray and places it at the address of the output pointer. The item is removed from the array.
 * @param da A pointer to the DynamicArray.
 * @param output A pointer the item will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError shift_char_da(DynamicArray *const da, char *const output);

/**
 * Gets the first item from the DynamicArray and places it at the address of the output pointer. The item is removed from the array.
 * @param da A pointer to the DynamicArray.
 * @param output A pointer the item will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError shift_float_da(DynamicArray *const da, float *const output);

/**
 * Gets the first item from the DynamicArray and places it at the address of the output pointer. The item is removed from the array.
 * @param da A pointer to the DynamicArray.
 * @param output A pointer the item will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError shift_double_da(DynamicArray *const da, double *const output);

/**
 * Gets the first item from the DynamicArray and places it at the address of the output pointer. The item is removed from the array.
 * @param da A pointer to the DynamicArray.
 * @param output A pointer the item will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 */
GALXLIB_API enum DynamicArrayError shift_ptr_da(DynamicArray *const da, void **const output);

/**
 * Removes the item at the given index from the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param index The index of the target which is to be removed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_ITEM_SIZE_DETERMINATION
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError remove_at_da(DynamicArray *const da, const size_t index);

/**
 * Removes the first matched item from the DynamicArray. The search is done from left to right.
 * @param da A pointer to the DynamicArray.
 * @param target A pointer to the target which is to be removed.
 * If the DynamicArray is of type VOID_PTR the target is used as it is, otherwise it is dereferenced.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ITEM_NOT_FOUND
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_ITEM_SIZE_DETERMINATION
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_TYPE_MISMATCH
 *
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError remove_first_da(DynamicArray *const da, void *const target);

/**
 * Applies a function to the item at the given index in the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param index The index at which the item can be found. The function will be applied to the item at this index.
 * @param worker The function which will be applied to the item.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError apply_at_da(const DynamicArray *const da, const size_t index, const void (*worker)(void *item_ptr));

/**
 * Applies a processor function to every item in the DynamicArray, from left to right.
 * If the DynamicArray is empty, nothing is done and DA_SUCCESS is returned.
 * @param da A pointer to the DynamicArray.
 * @param processor A function pointer to the function, which will process the items.
 * If the type of the array is VOID_PTR, the processor receives the item directly.
 * For any other type, the processor receives a pointer to the item, regardless of the type of the array.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError process_da(DynamicArray *const da, void (*processor)(void *item_ptr));

/**
 * Filters the given DynamicArray, leaving only the items selected by the filter function.
 * If the DynamicArray is empty, nothing is done and DA_SUCCESS is returned.
 * @param da A pointer to the DynamicArray.
 * @param filter A function pointer to the function, which will select the wanted items.
 * If the type of the array is VOID_PTR, the filter receives the item directly.
 * For any other type, the filter receives a pointer to the item, regardless of the type of the array.
 * The filter must return 1 if the item is to stay in the array. Any other value will lead to the removal of the item.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 *
 * - DA_ERR_MEMORY_ALLOCATION
 *
 * - DA_ERR_ITEM_SIZE_DETERMINATION
 *
 * - DA_ERR_TYPE_MISMATCH
 */
GALXLIB_API enum DynamicArrayError filter_da(DynamicArray *const da, int (*filter)(void *item_ptr));

/**
 * Gets the item at a given index, without modifing the DynamicArray.
 * @param da A pointer to the DynamicArray.
 * @param output A double void pointer, which will receive the address of the item.
 * If the type of the array is VOID_PTR, the item it self (which is a void pointer) is placed in the output.
 * For any other type, the address of the item, cast to void pointer regardless of the type of the array, is placed in output.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ARRAY_EMPTY
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError at_da(DynamicArray *const da, size_t index, void **output);

/**
 * Finds a specific item in the DynamicArray, without modifing the array. The search is done from left to right.
 * The search stops as soon as the selector chooses one item.
 * @param da A pointer to the DynamicArray.
 * @param output A double void pointer, which will receive the address of the item.
 * If the type of the array is VOID_PTR, the item it self (which is a void pointer) is placed in the output.
 * @param selector A function pointer to the function, which will select the item.
 * If the type of the array is VOID_PTR, the selector receives the item directly.
 * For any other type, the selector receives a pointer to the item, which is cast to void pointer, regardless of the type of the array.
 * The selector must return 1 to indicate the desired item. Any other value is treated as false.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ITEM_NOT_FOUND
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError find_da(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr));

/**
 * Finds a specific item in the DynamicArray, without modifing the array. The search is done from right to left.
 * The search stops as soon as the selector chooses one item.
 * @param da A pointer to the DynamicArray.
 * @param output A double void pointer, which will receive the address of the item.
 * If the type of the array is VOID_PTR, the item it self (which is a void pointer) is placed in the output.
 * @param selector A function pointer to the function, which will select the item.
 * If the type of the array is VOID_PTR, the selector receives the item directly.
 * For any other type, the selector receives a pointer to the item, which is cast to void pointer, regardless of the type of the array.
 * The selector must return 1 to indicate the desired item. Any other value is treated as false.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ITEM_NOT_FOUND
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError find_last_da(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr));

/**
 * Finds the index of a specific item in the DynamicArray, without modifing the array.
 * The search is done from left to right.
 * The search stops as soon as the selector chooses one item.
 * @param da A pointer to the DynamicArray.
 * @param output An int pointer, which will receive the index of the item.
 * @param selector A function pointer to the function, which will select the item.
 * If the type of the array is VOID_PTR, the selector receives the item directly.
 * For any other type, the selector receives a pointer to the item, which is cast to void pointer, regardless of the type of the array.
 * The selector must return 1 to indicate the desired item. Any other value is treated as false.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ITEM_NOT_FOUND
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError find_index_da(DynamicArray *const da, size_t *const output, int (*selector)(void *itemPtr));

/**
 * Finds the index of a specific item in the DynamicArray, without modifing the array.
 * The search is done from right to left.
 * The search stops as soon as the selector chooses one item.
 * @param da A pointer to the DynamicArray.
 * @param output An int pointer, which will receive the index of the item.
 * @param selector A function pointer to the function, which will select the item.
 * If the type of the array is VOID_PTR, the selector receives the item directly.
 * For any other type, the selector receives a pointer to the item, which is cast to void pointer, regardless of the type of the array. The selector must return 1 to indicate the desired item. Any other value is treated as false.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ITEM_NOT_FOUND
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError find_last_index_da(DynamicArray *const da, size_t *const output, int (*selector)(void *itemPtr));

/**
 * Finds the index of a specific item in the DynamicArray, without modifing the array, by directly comparing items to the given value.
 * The search is done from left to right and stops as soon as there is a match.
 * @param da A pointer to the DynamicArray.
 * @param output An int pointer, which will receive the index of the item.
 * @param value A void pointer to the value, against which the items will be compared.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ITEM_NOT_FOUND
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_TYPE_UNKNOWN
 *
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError index_of_da(DynamicArray *const da, size_t *const output, void *const value);

/**
 * Creates a new @ref DynamicArrayIterator.
 * @param da The @ref DynamicArray for iteration.
 * @param output The address, where the new @ref DynamicArrayIterator will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 *
 * - DA_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum DynamicArrayError new_iterator_da(DynamicArray *const da, DynamicArrayIterator **const output);

/**
 * Outputs 1 if there are more elements to iterate through, 0 otherwise.
 * @param itr The @ref DynamicArrayIterator managing the iteration process.
 * @param output The address, where the 1 or 0 output will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum DynamicArrayError has_next_dai(const DynamicArrayIterator *const itr, int *const output);

/**
 * Outputs the next element from the iteration process.
 * @param itr The @ref DynamicArrayIterator managing the iteration process.
 * @param output The address, where the element will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 * 
 * - DA_ERR_TYPE_MISMATCH
 * 
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError next_int_dai(DynamicArrayIterator *const itr, int *const output);

/**
 * Outputs the next element from the iteration process.
 * @param itr The @ref DynamicArrayIterator managing the iteration process.
 * @param output The address, where the element will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 * 
 * - DA_ERR_TYPE_MISMATCH
 * 
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError next_char_dai(DynamicArrayIterator *const itr, char *const output);

/**
 * Outputs the next element from the iteration process.
 * @param itr The @ref DynamicArrayIterator managing the iteration process.
 * @param output The address, where the element will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 * 
 * - DA_ERR_TYPE_MISMATCH
 * 
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError next_float_dai(DynamicArrayIterator *const itr, float *const output);

/**
 * Outputs the next element from the iteration process.
 * @param itr The @ref DynamicArrayIterator managing the iteration process.
 * @param output The address, where the element will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 * 
 * - DA_ERR_TYPE_MISMATCH
 * 
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError next_double_dai(DynamicArrayIterator *const itr, double *const output);

/**
 * Outputs the next element from the iteration process.
 * @param itr The @ref DynamicArrayIterator managing the iteration process.
 * @param output The address, where the element will be placed.
 * @return A value of the @ref DynamicArrayError:
 *
 * - DA_SUCCESS
 *
 * - DA_ERR_NULL_ARGUMENT
 * 
 * - DA_ERR_TYPE_MISMATCH
 * 
 * - DA_ERR_INDEX_OUT_OF_BOUNDS
 */
GALXLIB_API enum DynamicArrayError next_ptr_dai(DynamicArrayIterator *const itr, void **const output);

#endif