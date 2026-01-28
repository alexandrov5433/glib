#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#ifdef _WIN32
#ifdef GLIB_EXPORTS
#define GLIB_API __declspec(dllexport)
#else
#define GLIB_API __declspec(dllimport)
#endif
#else
#define GLIB_API
#endif

#ifndef DYNAMIC_ARRAY_MEMORY_INIT_CAPACITY
#define DYNAMIC_ARRAY_MEMORY_INIT_CAPACITY 200
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
    enum DynamicArrayType type;
    size_t singleItemSize;
} DynamicArray;

/**
 * Creates a new DynamicArray.
 * @param type The type of values the array will hold, according to the DynamicArrayType enum.
 * If an invalid value is given, NULL is returned.
 * @returns The pointer to the new DynamicArray.
 */
GLIB_API DynamicArray *newDynamicArray(enum DynamicArrayType const type);

/**
 * Frees the memory used by the DynamicArray. The items contained in the array are not freed.
 * @param da A pointer to the DynamicArray, which must be freed. If NULL, nothing is done.
 */
GLIB_API void freeDynamicArray(DynamicArray *const da);

/**
 * Adds an item to the and of the DynamicArray.
 * @param item A ponter to the item, which must be added. If NULL, 1 is returned.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. 1 if either of the arguments is NULL. 2 if memory could not be reallocated.
 * 3 if the type (enum) is not recognized.
 */
GLIB_API int pushDA(DynamicArray *const da, void *const item);

/**
 * Shifts the items of the DynamicArray to the right and places the given item at the start (index 0).
 * @param item A ponter to the item, which must be added. If NULL, 1 is returned.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @returns 0 on success. 1 if either of the arguments is NULL. 2 if memory could not be reallocated.
 * 3 if the type (enum) is not recognized.
 */
GLIB_API int unshiftDA(DynamicArray *const da, void *const item);

/**
 * Places the last item (index == count - 1) from the DynamicArray at the address of the output pointer. The item is removed from the array. The item is cast to int.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output An int pointer, at the address of which the value of the item will be placed.
 * If NULL, 1 is returned.
 * @returns 0 on success. 1 if the type of the DynamicArray is not INT.
 */
GLIB_API int popInt(DynamicArray *const da, int *const output);

/**
 * Places the last item (index == count - 1) from the DynamicArray at the address of the output pointer. The item is removed from the array. The item is cast to char.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output A char pointer, at the address of which the value of the item will be placed.
 * If NULL, 1 is returned.
 * @returns 0 on success. 1 if the type of the DynamicArray is not CHAR.
 */
GLIB_API int popChar(DynamicArray *const da, char *const output);

/**
 * Places the last item (index == count - 1) from the DynamicArray at the address of the output pointer. The item is removed from the array. The item is cast to float.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output A float pointer, at the address of which the value of the item will be placed.
 * If NULL, 1 is returned.
 * @returns 0 on success. 1 if the type of the DynamicArray is not FLOAT.
 */
GLIB_API int popFloat(DynamicArray *const da, float *const output);

/**
 * Places the last item (index == count - 1) from the DynamicArray at the address of the output pointer. The item is removed from the array. The item is cast to double.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output A double pointer, at the address of which the value of the item will be placed.
 * If NULL, 1 is returned.
 * @returns 0 on success. 1 if the type of the DynamicArray is not DOUBLE.
 */
GLIB_API int popDouble(DynamicArray *const da, double *const output);

/**
 * Places the last item (index == count - 1) from the DynamicArray at the address of the output pointer. The item is removed from the array. The item is cast to void pointer.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output A double void pointer, where the void pointer value of the item will be placed.
 * If NULL, 1 is returned.
 * @returns 0 on success. 1 if the type of the DynamicArray is not VOID_PTR.
 */
GLIB_API int popPointer(DynamicArray *const da, void **const output);

/**
 * Places the first item (index == 0) from the DynamicArray at the address of the output pointer. The item is removed from the array. The item is cast to int.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output An int pointer, at the address of which the value of the item will be placed.
 * If NULL, 1 is returned.
 * @returns 0 on success. 1 if the type of the DynamicArray is not INT.
 */
GLIB_API int shiftInt(DynamicArray *const da, int *const output);

