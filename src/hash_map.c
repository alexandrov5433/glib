#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/hash_map.h"
#include "../include/dynamic_array.h"

// ##################   static   ##################

static enum HashMapError _new_entry(const char *key, void *value, Entry **const output)
{
	if (key == NULL || value == NULL || output == NULL)
		return HM_ERR_NULL_ARGUMENT;

	Entry *entry = (Entry *)malloc(sizeof(Entry));
	if (entry == NULL)
		return HM_ERR_MEMORY_ALLOCATION;

	size_t key_length = strlen_s(key, HASH_MAP_KEY_MAX_LENGTH);
	if (key_length == 0)
		return HM_ERR_KEY_EMPTY;
	else if (key_length == HASH_MAP_KEY_MAX_LENGTH)
		return HM_ERR_KEY_MAX_LENGTH;

	// +1 for the null-terminator
	char *key_copy = (char *)malloc((key_length + 1) * sizeof(char));
	if (key_copy == NULL)
		return HM_ERR_MEMORY_ALLOCATION;

	if (strcpy_s(key_copy, key_length, key))
	{
		free(entry);
		return HM_ERR_KEY_COPY;
	}

	entry->key = key_copy;
	entry->value = value;
	*output = entry;

	return HM_SUCCESS;
}

static enum HashMapError _free_entry(Entry *entry)
{
	if (entry == NULL)
		return HM_ERR_NULL_ARGUMENT;
	if (entry->key != NULL)
		free(entry->key);

	free(entry);
	return HM_SUCCESS;
}

static enum HashMapError _hash_str(const char *str, const size_t hm_capacity, size_t *const output)
{
	if (str == NULL || output == NULL)
		return HM_ERR_NULL_ARGUMENT;

	size_t str_length = strlen_s(str, HASH_MAP_KEY_MAX_LENGTH);
	if (str_length == 0)
		return HM_ERR_KEY_EMPTY;
	else if (str_length == HASH_MAP_KEY_MAX_LENGTH)
		return HM_ERR_KEY_MAX_LENGTH;

	size_t ascii_value = 0;
	for (size_t i = 0; i < str_length; ++i)
		ascii_value += (size_t)(str[i]);

	*output = (size_t)((ascii_value * str_length) % hm_capacity);

	return HM_SUCCESS;
}

static enum HashMapError _incert_entry(const Entry *const new_entry, const size_t hm_capacity, Entry **const hm_entries)
{
	if (new_entry == NULL || hm_entries == NULL)
		return HM_ERR_NULL_ARGUMENT;

	size_t index = 0;
	int err_hash = _hash_str(new_entry->key, hm_capacity, &index);
	if (err_hash)
		return err_hash;

	for (size_t i = index; i < hm_capacity; ++i)
	{
		if (hm_entries[i] == NULL)
		{
			hm_entries[i] = new_entry;
			return HM_SUCCESS;
		}
	}
	for (size_t i = 0; i < index; ++i)
	{
		if (hm_entries[i] == NULL)
		{
			hm_entries[i] = new_entry;
			return HM_SUCCESS;
		}
	}

	return HM_ERR_FULL;
}

static enum HashMapError _incert_replace_entry(
    const Entry *const new_entry,
    const size_t hm_capacity,
    Entry **const hm_entries,
    int *const output_is_replaced,
    void (*value_destructor)(const Entry *const entry))
{
	// value_destructor CAN be NULL!
	if (new_entry == NULL || hm_entries == NULL || output_is_replaced == NULL)
		return HM_ERR_NULL_ARGUMENT;

	size_t index = 0;
	int err_hash = _hash_str(new_entry->key, hm_capacity, &index);
	if (err_hash)
		return err_hash;

	for (size_t i = index; i < hm_capacity; ++i)
	{
		Entry *current_entry = hm_entries[i];
		if (current_entry == NULL)
		{
			hm_entries[i] = new_entry;
			*output_is_replaced = 0;
			return HM_SUCCESS;
		}
		else if (strcmp(current_entry->key, new_entry->key) == 0)
		{
			if (value_destructor != NULL)
				value_destructor(current_entry);

			hm_entries[i] = new_entry;
			*output_is_replaced = 1;
			return HM_SUCCESS;
		}
	}
	for (size_t i = 0; i < index; ++i)
	{
		Entry *current_entry = hm_entries[i];
		if (current_entry == NULL)
		{
			hm_entries[i] = new_entry;
			*output_is_replaced = 0;
			return HM_SUCCESS;
		}
		else if (strcmp(current_entry->key, new_entry->key) == 0)
		{
			if (value_destructor != NULL)
				value_destructor(current_entry);

			hm_entries[i] = new_entry;
			*output_is_replaced = 1;
			return HM_SUCCESS;
		}
	}

	return HM_ERR_FULL;
}

