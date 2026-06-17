#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../../include/hash_map.h"
#include "../include/hash_map.h"
#include "../../include/gstring.h"
#include <assert.h>

static void test_new_hash_map(void)
{
	// =========================================================================
	// 1. Test Case: NULL Argument Error
	// =========================================================================
	// Passing NULL to the function must trigger an immediate guard rail check.
	enum HashMapError err_null = new_hash_map(NULL);
	assert(err_null == HM_ERR_NULL_ARGUMENT);

	// =========================================================================
	// 2. Test Case: Successful Initialization
	// =========================================================================
	HashMap *map = NULL;
	enum HashMapError err_success = new_hash_map(&map);

	// Assert structural integrity on successful allocation
	assert(err_success == HM_SUCCESS);
	assert(map != NULL);
	assert(map->entries != NULL);
	assert(map->n_ent == 0);
	assert(map->capacity == HASH_MAP_INIT_CAPACITY);
	assert(map->value_destructor == NULL);

	// =========================================================================
	// 3. Cleanup Validation
	// =========================================================================
	// Safely dispose of the created map to ensure no memory leaks occur in the test suite
	enum HashMapError err_free = free_hash_map(&map);
	assert(err_free == HM_SUCCESS);
	assert(map == NULL);
}

static void test_value_destructor(void **value)
{
	if (value && *value)
	{
		free(*value);
		*value = NULL;
	}
}

static void test_new_hash_map_d(void)
{
	HashMap *map = NULL;

	// =========================================================================
	// 1. Test Case: NULL Value Destructor Guard Rail
	// =========================================================================
	enum HashMapError err_null_dest = new_hash_map_d(NULL, &map);
	assert(err_null_dest == HM_ERR_NULL_ARGUMENT);
	assert(map == NULL);

	// =========================================================================
	// 2. Test Case: NULL Output Array Guard Rail
	// =========================================================================
	enum HashMapError err_null_out = new_hash_map_d(test_value_destructor, NULL);
	assert(err_null_out == HM_ERR_NULL_ARGUMENT);

	// =========================================================================
	// 3. Test Case: Complete NULL Arguments Bound Check
	// =========================================================================
	enum HashMapError err_both_null = new_hash_map_d(NULL, NULL);
	assert(err_both_null == HM_ERR_NULL_ARGUMENT);

	// =========================================================================
	// 4. Test Case: Successful Initialization with Custom Destructor
	// =========================================================================
	enum HashMapError err_success = new_hash_map_d(test_value_destructor, &map);

	assert(err_success == HM_SUCCESS);
	assert(map != NULL);
	assert(map->entries != NULL);
	assert(map->n_ent == 0);
	assert(map->capacity == HASH_MAP_INIT_CAPACITY);
	assert(map->value_destructor == test_value_destructor);

	// =========================================================================
	// 5. Cleanup Validation
	// =========================================================================
	enum HashMapError err_free = free_hash_map(&map);
	assert(err_free == HM_SUCCESS);
	assert(map == NULL);
}

static void test_destructor_alpha(void **value) { (void)value; }
static void test_destructor_beta(void **value) { (void)value; }

static void test_add_destructor_hm(void)
{
	// =========================================================================
	// 1. Error Cases: NULL Argument Boundary Testing
	// =========================================================================

	// Case A: Map parameter is NULL, valid destructor provided
	enum HashMapError err_null_map = add_destructor_hm(NULL, test_destructor_alpha);
	assert(err_null_map == HM_ERR_NULL_ARGUMENT);

	// Instantiate a valid map to test the remaining parameter combinations
	HashMap *map = NULL;
	enum HashMapError err_init = new_hash_map(&map);
	assert(err_init == HM_SUCCESS);
	assert(map != NULL);
	assert(map->value_destructor == NULL); // Starts as NULL by default

	// Case B: Valid map provided, destructor parameter is NULL
	enum HashMapError err_null_dest = add_destructor_hm(map, NULL);
	assert(err_null_dest == HM_ERR_NULL_ARGUMENT);

	// Case C: Both parameters are completely NULL
	enum HashMapError err_both_null = add_destructor_hm(NULL, NULL);
	assert(err_both_null == HM_ERR_NULL_ARGUMENT);

	// Confirm that the failed operations did not corrupt or modify the map's destructor
	assert(map->value_destructor == NULL);

	// =========================================================================
	// 2. Success Case: Attaching a Destructor
	// =========================================================================
	enum HashMapError err_add_success = add_destructor_hm(map, test_destructor_alpha);
	assert(err_add_success == HM_SUCCESS);
	assert(map->value_destructor == test_destructor_alpha);

	// =========================================================================
	// 3. Success Case: Overwriting / Replacing an Existing Destructor
	// =========================================================================
	// According to the specification, if one is already present, it must be replaced
	enum HashMapError err_replace_success = add_destructor_hm(map, test_destructor_beta);
	assert(err_replace_success == HM_SUCCESS);
	assert(map->value_destructor == test_destructor_beta);

	// =========================================================================
	// 4. Cleanup Validation
	// =========================================================================
	enum HashMapError err_free = free_hash_map(&map);
	assert(err_free == HM_SUCCESS);
	assert(map == NULL);
}

