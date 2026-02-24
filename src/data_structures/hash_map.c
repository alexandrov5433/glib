#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/data_structures/hash_map.h"
#include "../include/data_structures/dynamic_array.h"

// ##################   static   ##################

static Entry *_new_entry(char *key, void *value)
{
    Entry *entry = (Entry *)malloc(sizeof(Entry));
    if (entry == NULL)
        return NULL;

    // +1 for the null-terminator '\0'
    size_t keyLengthWithNullterminator = strlen(key) + 1;
    char *keyCopy = calloc(keyLengthWithNullterminator, sizeof(char));
    strcpy(keyCopy, key);
    entry->key = keyCopy;
    entry->value = value;
    return entry;
}

static void _free_entry(Entry *entry)
{
    if (entry == NULL)
        return;

    free(entry->key);
    free(entry);
}

static size_t _hash_str(char *str, size_t capacity)
{
    size_t length = strlen(str);
    int asciiValue = 0;
    for (int i = 0; i < length; i++)
        asciiValue += (int)(str[i]);

    return (size_t)((asciiValue * length) % capacity);
}

static int _incert_entry(Entry *newEntry, size_t capacity, Entry **entries)
{
    size_t index = _hash_str(newEntry->key, capacity);
    for (size_t i = index; i < capacity; i++)
    {
        if (entries[i] == NULL)
        {
            entries[i] = newEntry;
            return 0;
        }
    }
    for (size_t i = 0; i < index; i++)
    {
        if (entries[i] == NULL)
        {
            entries[i] = newEntry;
            return 0;
        }
    }
    return 1;
}

static int _transfer(Entry **source, Entry **destination, size_t sourceCapacity, size_t destinationCapacity)
{
    for (size_t h = 0; h < sourceCapacity; ++h)
    {
        Entry *ent = source[h];
        if (ent == NULL)
            goto _continue_main_loop;

        size_t index = _hash_str(ent->key, destinationCapacity);
        for (size_t i = index; i < destinationCapacity; ++i)
        {
            if (destination[i] == NULL)
            {
                destination[i] = ent;
                goto _continue_main_loop;
            }
        }
        for (size_t i = 0; i < index; ++i)
        {
            if (destination[i] == NULL)
            {
                destination[i] = ent;
                goto _continue_main_loop;
            }
        }
        return 1;
    _continue_main_loop:;
    }
    return 0;
}

static int _extend_hm(HashMap *map)
{
    int emptySpace = map->capacity - map->elements;
    if (emptySpace > (map->capacity * 0.25))
        return 0;

    size_t newCapacity = ceil(map->elements * 1.5);
    Entry **newEntries = calloc(newCapacity, sizeof(Entry *));
    if (newEntries == NULL)
        return 1;

    if (_transfer(map->entries, newEntries, map->capacity, newCapacity))
    {
        free(newEntries);
        return 2;
    }

    free(map->entries);
    map->entries = newEntries;
    map->capacity = newCapacity;
    return 0;
}

static int _squish_hm(HashMap *map)
{
    int emptySpace = map->capacity - map->elements;
    // reduce capacity if more than 90% are free
    if (emptySpace < (map->capacity * 0.9))
        return 0;

    size_t newCapacity = ceil(map->elements * 1.5);
    Entry **newEntries = calloc(newCapacity, sizeof(Entry *));
    if (newEntries == NULL)
        return 1;

    if (_transfer(map->entries, newEntries, map->capacity, newCapacity))
    {
        free(newEntries);
        return 2;
    }

    free(map->entries);
    map->entries = newEntries;
    map->capacity = newCapacity;
    return 0;
}

/**
 * @param initCapacity Initial count of places for Entry pointers.
 */
static Entry **_new_entries_array(size_t initCapacity)
{
    return calloc(initCapacity, sizeof(Entry *));
}

// ##################   public   ##################

HashMap *new_hash_map()
{
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    if (map == NULL)
        return NULL;

    Entry **entries = _new_entries_array(HASH_MAP_INIT_CAPACITY);
    if (entries == NULL)
        return NULL;

    map->entries = entries;
    map->elements = 0;
    map->capacity = HASH_MAP_INIT_CAPACITY;
    return map;
}