static enum HashMapError _transfer_entries(
    Entry **const source,
    Entry **const destination,
    const size_t source_capacity,
    const size_t destination_capacity)
{
	if (source == NULL || destination == NULL)
		return HM_ERR_NULL_ARGUMENT;

	for (size_t i = 0; i < source_capacity; ++i)
	{
		Entry *entry = source[i];
		if (entry == NULL)
			continue;

		int err_incert = _incert_entry(entry, destination_capacity, destination);
		if (err_incert)
			return err_incert;
	}

	return HM_SUCCESS;
}

static enum HashMapError _extend_hm(HashMap *const map)
{
	int empty_space = map->capacity - map->n_ent;
	if (empty_space > (map->capacity * 0.25))
		return HM_SUCCESS;

	size_t new_capacity = ceil(map->n_ent * 1.5);
	Entry **new_entries = (Entry **)malloc(new_capacity * sizeof(Entry *));
	if (new_entries == NULL)
		return HM_ERR_MEMORY_ALLOCATION;

	int err_trasfer = _transfer_entries(map->entries, new_entries, map->capacity, new_capacity);
	if (err_trasfer)
	{
		free(new_entries);
		return err_trasfer;
	}

	free(map->entries);
	map->entries = new_entries;
	map->capacity = new_capacity;

	return HM_SUCCESS;
}

static enum HashMapError _squish_hm(HashMap *const map)
{
	int empty_space = map->capacity - map->n_ent;
	// reduce capacity if more than 90% are free
	if (empty_space < (map->capacity * 0.9))
		return HM_SUCCESS;

	size_t new_capacity = ceil(map->n_ent * 1.5);
	Entry **new_entries = (Entry **)malloc(new_capacity * sizeof(Entry *));
	if (new_entries == NULL)
		return HM_ERR_MEMORY_ALLOCATION;

	int err_trasfer = _transfer_entries(map->entries, new_entries, map->capacity, new_capacity);
	if (err_trasfer)
	{
		free(new_entries);
		return err_trasfer;
	}

	free(map->entries);
	map->entries = new_entries;
	map->capacity = new_capacity;

	return HM_SUCCESS;
}

static enum HashMapError _new_entries_array(const size_t init_capacity, Entry ***const output)
{
	if (output == NULL)
		return HM_ERR_NULL_ARGUMENT;

	Entry **new_entries = (Entry **)malloc(init_capacity * sizeof(Entry *));
	if (new_entries == NULL)
		return HM_ERR_MEMORY_ALLOCATION;

	*output = new_entries;
	return HM_SUCCESS;
}

// ##################   public   ##################

enum HashMapError new_hash_map(void (*value_destructor)(void *value), HashMap **const output)
{
	if (output == NULL)
		return HM_ERR_NULL_ARGUMENT;

	HashMap *map = (HashMap *)malloc(sizeof(HashMap));
	if (map == NULL)
		return HM_ERR_MEMORY_ALLOCATION;

	Entry **entries = NULL;
	int err_entries = _new_entries_array(HASH_MAP_INIT_CAPACITY, &entries);
	if (err_entries)
	{
		free(map);
		return err_entries;
	}

	map->entries = entries;
	map->n_ent = 0;
	map->capacity = HASH_MAP_INIT_CAPACITY;
	map->value_destructor = value_destructor == NULL ? NULL : value_destructor;
	*output = map;

	return HM_SUCCESS;
}

enum HashMapError add_destructor_hm(HashMap *const map, void (*value_destructor)(void *value))
{
	if (map == NULL || value_destructor == NULL)
		return HM_ERR_NULL_ARGUMENT;

	map->value_destructor = value_destructor;
	return HM_SUCCESS;
}

