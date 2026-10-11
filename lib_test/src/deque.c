#include "../include/deque.h"
#include "../../include/deque.h"
#include "../../include/gstring.h"
#include <assert.h>
#include <stdio.h>

typedef struct DequeTestItem
{
    int id;
    char label[8];
} DequeTestItem;

static size_t _destructor_call_count;
static void *_last_destroyed_payload;

static void _deque_test_destructor(void **payload)
{
    assert(payload != NULL);
    _destructor_call_count++;
    _last_destroyed_payload = *payload;
}

static void _test_new_deque()
{
    deque_t *deque = NULL;

    assert(new_deque(NULL) == GLX_ERR_NULL_ARGUMENT);
    assert(new_deque(&deque) == GLX_SUCCESS);
    assert(deque != NULL);
    assert(deque->start_node == NULL);
    assert(deque->end_node == NULL);
    assert(deque->destructor == NULL);
    assert(deque->length == 0);
    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);
}

static void _test_new_deque_d()
{
    deque_t *deque = NULL;

    assert(new_deque_d(NULL, &deque) == GLX_ERR_NULL_ARGUMENT);
    assert(deque == NULL);
    assert(new_deque_d(_deque_test_destructor, NULL) == GLX_ERR_NULL_ARGUMENT);
    assert(new_deque_d(NULL, NULL) == GLX_ERR_NULL_ARGUMENT);

    assert(new_deque_d(_deque_test_destructor, &deque) == GLX_SUCCESS);
    assert(deque != NULL);
    assert(deque->start_node == NULL);
    assert(deque->end_node == NULL);
    assert(deque->destructor != NULL);
    assert(deque->length == 0);
    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);
}

static void _test_free_deque()
{
    deque_t *deque = NULL;

    assert(free_deque(NULL) == GLX_ERR_NULL_ARGUMENT);
    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);

    assert(new_deque(&deque) == GLX_SUCCESS);
    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);

    int first_value = 1;
    int second_value = 2;
    DequeTestItem item = {3, "three"};

    assert(new_deque(&deque) == GLX_SUCCESS);
    assert(add_start_deque(deque, &first_value) == GLX_SUCCESS);
    assert(add_start_deque(deque, &second_value) == GLX_SUCCESS);
    assert(add_start_deque(deque, &item) == GLX_SUCCESS);
    assert(deque->length == 3);
    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);

    _destructor_call_count = 0;
    _last_destroyed_payload = NULL;
    assert(new_deque_d(_deque_test_destructor, &deque) == GLX_SUCCESS);
    assert(add_start_deque(deque, &first_value) == GLX_SUCCESS);
    assert(add_start_deque(deque, &item) == GLX_SUCCESS);
    assert(add_start_deque(deque, &second_value) == GLX_SUCCESS);
    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);
    assert(_destructor_call_count == 3);
    assert(_last_destroyed_payload == &first_value);
}

static void _test_add_start_deque()
{
    deque_t *deque = NULL;
    int first_value = 1;
    int second_value = 2;
    DequeTestItem item = {3, "three"};

    assert(add_start_deque(NULL, &first_value) == GLX_ERR_NULL_ARGUMENT);
    assert(new_deque(&deque) == GLX_SUCCESS);
    assert(add_start_deque(deque, NULL) == GLX_ERR_NULL_ARGUMENT);
    assert(add_start_deque(deque, &first_value) == GLX_SUCCESS);
    assert(add_start_deque(deque, &second_value) == GLX_SUCCESS);
    assert(add_start_deque(deque, &item) == GLX_SUCCESS);

    assert(deque->length == 3);
    assert(deque->start_node->payload == &item);
    assert(deque->start_node->previous == NULL);
    assert(deque->start_node->next->payload == &second_value);
    assert(deque->start_node->next->previous == deque->start_node);
    assert(deque->start_node->next->next->payload == &first_value);
    assert(deque->start_node->next->next->previous == deque->start_node->next);
    assert(deque->end_node == deque->start_node->next->next);
    assert(deque->end_node->next == NULL);
    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);
}

