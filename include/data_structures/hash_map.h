#ifndef HASH_MAP_H
#define HASH_MAP_H

// _WIN32 is a predefined compiler macro when compiling for Windows.
// Must NOT be defined manually! More notes in CMakeLists.txt
#ifdef _WIN32
#ifdef GLIB_EXPORTS
#define GLIB_API __declspec(dllexport)
#else
#define GLIB_API __declspec(dllimport)
#endif
#else
#define GLIB_API
#endif

#ifndef HASH_MAP_INIT_CAPACITY
#define HASH_MAP_INIT_CAPACITY 10
#endif

/**
 * @param key A null-terminated string.
 * @param value A void pointer.
 */
typedef struct Entry
{
    char *key;
    void *value;
} Entry;

/**
 * @param Entry An array of Entry pointers, which is expanded and shrunken based on the need.
 * @param elements The number of elements (Entries) present in the HashMap.
 * @param capacity The number of elements (Entries) the HashMap can contain. This value is not constant.
 */
typedef struct HashMap
{
    struct Entry **entries;
    size_t elements;
    size_t capacity;
} HashMap;

/**
 * Creates a new HashMap. The HashMap starts with an initial capacity of 10.
 * @returns A pointer to the HashMap. If memory could not be allocated, NULL is returned.
 */
GLIB_API HashMap *new_hash_map();

/**
 * Frees the HashMap. The pointer of the keys and values are not freed.
 * @param map A pointer to the HashMap, which must be freed.
 */
GLIB_API void free_hash_map(HashMap *map);

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
GLIB_API int put_hm(char *key, void *value, HashMap *map);

/**
 * Returns the value for the given key in the HashMap.
 * @param key A null-terminated string, which is the key for the given value.
 * @param map A pointer to the HashMap, from which to find the value.
 * @returns The pointer for the given key. If not found, NULL is returned.
 */
GLIB_API void *get_hm(char *key, HashMap *map);

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
GLIB_API int remove_hm(char *key, HashMap *map);

/**
 * Applies a processor function to every Entry in the HashMap.
 * @param processor A function pointer to the function, which will receive a pointer to every currently available Entry in the HashMap.
 * @param map A pointer to the HashMap, the Entries of which must be processed.
 * @returns 0 on success; 1 if either of the arguments are null pointers.
 */
GLIB_API int process_hm(void (*processor)(Entry *const ptr), HashMap *const map);

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
 * The error codes of push_da are the second digit: in 31 the error code of push_da is 1;
 * 
 * 3 if at_da function returned with an error code > 0, while trying to get an Entry pointer element from the DynamicArray temporary storage;
 * 
 * 4 if the Entry pointer element from the DynamicArray temporary storage, returned from at_da, is NULL;
 * 
 * 5 if the selected Entry could not be incerted in the new Entries array due to lacking capacity;
 */
GLIB_API int filter_hm(int (*selector)(Entry *const ptr), HashMap *const map);

#endif