void free_hash_map(HashMap *map)
{
	if (map == NULL)
	{
		return;
	}
	for (int i = 0; i < map->capacity; ++i)
	{
		Entry *e = (map->entries)[i];
		if (e == NULL)
			continue;
		if (map->value_destructor != NULL)
			(map->value_destructor)(e);
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

	int isReplaced = 0;
	if (_incert_replace_entry(newEntry, map->capacity, map->entries, &isReplaced, map->value_destructor))
		return 1;

	// if 0, Entry was added. If 1, it replaced another, older one with the same key.
	if (isReplaced == 0)
		(map->n_ent)++;

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
			if (map->value_destructor != NULL)
				(map->value_destructor)(ent);

			_free_entry(ent);
			(map->entries)[i] = NULL;
			(map->n_ent)--;
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
			if (map->value_destructor != NULL)
				(map->value_destructor)(ent);

			_free_entry(ent);
			(map->entries)[i] = NULL;
			(map->n_ent)--;
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

	DynamicArray *entries_to_keep = NULL;
	int err_entries_to_keep = new_dynamic_array(VOID_PTR, &entries_to_keep);
	if (err_entries_to_keep)
		return 2;

	DynamicArray *entreis_to_estroy = NULL;
	if (map->value_destructor != NULL)
	{
		int err_entries_to_destroy = new_dynamic_array(VOID_PTR, &entreis_to_estroy);
		if (err_entries_to_destroy)
		{
			free_dynamic_array(entries_to_keep);
			return 2;
		}
	}

	for (size_t i = 0; i < map->capacity; ++i)
	{
		Entry *const e = map->entries[i];
		if (e == NULL)
			continue;

		if (selector(e))
		{
			int push_err = push_ptr_da(entries_to_keep, e);
			if (push_err > 0)
			{
				free_dynamic_array(entries_to_keep);
				free_dynamic_array(entreis_to_estroy);
				return 30 + push_err;
			}
		}
		else
		{
			if (map->value_destructor == NULL)
				continue;

			int push_err = push_ptr_da(entreis_to_estroy, e);
			if (push_err > 0)
			{
				free_dynamic_array(entries_to_keep);
				free_dynamic_array(entreis_to_estroy);
				return 30 + push_err;
			}
		}
	}

	size_t newEntriesCapacity = entries_to_keep->count + HASH_MAP_INIT_CAPACITY;
	Entry **newEntries = _new_entries_array(newEntriesCapacity);
	if (newEntries == NULL)
	{
		free_dynamic_array(entries_to_keep);
		free_dynamic_array(entreis_to_estroy);
		return 2;
	}

	// Case: all Entries must be removed from the HashMap.
	if (entries_to_keep->count == 0)
		goto _hashmap_manipulation_stage;

	// Case: leave selected Entries.
	for (size_t i = 0; i < entries_to_keep->count; ++i)
	{
		Entry *currentEntry = NULL;

		int atErr = at_da(entries_to_keep, i, (void **)&currentEntry);
		if (atErr)
		{
			free_dynamic_array(entries_to_keep);
			free_dynamic_array(entreis_to_estroy);
			return 3;
		}

		if (currentEntry == NULL)
		{
			free_dynamic_array(entries_to_keep);
			free_dynamic_array(entreis_to_estroy);
			return 4;
		}

		int incertErr = _incert_entry(currentEntry, newEntriesCapacity, newEntries);
		if (incertErr)
		{
			free_dynamic_array(entries_to_keep);
			free_dynamic_array(entreis_to_estroy);
			return 5;
		}
	}

_hashmap_manipulation_stage:
	if (map->value_destructor != NULL)
	{
		for (size_t i = 0; i < entreis_to_estroy->count; ++i)
		{
			Entry *currentEntry = NULL;

			int atErr = at_da(entreis_to_estroy, i, (void **)&currentEntry);
			if (atErr)
			{
				free_dynamic_array(entries_to_keep);
				free_dynamic_array(entreis_to_estroy);
				free(newEntries);
				return 3;
			}

			if (currentEntry == NULL)
				continue;

			(map->value_destructor)(currentEntry);
		}
	}

	free(map->entries);
	map->entries = newEntries;
	map->capacity = newEntriesCapacity;
	map->n_ent = entries_to_keep->count;

	free_dynamic_array(entries_to_keep);
	free_dynamic_array(entreis_to_estroy);

	return 0;
}
