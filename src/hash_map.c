#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/hash_map.h"
#include "../include/dynamic_array.h"

// ##################   static   ##################

static enum GalxlibError _new_entry(const char *key, void *value, Entry **const output)
{
	if (key == NULL || value == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	Entry *entry = (Entry *)malloc(sizeof(Entry));
	if (entry == NULL)
		return GLX_ERR_MEMORY_ALLOCATION;

	size_t key_length = strnlen(key, HASH_MAP_KEY_MAX_LENGTH);
	if (key_length == 0)
		return GLX_ERR_KEY_EMPTY;
	else if (key_length == HASH_MAP_KEY_MAX_LENGTH)
		return GLX_ERR_KEY_MAX_LENGTH;

	// +1 for the null-terminator
	char *key_copy = (char *)calloc(key_length + 1, sizeof(char));
	if (key_copy == NULL)
		return GLX_ERR_MEMORY_ALLOCATION;

	if (strncpy(key_copy, key, key_length) != key_copy)
	{
		free(entry);
		return GLX_ERR_KEY_COPY;
	}

	entry->key = key_copy;
	entry->value = value;
	*output = entry;

	return GLX_SUCCESS;
}

static enum GalxlibError _free_entry(Entry **entry)
{
	if (entry == NULL)
		return GLX_ERR_NULL_ARGUMENT;
	if (*entry == NULL)
		return GLX_SUCCESS;

	free((*entry)->key);
	free(*entry);
	*entry = NULL;

	return GLX_SUCCESS;
}

static enum GalxlibError _free_entry_d(Entry **entry, void (*value_destructor)(void **value))
{
	if (entry == NULL)
		return GLX_ERR_NULL_ARGUMENT;
	if (*entry == NULL)
		return GLX_SUCCESS;

	free((*entry)->key);

	if (value_destructor != NULL)
		value_destructor(&((*entry)->value));

	free(*entry);
	*entry = NULL;

	return GLX_SUCCESS;
}

static enum GalxlibError _hash_str(const char *const str, const size_t hm_capacity, size_t *const output)
{
	if (str == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	size_t str_length = strnlen(str, HASH_MAP_KEY_MAX_LENGTH);
	if (str_length == 0)
		return GLX_ERR_KEY_EMPTY;
	else if (str_length == HASH_MAP_KEY_MAX_LENGTH)
		return GLX_ERR_KEY_MAX_LENGTH;

	size_t ascii_value = 0;
	for (size_t i = 0; i < str_length; ++i)
		ascii_value += (size_t)(str[i]);

	*output = (size_t)((ascii_value * str_length) % hm_capacity);

	return GLX_SUCCESS;
}

static enum GalxlibError _incert_entry(Entry *const new_entry, const size_t hm_capacity, Entry **const hm_entries)
{
	if (new_entry == NULL || hm_entries == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	size_t index = 0;
	int err_hash = _hash_str(new_entry->key, hm_capacity, &index);
	if (err_hash)
		return err_hash;

	for (size_t i = index; i < hm_capacity; ++i)
	{
		if (hm_entries[i] == NULL)
		{
			hm_entries[i] = new_entry;
			return GLX_SUCCESS;
		}
	}
	for (size_t i = 0; i < index; ++i)
	{
		if (hm_entries[i] == NULL)
		{
			hm_entries[i] = new_entry;
			return GLX_SUCCESS;
		}
	}

	return GLX_ERR_FULL;
}

static inline enum GalxlibError _decrement_n_ent(HashMap *const map)
{
	if (NULL == map)
		return GLX_ERR_NULL_ARGUMENT;

	if ((size_t)0 == map->n_ent)
		return GLX_ERR_ZERO_LENGTH;

	(map->n_ent)--;
	return GLX_SUCCESS;
}

/**
 * Checks if HashMap::n_ent has reached the limit of HashMap::capacity.
 * If the limit is reached GLX_ERR_FULL is returned.
 * @return A value of the @ref HashMapError:
 *
 * - GLX_SUCCESS
 *
 * - GLX_ERR_NULL_ARGUMENT
 *
 * - GLX_ERR_FULL
 */
/* static inline enum GalxlibError _is_n_ent_limit_reached(HashMap *const map)
{
	if (NULL == map)
		return GLX_ERR_NULL_ARGUMENT;

	if (map->n_ent >= map->capacity)
		return GLX_ERR_FULL;

	return GLX_SUCCESS;
} */

/**
 * @brief Incerts the Entry into the HashMap and increments (i) the n_ent counter.
 * If an Entry with the same key is found, the old one is replaced (r) with the new one.
 * In which case, the n_ent counter is not incremented.
 */
static enum GalxlibError _incert_entry_ri(
    HashMap *const hm,
    Entry *const new_entry)
{
	if (hm == NULL || new_entry == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	size_t const hm_capacity = hm->capacity;
	Entry **const hm_entries = hm->entries;

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
			(hm->n_ent)++;
			return GLX_SUCCESS;
		}
		else if (strcmp(current_entry->key, new_entry->key) == 0)
		{
			_free_entry_d(&current_entry, hm->value_destructor);

			hm_entries[i] = new_entry;
			return GLX_SUCCESS;
		}
	}
	for (size_t i = 0; i < index; ++i)
	{
		Entry *current_entry = hm_entries[i];
		if (current_entry == NULL)
		{
			hm_entries[i] = new_entry;
			(hm->n_ent)++;
			return GLX_SUCCESS;
		}
		else if (strcmp(current_entry->key, new_entry->key) == 0)
		{
			_free_entry_d(&current_entry, hm->value_destructor);

			hm_entries[i] = new_entry;
			return GLX_SUCCESS;
		}
	}

