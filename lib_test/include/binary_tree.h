#ifndef TEST_BINARY_TREE
#define TEST_BINARY_TREE

int comparator_bt(const void *const existing_value, const void *const new_value);

void processor_bt(void *const data);
void processor_bt_does_nothing(void *const data);
int selector_bt_89(void *const data);
int selector_bt_5(void *const data);
int selector_bt_invalid(void *const data);
void test_binary_tree(void);

#endif