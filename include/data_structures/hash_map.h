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

typedef struct Entry {
    char *key;
    void *value;
} Entry;

typedef struct HashMap {
    struct Entry **entries;
    size_t elements;
    size_t capacity;
} HashMap;

/**
 * Creates a new HashMap.
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
 * @param key A string, which will be the key for the given value.
 * @param value A pointer, which will be added as the value in the HashMap for the given key.
 * @param map A pointer to the HashMap, in which the new entry must be added.
 * @returns 0 on success, 1 on failure. 
 */
GLIB_API int put_hm(char *key, void *value, HashMap *map);

/**
 * Returns the value for the given key in the HashMap.
 * @param key A string, which is the key for the given value.
 * @param map A pointer to the HashMap, from which to find the value.
 * @returns The pointer for the given key. If not found, NULL is returned.
 */
GLIB_API void *get_hm(char *key, HashMap *map);

/**
 * Removes an entry from the HashMap.
 * @param key A string, which is the key of the entry.
 * @param map A pointer to the HashMap, from which the entry must be removed.
 * @returns 0 on success, 1 on failure.
 */
GLIB_API int remove_hm(char *key, HashMap *map);

#endif