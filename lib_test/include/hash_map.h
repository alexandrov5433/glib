#include "../../include/hash_map.h"

#ifndef TEST_HASH_MAP
#define TEST_HASH_MAP

void hashMapProcessor(const Entry *const ptr);
int hashMapFilter(const Entry *const ptr);
void hashMapValueDestructor(void *value);
void hashMapTest();

#endif