static void _test_add_end_deque()
{
    deque_t *deque = NULL;
    int value = 2;
    DequeTestItem first_item = {1, "one"};
    DequeTestItem last_item = {3, "three"};

    assert(add_end_deque(NULL, &value) == GLX_ERR_NULL_ARGUMENT);
    assert(new_deque(&deque) == GLX_SUCCESS);
    assert(add_end_deque(deque, NULL) == GLX_ERR_NULL_ARGUMENT);
    assert(add_end_deque(deque, &first_item) == GLX_SUCCESS);
    assert(add_end_deque(deque, &value) == GLX_SUCCESS);
    assert(add_end_deque(deque, &last_item) == GLX_SUCCESS);

    assert(deque->length == 3);
    assert(deque->start_node->previous == NULL);
    assert(deque->start_node->payload == &first_item);
    assert(deque->start_node->next == deque->end_node->previous);
    assert(deque->end_node->payload == &last_item);
    assert(deque->end_node->next == NULL);
    assert(deque->end_node->previous->payload == &value);
    assert(deque->end_node->previous->previous == deque->start_node);

    void *removed = NULL;
    assert(remove_end_deque(deque, &removed) == GLX_SUCCESS);
    assert(removed == &last_item);
    assert(deque->length == 2);
    assert(remove_end_deque(deque, &removed) == GLX_SUCCESS);
    assert(removed == &value);
    assert(remove_end_deque(deque, &removed) == GLX_SUCCESS);
    assert(removed == &first_item);
    assert(deque->length == 0);
    assert(deque->start_node == NULL);
    assert(deque->end_node == NULL);
    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);
}

static void _test_remove_start_deque()
{
    deque_t *deque = NULL;
    DequeTestItem first_item = {1, "one"};
    int value = 2;
    DequeTestItem last_item = {3, "three"};
    void *removed = NULL;

    assert(remove_start_deque(NULL, &removed) == GLX_ERR_NULL_ARGUMENT);
    assert(new_deque(&deque) == GLX_SUCCESS);
    assert(remove_start_deque(deque, &removed) == GLX_ERR_ZERO_LENGTH);

    assert(add_start_deque(deque, &first_item) == GLX_SUCCESS);
    assert(add_start_deque(deque, &value) == GLX_SUCCESS);
    assert(add_start_deque(deque, &last_item) == GLX_SUCCESS);
    assert(remove_start_deque(deque, &removed) == GLX_SUCCESS);
    assert(removed == &last_item);
    assert(deque->length == 2);
    assert(remove_start_deque(deque, NULL) == GLX_SUCCESS);
    assert(deque->length == 1);

    assert(deque->end_node == deque->start_node);
    assert(deque->end_node->next == NULL);
    assert(deque->end_node->previous == NULL);
    assert(deque->start_node->previous == NULL);
    assert(deque->start_node->next == NULL);

    assert(remove_start_deque(deque, &removed) == GLX_SUCCESS);
    assert(removed == &first_item);
    assert(deque->length == 0);
    assert(deque->start_node == NULL);
    assert(deque->end_node == NULL);
    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);

    _destructor_call_count = 0;
    _last_destroyed_payload = NULL;
    assert(new_deque_d(_deque_test_destructor, &deque) == GLX_SUCCESS);
    assert(add_start_deque(deque, &value) == GLX_SUCCESS);
    assert(remove_start_deque(deque, &removed) == GLX_SUCCESS);
    assert(removed == &value);
    assert(_destructor_call_count == 1);
    assert(_last_destroyed_payload == &value);
    assert(free_deque(&deque) == GLX_SUCCESS);
}

static void _test_remove_end_deque()
{
    deque_t *deque = NULL;
    int first_value = 1;
    DequeTestItem item = {2, "two"};
    int last_value = 3;
    void *removed = NULL;

    assert(remove_end_deque(NULL, &removed) == GLX_ERR_NULL_ARGUMENT);
    assert(new_deque(&deque) == GLX_SUCCESS);
    assert(remove_end_deque(deque, &removed) == GLX_ERR_ZERO_LENGTH);

    assert(add_end_deque(deque, &first_value) == GLX_SUCCESS);
    assert(add_end_deque(deque, &item) == GLX_SUCCESS);
    assert(add_end_deque(deque, &last_value) == GLX_SUCCESS);
    assert(remove_end_deque(deque, &removed) == GLX_SUCCESS);
    assert(removed == &last_value);
    assert(deque->length == 2);
    assert(remove_end_deque(deque, NULL) == GLX_SUCCESS);
    assert(deque->length == 1);

    assert(deque->end_node == deque->start_node);
    assert(deque->end_node->next == NULL);
    assert(deque->end_node->previous == NULL);
    assert(deque->start_node->previous == NULL);
    assert(deque->start_node->next == NULL);

    assert(remove_end_deque(deque, &removed) == GLX_SUCCESS);
    assert(removed == &first_value);
    assert(deque->length == 0);
    assert(deque->start_node == NULL);
    assert(deque->end_node == NULL);

    assert(remove_end_deque(deque, &removed) == GLX_ERR_ZERO_LENGTH);
    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);

    _destructor_call_count = 0;
    _last_destroyed_payload = NULL;
    assert(new_deque_d(_deque_test_destructor, &deque) == GLX_SUCCESS);
    assert(add_end_deque(deque, &item) == GLX_SUCCESS);
    assert(remove_end_deque(deque, &removed) == GLX_SUCCESS);
    assert(removed == &item);
    assert(_destructor_call_count == 1);
    assert(_last_destroyed_payload == &item);
    assert(free_deque(&deque) == GLX_SUCCESS);
}