static void test_free_hash_map(void)
{
	// =========================================================================
	// 1. Error Case: NULL double pointer parameter boundary check
	// =========================================================================
	// Passing a complete NULL to the argument must return HM_ERR_NULL_ARGUMENT.
	enum HashMapError err_null_param = free_hash_map(NULL);
	assert(err_null_param == HM_ERR_NULL_ARGUMENT);

	// =========================================================================
	// 2. Success Case: Safe No-Op when the target map pointer is already NULL
	// =========================================================================
	// Per documentation, if it points to NULL, nothing is done and HM_SUCCESS is returned.
	HashMap *null_map = NULL;
	enum HashMapError err_null_ptr = free_hash_map(&null_map);
	assert(err_null_ptr == HM_SUCCESS);
	assert(null_map == NULL); // Target pointer should remain safely unchanged

	// =========================================================================
	// 3. Success Case: Freeing an initialized empty hash map
	// =========================================================================
	HashMap *empty_map = NULL;
	enum HashMapError err_init_empty = new_hash_map(&empty_map);
	assert(err_init_empty == HM_SUCCESS);
	assert(empty_map != NULL);

	enum HashMapError err_free_empty = free_hash_map(&empty_map);
	assert(err_free_empty == HM_SUCCESS);
	assert(empty_map == NULL); // Pointer must be set to NULL after execution

	// =========================================================================
	// 4. Success Case: Freeing a populated hash map (verifying internal loop)
	// =========================================================================
	HashMap *populated_map = NULL;
	enum HashMapError err_init_pop = new_hash_map(&populated_map);
	assert(err_init_pop == HM_SUCCESS);

	// Insert mock entries to ensure the inner entry cleanup logic runs safely
	char *key1 = "sample_key_1";
	char *val1 = "sample_value_1";
	char *key2 = "sample_key_2";
	char *val2 = "sample_value_2";

	enum HashMapError err_put1 = put_hm(populated_map, key1, val1);
	assert(err_put1 == HM_SUCCESS);
	enum HashMapError err_put2 = put_hm(populated_map, key2, val2);
	assert(err_put2 == HM_SUCCESS);
	assert(populated_map->n_ent == 2); // Confirm tracking elements are active

	// This call will clean up inner entry allocations and set our pointer to NULL
	enum HashMapError err_free_pop = free_hash_map(&populated_map);
	assert(err_free_pop == HM_SUCCESS);
	assert(populated_map == NULL);
}

static int test_destructor_execution_count = 0;

static void test_free_d_destructor(void **value)
{
	if (value && *value)
	{
		test_destructor_execution_count++;
		free(*value); // Safely clean up dynamically assigned heap values
		*value = NULL;
	}
}

static void test_free_hash_map_d(void)
{
	// =========================================================================
	// 1. Error Case: NULL double pointer parameter boundary check
	// =========================================================================
	// Passing a true NULL down to the parameter must yield HM_ERR_NULL_ARGUMENT.
	enum HashMapError err_null_param = free_hash_map_d(NULL);
	assert(err_null_param == HM_ERR_NULL_ARGUMENT);

	// =========================================================================
	// 2. Success Case: Safe No-Op behavior when target pointer is already NULL
	// =========================================================================
	// If the referenced pointer points to NULL, it should break early with HM_SUCCESS.
	HashMap *null_map = NULL;
	enum HashMapError err_null_ptr = free_hash_map_d(&null_map);
	assert(err_null_ptr == HM_SUCCESS);
	assert(null_map == NULL);

	// =========================================================================
	// 3. Error Case: Invocation on a map missing its destructor initialization
	// =========================================================================
	// Creating a standard map sets value_destructor to NULL.
	// Calling free_hash_map_d on it must reject the execution with HM_ERR_NULL_DESTRUCTOR.
	HashMap *map_without_destructor = NULL;
	enum HashMapError err_init_standard = new_hash_map(&map_without_destructor);
	assert(err_init_standard == HM_SUCCESS);
	assert(map_without_destructor->value_destructor == NULL);

	enum HashMapError err_missing_dest = free_hash_map_d(&map_without_destructor);
	assert(err_missing_dest == HM_ERR_NULL_DESTRUCTOR);
	assert(map_without_destructor != NULL); // Map layout must remain intact

	// Manual fallback clean up of this instance to avoid memory leaks
	enum HashMapError err_cleanup_fallback = free_hash_map(&map_without_destructor);
	assert(err_cleanup_fallback == HM_SUCCESS);

	// =========================================================================
	// 4. Success Case: Freeing an initialized empty map with destructor attached
	// =========================================================================
	HashMap *empty_map_d = NULL;
	enum HashMapError err_init_d = new_hash_map_d(test_free_d_destructor, &empty_map_d);
	assert(err_init_d == HM_SUCCESS);
	assert(empty_map_d->value_destructor == test_free_d_destructor);

	enum HashMapError err_free_empty = free_hash_map_d(&empty_map_d);
	assert(err_free_empty == HM_SUCCESS);
	assert(empty_map_d == NULL); // The outer target pointer must clear down to NULL

	// =========================================================================
	// 5. Success Case: Freeing a populated map & evaluating cascade destructors
	// =========================================================================
	HashMap *populated_map = NULL;
	enum HashMapError err_init_pop = new_hash_map_d(test_free_d_destructor, &populated_map);
	assert(err_init_pop == HM_SUCCESS);

	// Prepare heap tracking entries to put inside the table
	int *payload_val1 = (int *)malloc(sizeof(int));
	int *payload_val2 = (int *)malloc(sizeof(int));
	assert(payload_val1 != NULL && payload_val2 != NULL);
	*payload_val1 = 42;
	*payload_val2 = 84;

	enum HashMapError err_put1 = put_hm(populated_map, "first_key", payload_val1);
	assert(err_put1 == HM_SUCCESS);
	enum HashMapError err_put2 = put_hm(populated_map, "second_key", payload_val2);
	assert(err_put2 == HM_SUCCESS);
	assert(populated_map->n_ent == 2);

	// Reset tracker count right before kicking off the clean up cascade
	test_destructor_execution_count = 0;

	// This must purge internal entries, free data payloads, and nullify the handle
	enum HashMapError err_free_populated = free_hash_map_d(&populated_map);
	assert(err_free_populated == HM_SUCCESS);
	assert(populated_map == NULL);

	// Verify that the inner table loop targeted and ran the hook exactly 2 times
	assert(test_destructor_execution_count == 2);
}