	return GLX_ERR_FULL;
}

static enum GalxlibError _transfer_entries(
    Entry **const source,
    Entry **const destination,
    const size_t source_capacity,
    const size_t destination_capacity)
{
	if (source == NULL || destination == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	for (size_t i = 0; i < source_capacity; ++i)
	{
		Entry *entry = source[i];
		if (entry == NULL)
			continue;

		int err_incert = _incert_entry(entry, destination_capacity, destination);
		if (err_incert)
			return err_incert;
	}

	return GLX_SUCCESS;
}

static enum GalxlibError _extend_hm(HashMap *const map)
{
	int empty_space = map->capacity - map->n_ent;
	if (empty_space > (map->capacity * 0.25))
		return GLX_SUCCESS;

	int is_max_capacity_reached = map->capacity >= HASH_MAP_MAX_CAPACITY;
	int is_map_full = map->n_ent >= map->capacity;
	if (is_max_capacity_reached && is_map_full)
		return GLX_ERR_MAX_CAPACITY;
	else if (is_max_capacity_reached && !is_map_full) // Notice: ! (not-operator)
		return GLX_SUCCESS;

	size_t new_capacity = ceil(map->n_ent * 1.5);
	if (new_capacity > HASH_MAP_MAX_CAPACITY)
		new_capacity = HASH_MAP_MAX_CAPACITY;

	Entry **new_entries = (Entry **)calloc(new_capacity, sizeof(Entry *));
	if (new_entries == NULL)
		return GLX_ERR_MEMORY_ALLOCATION;

	int err_trasfer = _transfer_entries(map->entries, new_entries, map->capacity, new_capacity);
	if (err_trasfer)
	{
		free(new_entries);
		return err_trasfer;
	}

	free(map->entries);
	map->entries = new_entries;
	map->capacity = new_capacity;

	return GLX_SUCCESS;
}

static enum GalxlibError _squish_hm(HashMap *const map)
{
	int empty_space = map->capacity - map->n_ent;
	// reduce capacity if more than 90% are free
	if (empty_space < (map->capacity * 0.9))
		return GLX_SUCCESS;

	size_t new_capacity = ceil(map->n_ent * 1.5);
	Entry **new_entries = (Entry **)calloc(new_capacity, sizeof(Entry *));
	if (new_entries == NULL)
		return GLX_ERR_MEMORY_ALLOCATION;

	int err_trasfer = _transfer_entries(map->entries, new_entries, map->capacity, new_capacity);
	if (err_trasfer)
	{
		free(new_entries);
		return err_trasfer;
	}

	free(map->entries);
	map->entries = new_entries;
	map->capacity = new_capacity;

	return GLX_SUCCESS;
}

static enum GalxlibError _new_entries_array(const size_t init_capacity, Entry ***const output)
{
	if (output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	Entry **new_entries = (Entry **)calloc(init_capacity, sizeof(Entry *));
	if (new_entries == NULL)
		return GLX_ERR_MEMORY_ALLOCATION;

	*output = new_entries;
	return GLX_SUCCESS;
}

// ##################   public   ##################

enum GalxlibError new_hash_map(HashMap **const output)
{
	if (NULL == output)
		return GLX_ERR_NULL_ARGUMENT;

	HashMap *map = (HashMap *)malloc(sizeof(HashMap));
	if (NULL == map)
		return GLX_ERR_MEMORY_ALLOCATION;

	Entry **entries = NULL;
	int err = _new_entries_array(HASH_MAP_INIT_CAPACITY, &entries);
	if (err)
	{
		free(map);
		return err;
	}

	map->entries = entries;
	map->n_ent = 0;
	map->capacity = HASH_MAP_INIT_CAPACITY;
	map->value_destructor = NULL;
	*output = map;

	return GLX_SUCCESS;
}

enum GalxlibError new_hash_map_d(void (*value_destructor)(void **value), HashMap **const output)
{
	if (NULL == value_destructor || NULL == output)
		return GLX_ERR_NULL_ARGUMENT;

	HashMap *map = (HashMap *)malloc(sizeof(HashMap));
	if (NULL == map)
		return GLX_ERR_MEMORY_ALLOCATION;

	Entry **entries = NULL;
	int err = _new_entries_array(HASH_MAP_INIT_CAPACITY, &entries);
	if (err)
	{
		free(map);
		return err;
	}

	map->entries = entries;
	map->n_ent = 0;
	map->capacity = HASH_MAP_INIT_CAPACITY;
	map->value_destructor = value_destructor;
	*output = map;

	return GLX_SUCCESS;
}

enum GalxlibError add_destructor_hm(HashMap *const map, void (*value_destructor)(void **value))
{
	if (map == NULL || value_destructor == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	map->value_destructor = value_destructor;
	return GLX_SUCCESS;
}

enum GalxlibError free_hash_map(HashMap **map)
{
	if (map == NULL)
		return GLX_ERR_NULL_ARGUMENT;
	if (*map == NULL)
		return GLX_SUCCESS;

	for (size_t i = 0; i < (*map)->capacity; ++i)
	{
		Entry *e = ((*map)->entries)[i];
		if (e == NULL)
			continue;
		_free_entry(&e);
	}

	free((*map)->entries);
	free(*map);
	*map = NULL;

	return GLX_SUCCESS;
}

enum GalxlibError free_hash_map_d(HashMap **map)
{
	if (map == NULL)
		return GLX_ERR_NULL_ARGUMENT;
	if (*map == NULL)
		return GLX_SUCCESS;
	if ((*map)->value_destructor == NULL)
		return GLX_ERR_NULL_DESTRUCTOR;

	for (size_t i = 0; i < (*map)->capacity; ++i)
	{
		Entry *e = ((*map)->entries)[i];
		if (e == NULL)
			continue;
		_free_entry_d(&e, (*map)->value_destructor);
	}

	free((*map)->entries);
	free(*map);
	*map = NULL;

	return GLX_SUCCESS;
}

enum GalxlibError put_hm(HashMap *const map, char *const key, void *const value)
{
	if (map == NULL || key == NULL || value == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	int err_extend = _extend_hm(map);
	if (err_extend)
		return err_extend;

	Entry *new_entry = NULL;
	int err_new_entry = _new_entry(key, value, &new_entry);
	if (err_new_entry)
		return err_new_entry;

	int err_incert = _incert_entry_ri(map, new_entry);
	if (err_incert)
		return err_incert;

	return GLX_SUCCESS;
}

enum GalxlibError get_hm(const HashMap *const map, const char *const key, void **const output)
{
	if (map == NULL || key == NULL || output == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (map->n_ent == 0)
		return GLX_ERR_ZERO_LENGTH;

	size_t index = 0;
	int err_hash = _hash_str(key, map->capacity, &index);
	if (err_hash)
		return err_hash;

	for (size_t i = index; i < map->capacity; ++i)
	{
		Entry *ent = (map->entries)[i];
		if (ent == NULL)
			continue;
		if (strcmp(key, ent->key) == 0)
		{
			*output = ent->value;
			return GLX_SUCCESS;
		}
	}
	for (size_t i = 0; i < index; ++i)
	{
		Entry *ent = (map->entries)[i];
		if (ent == NULL)
			continue;
		if (strcmp(key, ent->key) == 0)
		{
			*output = ent->value;
			return GLX_SUCCESS;
		}
	}
	return GLX_ERR_NO_MATCH;
}

enum GalxlibError remove_hm(HashMap *const map, const char *const key)
{
	if (map == NULL || key == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (map->n_ent == 0)
		return GLX_ERR_ZERO_LENGTH;

	int err_squish = _squish_hm(map);
	if (err_squish)
		return err_squish;

	size_t index = 0;
	int err_hash = _hash_str(key, map->capacity, &index);
	if (err_hash)
		return err_hash;

	for (size_t i = index; i < map->capacity; ++i)
	{
		Entry *ent = (map->entries)[i];
		if (ent == NULL)
			continue;

		if (strcmp(key, ent->key) == 0)
		{
			_free_entry_d(&ent, map->value_destructor);
			(map->entries)[i] = NULL;
			_decrement_n_ent(map);
			return GLX_SUCCESS;
		}
	}
	for (size_t i = 0; i < index; ++i)
	{
		Entry *ent = (map->entries)[i];
		if (ent == NULL)
			continue;

		if (strcmp(key, ent->key) == 0)
		{
			_free_entry_d(&ent, map->value_destructor);
			(map->entries)[i] = NULL;
			_decrement_n_ent(map);
			return GLX_SUCCESS;
		}
	}
	return GLX_ERR_NO_MATCH;
}

enum GalxlibError process_e_hm(const HashMap *const map, void (*processor)(const Entry *const ptr))
{
	if (map == NULL || processor == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	for (size_t i = 0; i < map->capacity; ++i)
	{
		const Entry *const e = map->entries[i];
		if (e != NULL)
			processor(e);
	}

	return GLX_SUCCESS;
}

enum GalxlibError process_v_hm(const HashMap *const map, void (*processor)(const void *const ptr))
{
	if (map == NULL || processor == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	for (size_t i = 0; i < map->capacity; ++i)
	{
		const Entry *const e = map->entries[i];
		if (e != NULL)
			processor(e->value);
	}

	return GLX_SUCCESS;
}

enum GalxlibError filter_hm(HashMap *const map, int (*selector)(const Entry *const ptr))
{
	if (map == NULL || selector == NULL)
		return GLX_ERR_NULL_ARGUMENT;

	if (map->n_ent == 0)
		return GLX_SUCCESS;

	enum GalxlibError error_code = GLX_SUCCESS;

	DynamicArray *entries_to_keep = NULL;
	if (new_dynamic_array(DA_PTR, &entries_to_keep))
	{
		error_code = GLX_ERR_MEMORY_ALLOCATION;
		goto _error_case;
	}

	DynamicArray *entries_to_destroy = NULL;
	if (new_dynamic_array(DA_PTR, &entries_to_destroy))
	{
		error_code = GLX_ERR_MEMORY_ALLOCATION;
		goto _error_case;
	}

	// Sort Entries.
	for (size_t i = 0; i < map->capacity; ++i)
	{
		Entry *const e = map->entries[i];
		if (e == NULL)
			continue;

		if (selector(e))
		{
			if (push_ptr_da(entries_to_keep, e))
			{
				error_code = GLX_ERR_MEMORY_ALLOCATION;
				goto _error_case;
			}
		}
		else
		{
			if (push_ptr_da(entries_to_destroy, e))
			{
				error_code = GLX_ERR_MEMORY_ALLOCATION;
				goto _error_case;
			}
		}
	}

	size_t new_entries_capacity = entries_to_keep->count + HASH_MAP_INIT_CAPACITY;
	Entry **new_entries = NULL;
	int err_new_entries = _new_entries_array(new_entries_capacity, &new_entries);
	if (err_new_entries)
		return err_new_entries;

	// Fill new_entries[] with the selected Entries.
	for (size_t i = 0; i < entries_to_keep->count; ++i)
	{
		Entry *current_entry = NULL;

		int err_at = at_da(entries_to_keep, i, (void **)&current_entry);
		if (err_at != GLX_SUCCESS && err_at != GLX_ERR_ZERO_LENGTH)
		{
			error_code = GLX_ERR_MEMORY_ALLOCATION;
			goto _error_case;
		}

		int err_incert = _incert_entry(current_entry, new_entries_capacity, new_entries);
		if (err_incert)
		{
			error_code = err_incert;
			goto _error_case;
		}
	}

	// Free memory of the removed Entries.
	for (size_t i = 0; i < entries_to_destroy->count; ++i)
	{
		Entry *current_entry = NULL;

		int err_at = at_da(entries_to_destroy, i, (void **)&current_entry);
		if (err_at != GLX_SUCCESS && err_at != GLX_ERR_ZERO_LENGTH)
		{
			error_code = GLX_ERR_MEMORY_ALLOCATION;
			goto _error_case;
		}

		if (current_entry == NULL)
			continue;

		_free_entry_d(&current_entry, map->value_destructor);
	}

	free(map->entries);
	map->entries = new_entries;
	map->capacity = new_entries_capacity;
	map->n_ent = entries_to_keep->count;

	free_dynamic_array(&entries_to_keep);
	free_dynamic_array(&entries_to_destroy);

	return error_code;

_error_case:
	if (new_entries != NULL)
		free(new_entries);
	if (entries_to_keep != NULL)
		free_dynamic_array(&entries_to_keep);
	if (entries_to_destroy != NULL)
		free_dynamic_array(&entries_to_destroy);

	return error_code;
}
