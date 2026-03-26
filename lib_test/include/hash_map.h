#include "../../include/hash_map.h"

#ifndef TEST_HASH_MAP
#define TEST_HASH_MAP

void hashMapProcessor(Entry *const ptr);
int hashMapFilter(Entry *const ptr);
void hashMapValueDestructor(const Entry *const entry);
void hashMapTest();

#endif