static int test_put_destructor_count = 0;

static void test_put_value_destructor(void **value)
{
	if (value && *value)
	{
		test_put_destructor_count++;
		free(*value);
		*value = NULL;
	}
}

static void test_put_hm(void)
{
	// =========================================================================
	// 1. Error Cases: NULL Argument Boundary Testing
	// =========================================================================
	HashMap *map = NULL;
	enum HashMapError err = new_hash_map(&map);
	assert(err == HM_SUCCESS);

	// Ensure all permutations of missing NULL arguments are rejected
	assert(put_hm(NULL, "valid_key", "valid_val") == HM_ERR_NULL_ARGUMENT);
	assert(put_hm(map, NULL, "valid_val") == HM_ERR_NULL_ARGUMENT);
	assert(put_hm(map, "valid_key", NULL) == HM_ERR_NULL_ARGUMENT);

	// =========================================================================
	// 2. Success Case: Standard Insertion & Value Verification
	// =========================================================================
	char *key = "target_key";
	char *val1 = "initial_value";

	err = put_hm(map, key, val1);
	assert(err == HM_SUCCESS);

	void *retrieved = NULL;
	err = get_hm(map, key, &retrieved);
	assert(err == HM_SUCCESS);
	assert(retrieved == val1); // Ensure exact pointer matching

	// =========================================================================
	// 3. Edge Case: Overwriting an existing key (Without a Destructor)
	// =========================================================================
	char *val2 = "replaced_value";

	err = put_hm(map, key, val2);
	assert(err == HM_SUCCESS);

	err = get_hm(map, key, &retrieved);
	assert(err == HM_SUCCESS);
	assert(retrieved == val2); // Verify the old pointer value was overwritten

	// Safely dispose of the first map instance
	err = free_hash_map(&map);
	assert(err == HM_SUCCESS);

	// =========================================================================
	// 4. Edge Case: Overwriting an existing key (With a Destructor attached)
	// =========================================================================
	HashMap *map_d = NULL;
	err = new_hash_map_d(test_put_value_destructor, &map_d);
	assert(err == HM_SUCCESS);

	// Allocate tracking memory on the heap so our mock free() can safely execute
	int *heap_payload1 = (int *)malloc(sizeof(int));
	int *heap_payload2 = (int *)malloc(sizeof(int));
	assert(heap_payload1 != NULL && heap_payload2 != NULL);
	*heap_payload1 = 100;
	*heap_payload2 = 200;

	char *dup_key = "collision_key";

	// Initial insertion of payload 1
	err = put_hm(map_d, dup_key, heap_payload1);
	assert(err == HM_SUCCESS);
	assert(map_d->n_ent == 1);

	// Reset tracking metric before executing the collision test
	test_put_destructor_count = 0;

	// Insertion of payload 2 under the identical key string (Triggers Replacement)
	err = put_hm(map_d, dup_key, heap_payload2);
	assert(err == HM_SUCCESS);

	// CRITICAL ASSERTION: Verify the old entry's value was caught and cleared by the destructor
	assert(test_put_destructor_count == 1);
	assert(map_d->n_ent == 1);

	// Confirm that fetching the key now safely yields the secondary payload pointer
	void *retrieved_d = NULL;
	err = get_hm(map_d, dup_key, &retrieved_d);
	assert(err == HM_SUCCESS);
	assert(*(int *)retrieved_d == 200);

	// =========================================================================
	// 5. Cleanup Cascade Verification
	// =========================================================================
	// Freeing the map must cascade and clear out payload 2, incrementing the execution count to 2
	err = free_hash_map_d(&map_d);
	assert(err == HM_SUCCESS);
	assert(map_d == NULL);
	assert(test_put_destructor_count == 2);
}

