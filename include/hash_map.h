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
    HM_ERR_DYNAMIC_ARRAY = 8,               /**< (8) A DynamicArray function returned a @ref DynamicArrayError error code. */
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
 * Creates a new HashMap.
 * @return A value of the @ref HashMapError:
 * 
 * - HM_SUCCESS
 * 
 * - HM_ERR_NULL_ARGUMENT
 * 
 * - HM_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum HashMapError new_hash_map(void (*value_destructor)(void *value), HashMap **const output);

/**
 * Adds a destructor function to the HashMap. If one is already present, it is replaced with the new one.
 * It is ment to free the memory of the value pointer, contained in the Entry.
 * @param map A pointer to the HashMap, in which the destructor must be added.
 * @param value_destructor A function pointer. Receives as a single argument - a void pointer.
 * @return A value of the @ref HashMapError:
 * 
 * - HM_SUCCESS
 * 
 * - HM_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum HashMapError add_destructor_hm(HashMap *const map, void (*value_destructor)(void *value));

/**
 * Frees the memory of the HashMap. The pointer of the keys and values are not freed. 
 * If the HashMap containes a value_destructor, the value_destructor is invoked with every present value from each Entry.
 * @param map A pointer to the HashMap, which must be freed.
 * @return A value of the @ref HashMapError:
 * 
 * - HM_SUCCESS
 * 
 * - HM_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum HashMapError free_hash_map(HashMap *map);

/**
 * Adds a new Entry (key-value pair) to the HashMap. 
 * If an Entry with the same key already exists, it is replaced with the new one.
 * @param map A pointer to the HashMap, in which the new Entry must be added.
 * @param key A null-terminated string, which is copied to create the key for the given value.
 * @param value A pointer, which will be added as the value in the HashMap for the given key.
 * @return A value of the @ref HashMapError:
 * 
 * - HM_SUCCESS
 * 
 * - HM_ERR_NULL_ARGUMENT
 * 
 * - HM_ERR_FULL
 * 
 * - HM_ERR_KEY_EMPTY
 * 
 * - HM_ERR_KEY_MAX_LENGTH
 * 
 * - HM_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum HashMapError put_hm(HashMap *const map, char *const key, void *const value);

/**
 * Gets the value for the given key from the HashMap.
 * @param map A pointer to the HashMap, from which to find the value.
 * @param key A null-terminated string, which is the key for the given value.
 * @param output A pointer, where the value will be placed.
 * @return A value of the @ref HashMapError:
 * 
 * - HM_SUCCESS
 * 
 * - HM_EMPTY
 * 
 * - HM_NOT_FOUND
 * 
 * - HM_ERR_NULL_ARGUMENT
 * 
 * - HM_ERR_KEY_EMPTY
 * 
 * - HM_ERR_KEY_MAX_LENGTH
 */
GALXLIB_API enum HashMapError get_hm(const HashMap *const map, const char *const key, void **const output);

/**
 * Removes an Entry from the HashMap.
 * @param map A pointer to the HashMap, from which the Entry must be removed.
 * @param key A null-terminated string, which is the key of the Entry.
 * @return A value of the @ref HashMapError:
 * 
 * - HM_SUCCESS
 * 
 * - HM_EMPTY
 * 
 * - HM_NOT_FOUND
 * 
 * - HM_ERR_NULL_ARGUMENT
 * 
 * - HM_ERR_KEY_EMPTY
 * 
 * - HM_ERR_KEY_MAX_LENGTH
 * 
 * - HM_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum HashMapError remove_hm(HashMap *const map, const char *const key);

/**
 * Applies a processor function to every Entry in the HashMap.
 * @param map A pointer to the HashMap, the Entries of which must be processed.
 * @param processor A function pointer to the function, which will receive a pointer to every currently available Entry in the HashMap.
 * @return A value of the @ref HashMapError:
 * 
 * - HM_SUCCESS
 * 
 * - HM_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum HashMapError process_hm(const HashMap *const map, void (*processor)(const Entry *const ptr));

/**
 * Filters the Entries of the HashMap, leaving only the selected ones.
 * @param map A pointer to the HashMap, which must be filtered.
 * @param selector A function pointer to the function, which will receive a pointer to every currently available Entry in the HashMap.
 * The selector returns 1 if the Entry must STAY, 0 if it must be REMOVED.
 * @return A value of the @ref HashMapError:
 * 
 * - HM_SUCCESS
 * 
 * - HM_ERR_DYNAMIC_ARRAY
 * 
 * - HM_ERR_NULL_ARGUMENT
 * 
 * - HM_ERR_MEMORY_ALLOCATION
 * 
 * - HM_ERR_FULL
 * 
 * - HM_ERR_KEY_EMPTY
 * 
 * - HM_ERR_KEY_MAX_LENGTH
 */
GALXLIB_API enum HashMapError filter_hm(HashMap *const map, int (*selector)(const Entry *const ptr));

#endif