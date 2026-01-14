#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/data_structures/hashMap.h"

HashMap *newHashMap()
{
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    if (map == NULL)
    {
        return NULL;
    }
    Entry **entries = calloc(HASH_MAP_CAPACITY, sizeof(Entry *));
    if (entries == NULL)
    {
        return NULL;
    }
    map->entries = entries;
    map->elements = 0;
    map->capacity = HASH_MAP_CAPACITY;
    return map;
}

static Entry *newEntry(char *key, void *value)
{
    Entry *entry = (Entry *)malloc(sizeof(Entry));
    if (entry == NULL)
    {
        return NULL;
    }
    entry->key = key;
    entry->value = value;
    return entry;
}

static void freeEntry(Entry *entry)
{
    if (entry == NULL)
    {
        return;
    }
    free(entry);
}

void freeHashMap(HashMap *map) {
    if (map == NULL) {
        return;
    }
    for (int i = 0; i < map->capacity; i++) {
        Entry *e = (map->entries)[i];
        if (e == NULL)
            continue;
        freeEntry(e);
    }
    free(map->entries);
    free(map);
}

static int hashStr(char *str)
{
    size_t length = strlen(str);
    int asciiValue = 0;
    for (int i = 0; i < length; i++)
    {
        asciiValue += (int)(str[i]);
    }
    return (int)((asciiValue * length) % HASH_MAP_CAPACITY);
}

static void extendHM(HashMap *map)
{
    int emptySpace = map->capacity - map->elements;
    if (emptySpace > (map->capacity * 0.5)) {
        return;
    }
    int newCapacity = map->capacity * 1.5;
    newCapacity = newCapacity < HASH_MAP_CAPACITY ? HASH_MAP_CAPACITY : newCapacity;
    Entry **newEntries = realloc(map->entries, (sizeof(Entry *) * newCapacity));
    if (newEntries == NULL)
    {
        puts("Error: Memory reallocation failed while extending glib::HashMap.");
        return;
    }
    map->entries = newEntries;
    map->capacity = newCapacity;
}

static void squishHM(HashMap *map)
{
    int emptySpace = map->capacity - map->elements;
    if (emptySpace < (map->capacity * 0.85))
    {
        return;
    }
    int newCapacity = map->capacity * 0.5 + map->elements;
    newCapacity = newCapacity < HASH_MAP_CAPACITY ? HASH_MAP_CAPACITY : newCapacity;
    Entry **newEntries = calloc(newCapacity, sizeof(Entry *));
    if (newEntries == NULL)
    {
        return;
    }
    for (int i = 0; i < map->capacity; i++)
    {
        Entry *e = (map->entries)[i];
        if (e == NULL)
            continue;
        int newIndex = hashStr(e->key);
        newEntries[newIndex] = e;
    }
    free(map->entries);
    map->entries = newEntries;
    map->capacity = newCapacity;
}

int putHM(char *key, void *value, HashMap *map)
{
    extendHM(map);
    int index = hashStr(key);
    for (int i = index; i < map->capacity; i++)
    {
        if ((map->entries)[i] == NULL)
        {
            (map->entries)[i] = newEntry(key, value);
            (map->elements)++;
            return 0;
        }
    }
    for (int i = 0; i < index; i++)
    {
        if ((map->entries)[i] == NULL)
        {
            (map->entries)[i] = newEntry(key, value);
            (map->elements)++;
            return 0;
        }
    }
    return 1;
}

void *getHM(char *key, HashMap *map)
{
    int index = hashStr(key);
    for (int i = index; i < map->capacity; i++)
    {
        Entry *ent = (map->entries)[i];
        if (ent == NULL)
        {
            return NULL;
        }
        if (strcmp(key, ent->key) == 0)
        {
            return ent->value;
        }
    }
    for (int i = 0; i < index; i++)
    {
        Entry *ent = (map->entries)[i];
        if (ent == NULL)
        {
            return NULL;
        }
        if (strcmp(key, ent->key) == 0)
        {
            return ent->value;
        }
    }
    return NULL;
}

int removeHM(char *key, HashMap *map)
{
    squishHM(map);
    int index = hashStr(key);
    for (int i = index; i < map->capacity; i++)
    {
        Entry *ent = (map->entries)[i];
        if (ent == NULL)
        {
            continue;
        }
        if (strcmp(key, ent->key) == 0)
        {

            freeEntry(ent);
            (map->entries)[i] = NULL;
            (map->elements)--;
            return 0;
        }
    }
    for (int i = 0; i < index; i++)
    {
        Entry *ent = (map->entries)[i];
        if (ent == NULL)
        {
            continue;
        }
        if (strcmp(key, ent->key) == 0)
        {
            
            freeEntry(ent);
            (map->entries)[i] = NULL;
            (map->elements)--;
            return 0;
        }
    }
    return 1;
}