static void test_get_hm(void)
{
	// =========================================================================
	// 1. Error Cases: NULL Argument Boundary Testing
	// =========================================================================
	HashMap *map = NULL;
	enum HashMapError err_init = new_hash_map(&map);
	assert(err_init == HM_SUCCESS);
	assert(map != NULL);

	void *output_val = NULL;

	// Validate that all permutations of missing arguments return HM_ERR_NULL_ARGUMENT
	assert(get_hm(NULL, "test_key", &output_val) == HM_ERR_NULL_ARGUMENT);
	assert(get_hm(map, NULL, &output_val) == HM_ERR_NULL_ARGUMENT);
	assert(get_hm(map, "test_key", NULL) == HM_ERR_NULL_ARGUMENT);

	// =========================================================================
	// 2. Error Case: Querying an Empty Map
	// =========================================================================
	// Prior to looking up elements, the function returns HM_EMPTY if n_ent is 0
	assert(map->n_ent == 0);
	enum HashMapError err_empty = get_hm(map, "any_key", &output_val);
	assert(err_empty == HM_EMPTY);

	// =========================================================================
	// 3. Success Case: Populating the Map and Fetching Valid Keys
	// =========================================================================
	char *key_alpha = "alpha_key";
	char *val_alpha = "alpha_payload";
	char *key_beta = "beta_key";
	char *val_beta = "beta_payload";

	// Insert elements to make lookup paths viable
	assert(put_hm(map, key_alpha, val_alpha) == HM_SUCCESS);
	assert(put_hm(map, key_beta, val_beta) == HM_SUCCESS);
	assert(map->n_ent == 2);

	// Fetch the first element and verify the payload pointer matches perfectly
	output_val = NULL;
	enum HashMapError err_get_alpha = get_hm(map, key_alpha, &output_val);
	assert(err_get_alpha == HM_SUCCESS);
	assert(output_val == val_alpha);

	// Fetch the second element to ensure the internal indexing and loops are accurate
	output_val = NULL;
	enum HashMapError err_get_beta = get_hm(map, key_beta, &output_val);
	assert(err_get_beta == HM_SUCCESS);
	assert(output_val == val_beta);

	// =========================================================================
	// 4. Error Case: Querying a Missing Key (Map Not Empty)
	// =========================================================================
	// When a map contains values but a non-existent key is supplied, it must return HM_NOT_FOUND
	output_val = NULL;
	enum HashMapError err_not_found = get_hm(map, "non_existent_key", &output_val);
	assert(err_not_found == HM_NOT_FOUND);
	assert(output_val == NULL); // The output placeholder should remain unchanged

	// =========================================================================
	// 5. Cleanup Verification
	// =========================================================================
	enum HashMapError err_free = free_hash_map(&map);
	assert(err_free == HM_SUCCESS);
	assert(map == NULL);
}

static int test_remove_destructor_count = 0;

static void test_remove_value_destructor(void **value)
{
	if (value && *value)
	{
		test_remove_destructor_count++;
		free(*value);
		*value = NULL;
	}
}

static void test_remove_hm(void)
{
	// =========================================================================
	// 1. Error Cases: NULL Argument Boundary Testing
	// =========================================================================
	HashMap *map = NULL;
	enum HashMapError err = new_hash_map(&map);
	assert(err == HM_SUCCESS);

	// Verify all missing input combinations are blocked safely
	assert(remove_hm(NULL, "valid_key") == HM_ERR_NULL_ARGUMENT);
	assert(remove_hm(map, NULL) == HM_ERR_NULL_ARGUMENT);

	// =========================================================================
	// 2. Error Case: Requesting Removal on an Empty Table Layout
	// =========================================================================
	// Should break early returning HM_EMPTY if no nodes are active
	assert(remove_hm(map, "any_key") == HM_EMPTY);

	// =========================================================================
	// 3. Error Case: Requesting Removal for a Non-Existent Key
	// =========================================================================
	assert(put_hm(map, "active_key", "sample_payload") == HM_SUCCESS);

	// Querying an inactive item string must return HM_NOT_FOUND
	assert(remove_hm(map, "missing_key") == HM_NOT_FOUND);

	// =========================================================================
	// 4. Success Case: Removal Without Destructor Attached
	// =========================================================================
	assert(remove_hm(map, "active_key") == HM_SUCCESS);

	// Confirm that the key is completely gone from lookup paths
	void *lookup_verify = NULL;
	assert(get_hm(map, "active_key", &lookup_verify) == HM_EMPTY);

	// Dispose of initial test mapping instance safely
	assert(free_hash_map(&map) == HM_SUCCESS);

	// =========================================================================
	// 5. Success Case: Removal WITH Destructor Active (Verifying Cleanup Loop)
	// =========================================================================
	HashMap *map_d = NULL;
	err = new_hash_map_d(test_remove_value_destructor, &map_d);
	assert(err == HM_SUCCESS);

	// Allocate an item onto the heap so our tracker free() can safely execute
	int *heap_payload = (int *)malloc(sizeof(int));
	assert(heap_payload != NULL);
	*heap_payload = 999;

	assert(put_hm(map_d, "target_key", heap_payload) == HM_SUCCESS);

	// Reset tracking metrics right before cutting the value node loose
	test_remove_destructor_count = 0;

	// Execute the target element removal
	assert(remove_hm(map_d, "target_key") == HM_SUCCESS);

	assert(test_remove_destructor_count == 1);

	// Double check that map registry fields are empty
	assert(get_hm(map_d, "target_key", &lookup_verify) == HM_EMPTY);

	// Final clean up deallocation of the map container framework
	assert(free_hash_map_d(&map_d) == HM_SUCCESS);
	assert(map_d == NULL);
}

static int test_processor_invocation_count = 0;
static int test_seen_alpha = 0;
static int test_seen_beta = 0;

static void test_mock_entry_processor(const Entry *const ptr)
{
	if (ptr != NULL)
	{
		test_processor_invocation_count++;

		// Inspect the contents of the visited Entry to verify data integrity
		if (strcmp(ptr->key, "alpha_key") == 0)
		{
			assert(strcmp((char *)ptr->value, "alpha_value") == 0);
			test_seen_alpha = 1;
		}
		else if (strcmp(ptr->key, "beta_key") == 0)
		{
			assert(strcmp((char *)ptr->value, "beta_value") == 0);
			test_seen_beta = 1;
		}
	}
}