/**
 * Places the first item (index == 0) from the DynamicArray at the address of the output pointer. The item is removed from the array. The item is cast to char.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output A char pointer, at the address of which the value of the item will be placed.
 * If NULL, 1 is returned.
 * @returns 0 on success. 1 if the type of the DynamicArray is not CHAR.
 */
GLIB_API int shiftChar(DynamicArray *const da, char *const output);

/**
 * Places the first item (index == 0) from the DynamicArray at the address of the output pointer. The item is removed from the array. The item is cast to float.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output A float pointer, at the address of which the value of the item will be placed.
 * If NULL, 1 is returned.
 * @returns 0 on success. 1 if the type of the DynamicArray is not FLOAT.
 */
GLIB_API int shiftFloat(DynamicArray *const da, float *const output);

/**
 * Places the first item (index == 0) from the DynamicArray at the address of the output pointer. The item is removed from the array. The item is cast to double.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output A double pointer, at the address of which the value of the item will be placed.
 * If NULL, 1 is returned.
 * @returns 0 on success. 1 if the type of the DynamicArray is not DOUBLE.
 */
GLIB_API int shiftDouble(DynamicArray *const da, double *output);

/**
 * Places the first item (index == 0) from the DynamicArray at the address of the output pointer. The item is removed from the array. The item is cast to void pointer.
 * @param da A pointer to the DynamicArray. If NULL or the array is empty, 1 is returned.
 * @param output A double void pointer, where the void pointer value of the item will be placed.
 * If NULL, 1 is returned.
 * @returns 0 on success. 1 if the type of the DynamicArray is not VIOD_PTR.
 */
GLIB_API int shiftPointer(DynamicArray *const da, void **const output);

/**
 * Applies a processor function to every item in the DynamicArray, from left to right.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param processor A function pointer to the function, which will process the items. If NULL, 1 is returned.
 * If the type of the array is VOID_PTR, the processor receives the item directly.
 * For any other type, the processor receives a pointer to the item, which is cast to void pointer, regardless of the type of the array.
 * The processor also receives a loopBreakTrigger (int *), which breaks the loop when it's value is set to 1.
 * @returns 0 on success.
 */
GLIB_API int processDA(DynamicArray *const da, void (*processor)(void *itemPtr, int *loopBreakTrigger));

/**
 * Filters the given DynamicArray, leaving only the items selected by the filter function.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param filter A function pointer to the function, which will select the wanted items. If NULL, 1 is returned.
 * If the type of the array is VOID_PTR, the filter receives the item directly.
 * For any other type, the filter receives a pointer to the item, which is cast to void pointer, regardless of the type of the array. The filter must return 1 if the item is to stay in the array. Any other value will lead to the removal of the item.
 * @returns 0 on success.
 */
GLIB_API int filterDA(DynamicArray *const da, int (*filter)(void *itemPtr));

/**
 * Gets the item at a given index, without modifing the DynamicArray.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param output A double void pointer, which will receive the address of the item. If NULL, 1 is returned.
 * If the type of the array is VOID_PTR, the item it self (which is a void pointer) is placed in the output.
 * For any other type, the address of the item, cast to void pointer regardless of the type of the array, is placed in output.
 * @returns 0 on success. 1 if the given index is out of bounds.
 */
GLIB_API int atDA(DynamicArray *const da, size_t index, void **output);

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
GLIB_API int findDA(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr));

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
GLIB_API int findLastDA(DynamicArray *const da, void **const output, int (*selector)(void *itemPtr));

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
GLIB_API int findIndexDA(DynamicArray *const da, int *const output, int (*selector)(void *itemPtr));

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
GLIB_API int findLastIndexDA(DynamicArray *const da, int *const output, int (*selector)(void *itemPtr));

/**
 * Finds the index of a specific item in the DynamicArray, without modifing the array, by directly comparing items to the given value.
 * The search is done from left to right.
 * The search stops as soon as the selector chooses one item.
 * @param da A pointer to the DynamicArray. If NULL, 1 is returned.
 * @param output An int pointer, which will receive the index of the item. If NULL, 1 is returned.
 * @param value A void pointer to the value, against which the items will be compared. If NULL, 1 is returned.
 * @returns 0 on success. -1 if the item was not found.
 */
GLIB_API int indexOfDA(DynamicArray *const da, int *const output, void *const value);

#endif