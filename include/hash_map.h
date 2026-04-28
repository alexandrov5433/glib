#ifndef GALXLIB_HASH_MAP_H
#define GALXLIB_HASH_MAP_H

#include <stddef.h>

// _WIN32 is a predefined compiler macro when compiling for Windows.
// Must NOT be defined manually! More notes in CMakeLists.txt
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

#ifndef HASH_MAP_INIT_CAPACITY
#define HASH_MAP_INIT_CAPACITY 10
#endif

#ifndef HASH_MAP_KEY_MAX_LENGTH
#define HASH_MAP_KEY_MAX_LENGTH 2000000000UL
#endif

/**
 * @enum HashMapError
 * @brief The error codes returned by the HashMap functions.
 */
enum HashMapError
{
    HM_EMPTY = -2,                          /**< (-2) The HashMap does not include any entries. */
    HM_NOT_FOUND = -1,                      /**< (-1) An entry with the given key was not found among those in the HashMap. */
    HM_SUCCESS = 0,                         /**< (0) Successful execution of the called function. */
    HM_ERR_NULL_ARGUMENT = 1,               /**< (1) One or more arguments are NULL. */
    HM_ERR_MEMORY_ALLOCATION = 2,           /**< (2) Failed to allocate or reallocate memory. */
    HM_ERR_KEY_MAX_LENGTH = 3,              /**< (3) The legnth of the given key exceeds the HASH_MAP_KEY_MAX_LENGTH limit. */
    HM_ERR_KEY_EMPTY = 4,                   /**< (4) The legnth of the given key is 0. */
    HM_ERR_KEY_COPY = 5,                    /**< (5) An error occured while coping the key using strcpy_s. */
    HM_ERR_FULL = 6,                        /**< (6) The HashMap is full. */
    HM_ERR_INVALID_ARGUMENT_DIMENTIONS = 7, /**< (7) The dimentions of one or more arguments, either alone or in their combination, do not match the expectations of the function. */
};

/**
 * @struct Entry
 * @brief A structure representing a key-value pair in the HashMap.
 */
typedef struct Entry
{
    char *key;   /**< A null-terminated character array. */
    void *value; /**< A pointer. */
} Entry;

/**
 * @struct HashMap
 * @brief The structure containing all of the data for the HashMap.
 */
typedef struct HashMap
{
    struct Entry **entries;                /**< An array of Entry pointers, which is expanded and shrunken based on the need. */
    size_t n_ent;                          /**< The number of Entries present in the HashMap. This value is not constant. */
    size_t capacity;                       /**< The number of Entries the HashMap can contain. This value is not constant. */
    void (*value_destructor)(void *value); /**< A pointer to a function, implemented by the user. It is ment to free the memory of the value when it is removed from the HashMap. Defaults to NULL. */
} HashMap;

/**
 * Creates a new HashMap. The HashMap starts with an initial capacity of 10.
 * @returns A pointer to the HashMap. If memory could not be allocated, NULL is returned.
 */
GALXLIB_API HashMap *new_hash_map();

/**
 * Adds a destructor function to the HashMap. If one is already present, the old one is replaced with the new one.
 * The destructor receives every available Entry upon calling free_hash_map and is ment to free the memory of the value pointer, given by the user.
 * @param value_destructor A function pointer. Receives as a single argument an Entry pointer.
 * This function must NOT manipulate the Entry it self or its properties.
 * It sould simply free the memory pointer to by the value property pointer. The key preperty may be red, but not changed.
 * @param map A pointer to the HashMap, in which the destructor must be added.
 * @returns 0 on success. 1 on failure, if either of the arguments are NULL.
 */
GALXLIB_API int add_destructor_hm(void (*value_destructor)(const Entry *const entry), HashMap *map);

/**
 * Frees the HashMap. The pointer of the keys and values are not freed.
 * @param map A pointer to the HashMap, which must be freed.
 */
GALXLIB_API void free_hash_map(HashMap *map);

/**
 * Adds a new entry (key-value pair) to the HashMap.
 * @param key A null-terminated string, which is copied to create the key for the given value.
 * @param value A pointer, which will be added as the value in the HashMap for the given key.
 * @param map A pointer to the HashMap, in which the new entry must be added.
 * @returns 0 on success;
 * 1 on failure due to lacking capacity;
 *
 * 2 on failure to expand the HashMap because of memory allocation failure;
 *
 * 22 on failure to due to lack of free space after extending it;
 *
 * 3 on failure to create a new Entry from the given key-value pair;
 */
GALXLIB_API int put_hm(char *key, void *value, HashMap *map);

/**
 * Returns the value for the given key in the HashMap.
 * @param key A null-terminated string, which is the key for the given value.
 * @param map A pointer to the HashMap, from which to find the value.
 * @returns The pointer for the given key. If not found, NULL is returned.
 */
GALXLIB_API void *get_hm(char *key, HashMap *map);

/**
 * Removes an entry from the HashMap.
 * @param key A null-terminated string, which is the key of the entry.
 * @param map A pointer to the HashMap, from which the entry must be removed.
 * @returns 0 on success;
 * 1 on failure, because an entry with the given key was not found;
 *
 * 2 on failure due to memory allocation failure.
 *
 * 22 on failure due to lack of free space after shrinking it;
 */
GALXLIB_API int remove_hm(char *key, HashMap *map);

/**
 * Applies a processor function to every Entry in the HashMap.
 * @param processor A function pointer to the function, which will receive a pointer to every currently available Entry in the HashMap.
 * @param map A pointer to the HashMap, the Entries of which must be processed.
 * @returns 0 on success; 1 if either of the arguments are null pointers.
 */
GALXLIB_API int process_hm(void (*processor)(Entry *const ptr), HashMap *const map);

/**
 * Filters the Entries of the HashMap, leaving only the selected ones.
 * @param selector A function pointer to the function, which will receive a pointer to every currently available Entry in the HashMap.
 * The selector returns 1 if the Entry must STAY, 0 if it must be REMOVED.
 * @param map A pointer to the HashMap, which must be filtered.
 * @returns 0 on success;
 * 1 if either of the arguments are null pointers;
 *
 * 2 if memory could not be allocated for temporary storage;
 *
 * 30-something if the Entry pointer could not be placed in the DynamicArray for temporary storage by the push_da function.
 * The error codes of push_da are the second digit: in 31 the error code of push_da is 1.
 * The error code is created by adding the error code from push_da to 30.
 *
 * 3 if at_da function returned with an error code > 0, while trying to get an Entry pointer element from the DynamicArray temporary storage;
 *
 * 4 if the Entry pointer element from the DynamicArray temporary storage, returned from at_da, is NULL;
 *
 * 5 if the selected Entry could not be incerted in the new Entries array due to lacking capacity;
 */
GALXLIB_API int filter_hm(int (*selector)(Entry *const ptr), HashMap *const map);

#endif