static void test_process_e_hm(void)
{
	// Initialize a temporary HashMap for verification
	HashMap *map = NULL;
	enum HashMapError err = new_hash_map(&map);
	assert(err == HM_SUCCESS);
	assert(map != NULL);

	// =========================================================================
	// 1. Error Cases: NULL Argument Protections
	// =========================================================================
	// Both arguments must be non-NULL to pass safety boundaries
	assert(process_e_hm(NULL, test_mock_entry_processor) == HM_ERR_NULL_ARGUMENT);
	assert(process_e_hm(map, NULL) == HM_ERR_NULL_ARGUMENT);

	// =========================================================================
	// 2. Success Case: Running over an Empty Map
	// =========================================================================
	// On an empty map, the loop executes up to capacity but shouldn't find non-NULL entries
	test_processor_invocation_count = 0;

	enum HashMapError err_empty = process_e_hm(map, test_mock_entry_processor);
	assert(err_empty == HM_SUCCESS);
	assert(test_processor_invocation_count == 0); // Processor callback should never execute

	// =========================================================================
	// 3. Success Case: Processing a Populated Map
	// =========================================================================
	// Populate the table with test items
	assert(put_hm(map, "alpha_key", "alpha_value") == HM_SUCCESS);
	assert(put_hm(map, "beta_key", "beta_value") == HM_SUCCESS);
	assert(map->n_ent == 2); // Confirm internal item counts

	// Reset tracking indicators prior to triggering the iteration pipeline
	test_processor_invocation_count = 0;
	test_seen_alpha = 0;
	test_seen_beta = 0;

	// Execute the entry processing function over the table
	enum HashMapError err_populated = process_e_hm(map, test_mock_entry_processor);
	assert(err_populated == HM_SUCCESS);

	// CRITICAL ASSERTIONS:
	// Ensure every single entry was processed exactly once
	assert(test_processor_invocation_count == 2);
	assert(test_seen_alpha == 1);
	assert(test_seen_beta == 1);

	// =========================================================================
	// 4. Clean Up
	// =========================================================================
	err = free_hash_map(&map);
	assert(err == HM_SUCCESS);
	assert(map == NULL);
}

static int test_v_processor_invocation_count = 0;
static int test_v_seen_alpha = 0;
static int test_v_seen_beta = 0;

static void test_mock_value_processor(const void *const ptr)
{
	if (ptr != NULL)
	{
		test_v_processor_invocation_count++;

		// Inspect the underlying value payloads to ensure data integrity
		if (strcmp((char *)ptr, "alpha_value") == 0)
		{
			test_v_seen_alpha = 1;
		}
		else if (strcmp((char *)ptr, "beta_value") == 0)
		{
			test_v_seen_beta = 1;
		}
	}
}

static void test_process_v_hm(void)
{
	// Initialize a temporary HashMap for verification
	HashMap *map = NULL;
	enum HashMapError err = new_hash_map(&map);
	assert(err == HM_SUCCESS);
	assert(map != NULL);

	// =========================================================================
	// 1. Error Cases: NULL Argument Protections
	// =========================================================================
	// Both arguments must be non-NULL to pass safety boundaries
	assert(process_v_hm(NULL, test_mock_value_processor) == HM_ERR_NULL_ARGUMENT);
	assert(process_v_hm(map, NULL) == HM_ERR_NULL_ARGUMENT);

	// =========================================================================
	// 2. Success Case: Running over an Empty Map
	// =========================================================================
	// On an empty map, the loop executes up to capacity but shouldn't find non-NULL entries
	test_v_processor_invocation_count = 0;

	enum HashMapError err_empty = process_v_hm(map, test_mock_value_processor);
	assert(err_empty == HM_SUCCESS);
	assert(test_v_processor_invocation_count == 0); // Value callback should never execute

	// =========================================================================
	// 3. Success Case: Processing a Populated Map
	// =========================================================================
	// Populate the table with test items
	assert(put_hm(map, "alpha_key", "alpha_value") == HM_SUCCESS);
	assert(put_hm(map, "beta_key", "beta_value") == HM_SUCCESS);
	assert(map->n_ent == 2); // Confirm internal item counts

	// Reset tracking indicators prior to triggering the iteration pipeline
	test_v_processor_invocation_count = 0;
	test_v_seen_alpha = 0;
	test_v_seen_beta = 0;

	// Execute the value processing function over the table
	enum HashMapError err_populated = process_v_hm(map, test_mock_value_processor);
	assert(err_populated == HM_SUCCESS);

	// CRITICAL ASSERTIONS:
	// Ensure every single value was processed exactly once
	assert(test_v_processor_invocation_count == 2);
	assert(test_v_seen_alpha == 1);
	assert(test_v_seen_beta == 1);

	// =========================================================================
	// 4. Clean Up
	// =========================================================================
	err = free_hash_map(&map);
	assert(err == HM_SUCCESS);
	assert(map == NULL);
}

static int test_filter_destructor_count = 0;

static void test_filter_value_destructor(void **value)
{
	if (value && *value)
	{
		test_filter_destructor_count++;
		free(*value);
		*value = NULL;
	}
}

/**
 * @brief Selector callback function for filter_hm.
 * Keeps entries starting with 'k' (keep), discards entries starting with 'r' (remove).
 */