void free_hash_map(HashMap *map)
{
    if (map == NULL)
    {
        return;
    }
    for (int i = 0; i < map->capacity; i++)
    {
        Entry *e = (map->entries)[i];
        if (e == NULL)
            continue;
        _free_entry(e);
    }
    free(map->entries);
    free(map);
}

int put_hm(char *key, void *value, HashMap *map)
{
    switch (_extend_hm(map))
    {
    case 1:
        return 2;
    case 2:
        return 22;
    }

    Entry *newEntry = _new_entry(key, value);
    if (newEntry == NULL)
        return 3;

    if (_incert_entry(newEntry, map->capacity, map->entries))
        return 1;

    (map->elements)++;

    return 0;
}

void *get_hm(char *key, HashMap *map)
{
    size_t index = _hash_str(key, map->capacity);
    for (size_t i = index; i < map->capacity; i++)
    {
        Entry *ent = (map->entries)[i];
        if (ent == NULL)
            continue;
        if (strcmp(key, ent->key) == 0)
            return ent->value;
    }
    for (size_t i = 0; i < index; i++)
    {
        Entry *ent = (map->entries)[i];
        if (ent == NULL)
            continue;
        if (strcmp(key, ent->key) == 0)
            return ent->value;
    }
    return NULL;
}

int remove_hm(char *key, HashMap *map)
{
    switch (_squish_hm(map))
    {
    case 1:
        return 2;
    case 2:
        return 22;
    }

    size_t index = _hash_str(key, map->capacity);
    for (size_t i = index; i < map->capacity; i++)
    {
        Entry *ent = (map->entries)[i];
        if (ent == NULL)
            continue;

        if (strcmp(key, ent->key) == 0)
        {
            _free_entry(ent);
            (map->entries)[i] = NULL;
            (map->elements)--;
            return 0;
        }
    }
    for (size_t i = 0; i < index; i++)
    {
        Entry *ent = (map->entries)[i];
        if (ent == NULL)
            continue;

        if (strcmp(key, ent->key) == 0)
        {
            _free_entry(ent);
            (map->entries)[i] = NULL;
            (map->elements)--;
            return 0;
        }
    }
    return 1;
}

int process_hm(void (*processor)(Entry *const ptr), HashMap *const map)
{
    if (map == NULL || processor == NULL)
        return 1;

    for (size_t i = 0; i < map->capacity; ++i)
    {
        Entry *const e = map->entries[i];
        if (e != NULL)
            processor(e);
    }

    return 0;
}

int filter_hm(int (*selector)(Entry *const ptr), HashMap *const map)
{
    if (map == NULL || selector == NULL)
        return 1;

    DynamicArray *entryBucket = new_dynamic_array(VOID_PTR);
    if (entryBucket == NULL)
        return 2;

    // TODO: free memory of removed Entries???
    for (size_t i = 0; i < map->capacity; ++i)
    {
        Entry *const e = map->entries[i];
        if (e == NULL)
            continue;

        if (selector(e))
        {
            int pushErr = push_da(entryBucket, e);
            if (pushErr > 0)
            {
                free_dynamic_array(entryBucket);
                return 30 + pushErr;
            }
        }
    }

    size_t newEntriesCapacity = entryBucket->count + HASH_MAP_INIT_CAPACITY;
    Entry **newEntries = _new_entries_array(newEntriesCapacity);
    if (newEntries == NULL)
    {
        free_dynamic_array(entryBucket);
        return 2;
    }

    // Case: all Entries must be removed from the HashMap.
    if (entryBucket->count == 0)
        goto _hashmap_manipulation_stage;

    // Case: some Entries must be removed.
    for (size_t i = 0; i < entryBucket->count; ++i)
    {
        Entry *currentEntry = NULL;

        int atErr = at_da(entryBucket, i, (void **)&currentEntry);
        if (atErr)
        {
            free_dynamic_array(entryBucket);
            return 3;
        }

        if (currentEntry == NULL)
        {
            free_dynamic_array(entryBucket);
            return 4;
        }

        int incertErr = _incert_entry(currentEntry, newEntriesCapacity, newEntries);
        if (incertErr)
        {
            free_dynamic_array(entryBucket);
            return 5;
        }
    }

_hashmap_manipulation_stage:
    free(map->entries);
    map->entries = newEntries;
    map->capacity = newEntriesCapacity;
    map->elements = entryBucket->count;

    free_dynamic_array(entryBucket);

    return 0;
}
