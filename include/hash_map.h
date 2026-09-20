/**
 * @file hash_map.h
 * @brief Header file for hash map implementation.
 */

#ifndef GALXLIB_HASH_MAP_H
#define GALXLIB_HASH_MAP_H

#include <stddef.h>
#include "./error.h"

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

#else
#define GALXLIB_API

#endif

#ifndef HASH_MAP_INIT_CAPACITY
#define HASH_MAP_INIT_CAPACITY 100UL
#endif

#ifndef HASH_MAP_MAX_CAPACITY
#define HASH_MAP_MAX_CAPACITY __SIZE_MAX__
#endif

#ifndef HASH_MAP_KEY_MAX_LENGTH
#define HASH_MAP_KEY_MAX_LENGTH 2000000000UL
#endif

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
	struct Entry **entries;			/**< An array of Entry pointers, which is expanded and shrunken based on the need. */
	size_t n_ent;				/**< The number of Entries present in the HashMap. This value is not constant. */
	size_t capacity;			/**< The number of Entries the HashMap can contain. This value is not constant. */
	void (*value_destructor)(void **value); /**< A function, implemented by the user. It is ment to free the memory of the Entry::value when it is removed from the HashMap. Defaults to NULL. */
} HashMap;

/**
 * Creates a new @ref HashMap.
 * @param output A pointer where the the new HashMap will be outputed.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum GalxlibError new_hash_map(HashMap **const output);

/**
 * Creates a new @ref HashMap.
 * @param value_destructor A function, which will be placed in the HashMap as the value_destructor.
 * @param output A pointer where the the new HashMap will be outputed.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum GalxlibError new_hash_map_d(void (*value_destructor)(void **value), HashMap **const output);

/**
 * Adds a destructor function to the HashMap. If one is already present, it is replaced with the new one.
 * It is ment to free the memory of the value pointer, contained in the Entry.
 * @param map A pointer to the HashMap, in which the destructor must be added.
 * @param value_destructor A function pointer. Receives as a single argument - a void pointer.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum GalxlibError add_destructor_hm(HashMap *const map, void (*value_destructor)(void **value));

/**
 * Frees the memory of the HashMap. The values of the Entries are not freed.
 * @param map A pointer to the HashMap, which must be freed.
 * If it points to NULL, nothing is done and GLX_SUCCESS is returned.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum GalxlibError free_hash_map(HashMap **map);

/**
 * Frees the memory of the HashMap. 
 * The HashMap::value_destructor is invoked with each Entry::value.
 * @param map A pointer to the HashMap, which must be freed.
 * If it points to NULL, nothing is done and GLX_SUCCESS is returned.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 * 
 * - GLX_ERR_NULL_DESTRUCTOR
 */
GALXLIB_API enum GalxlibError free_hash_map_d(HashMap **map);

/**
 * Adds a new Entry (key-value pair) to the HashMap.
 * If an (old) Entry with the same key already exists, it is replaced with the new one.
 * If the HashMap has a value_destructor, it is invoked with the value of the removed (old) Entry.
 * @param map A pointer to the HashMap, in which the new Entry must be added.
 * @param key A null-terminated string, which is copied to create the key for the given value.
 * @param value A pointer, which will be added as the value in the HashMap for the given key.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_FULL
 *
 * - GLX_ERR_KEY_EMPTY
 *
 * - GLX_ERR_KEY_MAX_LENGTH
 *
 * - GLX_ERR_MEMORY_ALLOCATION
 * 
 * - GLX_ERR_MAX_CAPACITY
 */
GALXLIB_API enum GalxlibError put_hm(HashMap *const map, char *const key, void *const value);

/**
 * Gets the value for the given key from the HashMap.
 * @param map A pointer to the HashMap, from which to find the value.
 * @param key A null-terminated string, which is the key for the given value.
 * @param output A pointer, where the value will be placed.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_ZERO_LENGTH
 *
 * - GLX_ERR_NO_MATCH
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_KEY_EMPTY
 *
 * - GLX_ERR_KEY_MAX_LENGTH
 */
GALXLIB_API enum GalxlibError get_hm(const HashMap *const map, const char *const key, void **const output);

/**
 * Removes an Entry from the HashMap.
 * If the HashMap has a value_destructor, it is invoked with the value of the Entry.
 * @param map A pointer to the HashMap, from which the Entry must be removed.
 * @param key A null-terminated string, which is the key of the Entry.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_ZERO_LENGTH
 *
 * - GLX_ERR_NO_MATCH
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_KEY_EMPTY
 *
 * - GLX_ERR_KEY_MAX_LENGTH
 *
 * - GLX_ERR_MEMORY_ALLOCATION
 */
GALXLIB_API enum GalxlibError remove_hm(HashMap *const map, const char *const key);

/**
 * Applies a processor function to every Entry in the HashMap.
 * @param map A pointer to the HashMap, which must be processed.
 * @param processor A function, which will receive a pointer to every available Entry in the HashMap.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum GalxlibError process_e_hm(const HashMap *const map, void (*processor)(const Entry *const ptr));

/**
 * Applies a processor function to value in the HashMap.
 * @param map A pointer to the HashMap, which must be processed.
 * @param processor A function, which will receive every value (a pointer) in the HashMap.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 */
GALXLIB_API enum GalxlibError process_v_hm(const HashMap *const map, void (*processor)(const void *const ptr));

/**
 * Filters the Entries of the HashMap, leaving only the selected ones.
 * If the HashMap has a value_destructor, it is invoked with the value of the removed Entries.
 * @param map A pointer to the HashMap, which must be filtered.
 * @param selector A function, which will receive a pointer to every currently available Entry in the HashMap.
 * The selector returns 1 if the Entry must STAY, 0 if it must be REMOVED.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_MEMORY_ALLOCATION
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_MEMORY_ALLOCATION
 *
 * - GLX_ERR_FULL
 *
 * - GLX_ERR_KEY_EMPTY
 *
 * - GLX_ERR_KEY_MAX_LENGTH
 */
GALXLIB_API enum GalxlibError filter_hm(HashMap *const map, int (*selector)(const Entry *const ptr));

#endif