static int test_filter_selector(const Entry *const ptr)
{
	if (ptr == NULL || ptr->key == NULL)
	{
		return 0;
	}
	return (ptr->key[0] == 'k');
}

static void test_filter_hm(void)
{
	// =========================================================================
	// 1. Error Cases: NULL Argument Boundary Testing
	// =========================================================================
	HashMap *map = NULL;
	enum HashMapError err = new_hash_map(&map);
	assert(err == HM_SUCCESS);
	assert(map != NULL);

	// Ensure missing pointer mutations are caught gracefully
	assert(filter_hm(NULL, test_filter_selector) == HM_ERR_NULL_ARGUMENT);
	assert(filter_hm(map, NULL) == HM_ERR_NULL_ARGUMENT);

	// =========================================================================
	// 2. Success Case: Filtering an Empty Map
	// =========================================================================
	// If n_ent is 0, filter_hm returns HM_SUCCESS early without modifying structures
	assert(map->n_ent == 0);
	assert(filter_hm(map, test_filter_selector) == HM_SUCCESS);
	assert(map->n_ent == 0);

	// =========================================================================
	// 3. Success Case: Filtering Populated Map WITHOUT a Value Destructor
	// =========================================================================
	assert(put_hm(map, "keep_alpha", "payload_1") == HM_SUCCESS);
	assert(put_hm(map, "remove_alpha", "payload_2") == HM_SUCCESS);
	assert(put_hm(map, "keep_beta", "payload_3") == HM_SUCCESS);
	assert(put_hm(map, "remove_beta", "payload_4") == HM_SUCCESS);
	assert(map->n_ent == 4);

	// Execute filter algorithm
	assert(filter_hm(map, test_filter_selector) == HM_SUCCESS);

	// Post-filter structural verification
	assert(map->n_ent == 2);

	void *out_val = NULL;
	// Retained elements should still be found
	assert(get_hm(map, "keep_alpha", &out_val) == HM_SUCCESS && out_val == "payload_1");
	assert(get_hm(map, "keep_beta", &out_val) == HM_SUCCESS && out_val == "payload_3");
	// Removed elements should now be gone
	assert(get_hm(map, "remove_alpha", &out_val) == HM_NOT_FOUND);
	assert(get_hm(map, "remove_beta", &out_val) == HM_NOT_FOUND);

	// Clean up first map instance safely
	assert(free_hash_map(&map) == HM_SUCCESS);

	// =========================================================================
	// 4. Success Case: Filtering Populated Map WITH a Value Destructor
	// =========================================================================
	HashMap *map_d = NULL;
	err = new_hash_map_d(test_filter_value_destructor, &map_d);
	assert(err == HM_SUCCESS);
	assert(map_d != NULL);

	// Allocate dynamic objects on the heap so they can be securely freed
	int *val1 = (int *)malloc(sizeof(int));
	int *val2 = (int *)malloc(sizeof(int));
	int *val3 = (int *)malloc(sizeof(int));
	int *val4 = (int *)malloc(sizeof(int));
	assert(val1 && val2 && val3 && val4);
	*val1 = 10;
	*val2 = 20;
	*val3 = 30;
	*val4 = 40;

	assert(put_hm(map_d, "keep_one", val1) == HM_SUCCESS);
	assert(put_hm(map_d, "remove_one", val2) == HM_SUCCESS);
	assert(put_hm(map_d, "keep_two", val3) == HM_SUCCESS);
	assert(put_hm(map_d, "remove_two", val4) == HM_SUCCESS);
	assert(map_d->n_ent == 4);

	// Reset global metrics tracking custom destructor passes
	test_filter_destructor_count = 0;

	// Trigger filter processing cycle
	assert(filter_hm(map_d, test_filter_selector) == HM_SUCCESS);

	// VERIFY CRITICAL LIFE CYCLE COUNTS:
	// Exactly 2 items ("remove_one" and "remove_two") should have been processed and freed
	assert(map_d->n_ent == 2);
	assert(test_filter_destructor_count == 2);

	// Verify retained entries still provide valid address pointer references
	assert(get_hm(map_d, "keep_one", &out_val) == HM_SUCCESS && *(int *)out_val == 10);
	assert(get_hm(map_d, "keep_two", &out_val) == HM_SUCCESS && *(int *)out_val == 30);
	assert(get_hm(map_d, "remove_one", &out_val) == HM_NOT_FOUND);

	// =========================================================================
	// 5. Cleanup Verification Cascade
	// =========================================================================
	// Disposing the map container completely must flush out the remaining 2 items
	assert(free_hash_map_d(&map_d) == HM_SUCCESS);
	assert(map_d == NULL);

	// 2 (from filtering phase) + 2 (from map disposal cascade) = 4 total passes
	assert(test_filter_destructor_count == 4);
}

void test_hash_map()
{
	puts("################## Test: HashMap ##################");

	test_new_hash_map();
	test_new_hash_map_d();
	test_add_destructor_hm();
	test_free_hash_map();
	test_free_hash_map_d();
	test_put_hm();
	test_get_hm();
	test_remove_hm();
	test_process_e_hm();
	test_process_v_hm();
	test_filter_hm();

	puts(ANSI_COLOR_GREEN "All tests passed!" ANSI_COLOR_RESET);
	puts("################## Test: HashMap ##################");
};