static void _test_peek_start_deque()
{
    deque_t *deque = NULL;
    int first_value = 17;
    DequeTestItem last_item = {42, "answer"};
    void *output = NULL;

    assert(peek_start_deque(NULL, &output) == GLX_ERR_NULL_ARGUMENT);
    assert(peek_start_deque(NULL, NULL) == GLX_ERR_NULL_ARGUMENT);
    assert(new_deque(&deque) == GLX_SUCCESS);
    assert(peek_start_deque(deque, &output) == GLX_ERR_ZERO_LENGTH);
    assert(peek_start_deque(deque, NULL) == GLX_ERR_NULL_ARGUMENT);

    deque_t missing_start = {NULL, NULL, NULL, 1};
    assert(peek_start_deque(&missing_start, &output) == GLX_ERR_NULL_NODE);

    assert(add_end_deque(deque, &first_value) == GLX_SUCCESS);
    assert(add_end_deque(deque, &last_item) == GLX_SUCCESS);
    deque_node_t *original_start = deque->start_node;
    deque_node_t *original_end = deque->end_node;
    size_t original_length = deque->length;

    assert(peek_start_deque(deque, &output) == GLX_SUCCESS);
    assert(output == &first_value);
    assert(deque->start_node == original_start);
    assert(deque->end_node == original_end);
    assert(deque->length == original_length);

    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);
}

static void _test_peek_end_deque()
{
    deque_t *deque = NULL;
    int first_value = 17;
    DequeTestItem last_item = {42, "answer"};
    void *output = NULL;

    assert(peek_end_deque(NULL, &output) == GLX_ERR_NULL_ARGUMENT);
    assert(peek_end_deque(NULL, NULL) == GLX_ERR_NULL_ARGUMENT);
    assert(new_deque(&deque) == GLX_SUCCESS);
    assert(peek_end_deque(deque, &output) == GLX_ERR_ZERO_LENGTH);
    assert(peek_end_deque(deque, NULL) == GLX_ERR_NULL_ARGUMENT);

    deque_t missing_end = {NULL, NULL, NULL, 1};
    assert(peek_end_deque(&missing_end, &output) == GLX_ERR_NULL_NODE);

    assert(add_end_deque(deque, &first_value) == GLX_SUCCESS);
    assert(add_end_deque(deque, &last_item) == GLX_SUCCESS);
    deque_node_t *original_start = deque->start_node;
    deque_node_t *original_end = deque->end_node;
    size_t original_length = deque->length;

    assert(peek_end_deque(deque, &output) == GLX_SUCCESS);
    assert(output == &last_item);
    assert(deque->start_node == original_start);
    assert(deque->end_node == original_end);
    assert(deque->length == original_length);

    assert(free_deque(&deque) == GLX_SUCCESS);
    assert(deque == NULL);

}

void test_deque()
{
    puts("################## Test: Deque ##################");
    puts("Running tests...");

    _test_new_deque();
    _test_new_deque_d();
    _test_free_deque();
    _test_add_start_deque();
    _test_add_end_deque();
    _test_remove_start_deque();
    _test_remove_end_deque();
    _test_peek_start_deque();
    _test_peek_end_deque();

    puts(ANSI_COLOR_GREEN "All tests passed!" ANSI_COLOR_RESET);
    puts("################## Test: Deque ##################");
}
