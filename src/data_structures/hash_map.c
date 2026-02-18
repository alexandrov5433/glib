#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/data_structures/hash_map.h"

// ##################   static   ##################

static Entry *_new_entry(char *key, void *value)
{
    Entry *entry = (Entry *)malloc(sizeof(Entry));
    if (entry == NULL)
        return NULL;

    // +1 for the null-terminator '\n'
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

static int _extend_hm(HashMap *map)
{
    int emptySpace = map->capacity - map->elements;
    if (emptySpace > (map->capacity * 0.25))
        return 0;

    size_t newCapacity = ceil(map->elements * 1.5);
    Entry **newEntries = realloc(map->entries, (sizeof(Entry *) * newCapacity));
    if (newEntries == NULL)
        return 1;

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

    for (size_t i = 0; i < map->capacity; i++)
    {
    _top_of_loop:
        Entry *e = (map->entries)[i];
        if (e == NULL)
            continue;

        size_t newIndex = _hash_str(e->key, newCapacity);
        for (size_t j = newIndex; j < newCapacity; j++)
        {
            if (newEntries[j] == NULL)
            {
                newEntries[j] = e;
                i++;
                goto _top_of_loop;
            }
        }
        for (size_t j = 0; j < newIndex; j++)
        {
            if (newEntries[j] == NULL)
            {
                newEntries[j] = e;
                i++;
                goto _top_of_loop;
            }
        }
    }
    free(map->entries);
    map->entries = newEntries;
    map->capacity = newCapacity;
    return 0;
}

// ##################   public   ##################

HashMap *new_hash_map()
{
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    if (map == NULL)
        return NULL;

    Entry **entries = calloc(HASH_MAP_INIT_CAPACITY, sizeof(Entry *));
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
    if (_extend_hm(map))
        return 2;

    size_t index = _hash_str(key, map->capacity);
    for (size_t i = index; i < map->capacity; i++)
    {
        if ((map->entries)[i] == NULL)
        {
            (map->entries)[i] = _new_entry(key, value);
            (map->elements)++;
            return 0;
        }
    }
    for (size_t i = 0; i < index; i++)
    {
        if ((map->entries)[i] == NULL)
        {
            (map->entries)[i] = _new_entry(key, value);
            (map->elements)++;
            return 0;
        }
    }
    return 1;
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
    if (_squish_hm(map))
        return 2;

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