/* void hashMapProcessor(const Entry *const ptr)
{
    (*(int *)(ptr->value))++;
}
int hashMapFilter(const Entry *const ptr)
{
    if (*(int *)(ptr->value) >= 50)
	return 0;

    return 1;
}
void hashMapValueDestructor(void *value)
{
    free(value);
} */
/*
void hashMapTest()
{
    puts("################## Test: HashMap ##################");
    int VALUES = 10000;
    HashMap *map = NULL;
    int err_map = new_hash_map(NULL, &map);
    if (err_map)
    {
	puts("HashMap was not initialized.");
	printf("HashMap error code: %d\n", err_map);
	goto _test_failure;
    }

    puts("Testing: put_hm");
    clock_t c11 = clock();
    char strBuff[10] = {0};
    for (int i = 0; i < VALUES; ++i)
    {
	memset(strBuff, 0, 10);
	snprintf(strBuff, 10, "%d", i);
	int *num = (int *)malloc(sizeof(int));
	*num = i;

	printf("\rFunction put_hm adding: %d", i);
	int err = put_hm(map, strBuff, num);
	if (err)
	{
	    printf("Function put_hm returned with Error: %d\n", err);
	    goto _test_failure;
	}
    }
    clock_t c12 = clock();
    double dur1 = 1000.0 * (c12 - c11) / CLOCKS_PER_SEC;
    printf("\nCPU time used from put_hm (per clock()): %.2f ms\n", dur1);
    printf("CPU time used from put_hm (per clock()) per value: %.6f ms\n", dur1 / VALUES);

    puts("Testing: get_hm");
    clock_t c21 = clock();
    for (int i = 0; i < VALUES; ++i)
    {
	memset(strBuff, 0, 10);
	snprintf(strBuff, 10, "%d", i);
	int *res = NULL;
	int err_get = get_hm(map, strBuff, (void **)&res);
	if (err_get)
	{
	    printf("HashMap error code: %d\n", err_get);
	    goto _test_failure;
	}
	if (*res != i)
	{
	    printf("\nFunction get_hm returned (%d), but expected (%d).\n", *res, i);
	    goto _test_failure;
	}
	else
	{
	    printf("\rFunction get_hm returned (%d), at index(%d).", *res, i);
	}
    }
    clock_t c22 = clock();
    double dur2 = 1000.0 * (c22 - c21) / CLOCKS_PER_SEC;
    printf("\nCPU time used from get_hm (per clock()): %.2f ms\n", dur2);
    printf("CPU time used from get_hm (per clock()) per value: %.6f ms\n", dur2 / VALUES);

    puts("Testing: process_hm");
    clock_t cp1 = clock();
    int err_process = process_hm(map, hashMapProcessor);
    if (err_process)
    {
	printf("HashMap error code: %d\n", err_process);
	goto _test_failure;
    }
    clock_t cp2 = clock();
    double durp = 1000.0 * (cp2 - cp1) / CLOCKS_PER_SEC;
    printf("CPU time used from process_hm (per clock()): %.2f ms\n", durp);
    printf("CPU time used from process_hm (per clock()) per value: %.6f ms\n", durp / VALUES);
    printf("Checking results of process_hm...");
    for (int i = 0; i < VALUES; ++i)
    {
	memset(strBuff, 0, 10);
	snprintf(strBuff, 10, "%d", i);
	int *res = NULL;
	int err_get = get_hm(map, strBuff, (void **)&res);
	if (err_get)
	{
	    printf("HashMap error code: %d\n", err_get);
	    goto _test_failure;
	}
	if (*res != i + 1)
	{
	    printf("\nFunction get_hm returned (%d), but expected (%d) after process_hm.\n", *res, i + 1);
	    goto _test_failure;
	}
    }
    printf("done.\n");

    puts("Testing: remove_hm");
    clock_t c31 = clock();
    for (int i = 0; i < VALUES; ++i)
    {
	memset(strBuff, 0, 10);
	snprintf(strBuff, 10, "%d", i);
	int err = remove_hm(map, strBuff);
	if (err)
	{
	    printf("\nFunction remove_hm returned with Error: %d\n", err);
	    goto _test_failure;
	}
	printf("\rFunction remove_hm removed key: %s", strBuff);
    }
    clock_t c32 = clock();
    double dur3 = 1000.0 * (c32 - c31) / CLOCKS_PER_SEC;
    printf("\nCPU time used from remove_hm (per clock()): %.2f ms\n", dur3);
    printf("CPU time used from remove_hm (per clock()) per value: %.6f ms\n", dur3 / VALUES);

    if (map->n_ent != 0)
    {
	printf("HashMap elements are %d, but expected 0.\n", map->n_ent);
	goto _test_failure;
    }

    int err_free = free_hash_map(map);
    if (err_free)
    {
	printf("HashMap error code: %d\n", err_free);
	goto _test_failure;
    }

    // filter_hm test

    puts("Testing: filter_hm");
    HashMap *mapFilter = NULL;
    int err_map_filter_init = new_hash_map(NULL, &mapFilter);
    if (err_map_filter_init)
    {
	printf("HashMap error code: %d\n", err_map_filter_init);
	goto _test_failure;
    }
    if (!mapFilter)
    {
	puts("HashMap was not initialized for the test of filter_hm.");
	goto _test_failure;
    }
    for (int i = 0; i < VALUES; ++i)
    {
	memset(strBuff, 0, 10);
	snprintf(strBuff, 10, "%d", i);
	int *num = (int *)malloc(sizeof(int));
	*num = i;

	printf("\rFunction put_hm adding: %d", i);
	int err = put_hm(mapFilter, strBuff, num);
	if (err)
	{
	    printf("\nFunction put_hm returned with Error: %d\n", err);
	    goto _test_failure;
	}
    }
    puts("\nHashMap ready. Invoking filter_hm");
    clock_t cf1 = clock();
    int filterErr = filter_hm(mapFilter, hashMapFilter);
    if (filterErr)
    {
	printf("Function filter_hm returned with Error: %d\n", filterErr);
	goto _test_failure;
    }
    clock_t cf2 = clock();
    double durf = 1000.0 * (c32 - c31) / CLOCKS_PER_SEC;
    printf("CPU time used from filter_hm (per clock()): %.2f ms\n", durf);
    printf("CPU time used from filter_hm (per clock()) per value: %.6f ms\n", durf / VALUES);
    printf("Checking results of filter_hm...");
    for (int i = 0; i < fmin(50, VALUES); ++i)
    {
	memset(strBuff, 0, 10);
	snprintf(strBuff, 10, "%d", i);
	int *res = NULL;
	int err_get = get_hm(mapFilter, strBuff, (void **)&res);
	if (err_get)
	{
	    printf("HashMap error code: %d\n", err_get);
	    goto _test_failure;
	}
	if (res == NULL)
	{
	    printf("\nFunction get_hm returned NULL, but expected (%d) after filter_hm.\n", i + 1);
	    goto _test_failure;
	}
	if (*res != i)
	{
	    printf("\nFunction get_hm returned (%d), but expected (%d) after filter_hm.\n", *res, i + 1);
	    goto _test_failure;
	}
    }
    printf("done.\n");
    free_hash_map(mapFilter);

    puts("Testing: put action for identicle keys. Value must be replaced.");
    HashMap *identicleHM = NULL;
    new_hash_map(NULL, &identicleHM);
    for (int i = 0; i < 3; ++i)
    {
	memset(strBuff, 0, 10);
	snprintf(strBuff, 10, "%d", i);
	int *num = (int *)malloc(sizeof(int));
	*num = i;

	printf("\rFunction put_hm adding: %d", i);
	int err = put_hm(identicleHM, strBuff, num);
	if (err)
	{
	    printf("Function put_hm returned with Error: %d\n", err);
	    goto _test_failure;
	}
    }
    for (int i = 0; i < 3; ++i)
    {
	memset(strBuff, 0, 10);
	snprintf(strBuff, 10, "%d", i);
	int *num = (int *)malloc(sizeof(int));
	*num = i * 10;

	printf("\rFunction put_hm adding: %d", i);
	int err = put_hm(identicleHM, strBuff, num);
	if (err)
	{
	    printf("Function put_hm returned with Error: %d\n", err);
	    goto _test_failure;
	}
    }
    printf("\nChecking results for putting identicle keys (value replacement)...");
    for (int i = 0; i < 3; ++i)
    {
	memset(strBuff, 0, 10);
	snprintf(strBuff, 10, "%d", i);
	int *res = NULL;
	int err_get = get_hm(identicleHM, strBuff, (void **)&res);
	if (err_get)
	{
	    printf("HashMap error code: %d\n", err_get);
	    goto _test_failure;
	}
	if (*res != i * 10)
	{
	    printf("\nFunction get_hm returned (%d), but expected (%d).\n", *res, i * 10);
	    goto _test_failure;
	}
    }
    if (identicleHM->n_ent != 3)
    {
	printf("\nIncorrect elements count of %d in identicleHM. Expected 3.\n", identicleHM->n_ent);
	goto _test_failure;
    }
    free_hash_map(identicleHM);
    printf("done.\n");

    puts("Testing: value_destructor.");
    HashMap *destructorHM = NULL;
    new_hash_map(NULL, &destructorHM);
    int destAddErr = add_destructor_hm(destructorHM, hashMapValueDestructor);
    if (destAddErr)
    {
	printf("Failed to add value_destructor to HashMap. add_destructor_hm returned %d\n", destAddErr);
	goto _test_failure;
    }
    for (int i = 0; i < VALUES; ++i)
    {
	memset(strBuff, 0, 10);
	snprintf(strBuff, 10, "%d", i);
	int *num = (int *)malloc(sizeof(int));
	*num = i;

	int err = put_hm(destructorHM, strBuff, num);
	if (err)
	{
	    printf("Function put_hm returned with Error: %d\n", err);
	    goto _test_failure;
	}
    }
    int errDestRemove = remove_hm(destructorHM, "0");
    if (errDestRemove)
    {
	printf("Failed to remove value fromm HashMap. remove_hm returned %d\n", errDestRemove);
	goto _test_failure;
    }
    int filterDestErr = filter_hm(destructorHM, hashMapFilter);
    if (filterDestErr)
    {
	printf("Function filter_hm returned with Error: %d\n", filterDestErr);
	goto _test_failure;
    }
    if (destructorHM->n_ent != 49)
    {
	printf("Incorrect elements count after filtration. Expected 49, received %d\n", destructorHM->n_ent);
	goto _test_failure;
    }
    free_hash_map(destructorHM);

    puts(ANSI_COLOR_GREEN "Result: Success" ANSI_COLOR_RESET);
    puts("################## Test: HashMap ##################");
    return;
_test_failure:
    puts(ANSI_COLOR_RED "Result: Failure" ANSI_COLOR_RESET);
    puts("################## Test: HashMap ##################");
}
 */