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

#ifndef HASH_MAP_CAPACITY
#define HASH_MAP_CAPACITY 5000
#endif

typedef struct Entry {
    char *key;
    void *value;
} Entry;

typedef struct HashMap {
    struct Entry **entries;
    int elements;
    int capacity;
} HashMap;

GLIB_API HashMap *newHashMap();

GLIB_API void freeHashMap(HashMap *map);

GLIB_API int putHM(char *key, void *value, HashMap *map);

GLIB_API void *getHM(char *key, HashMap *map);

GLIB_API int removeHM(char *key, HashMap *map);

#endif