#ifndef TEST_DYNAMIC_ARRAY
#define TEST_DYNAMIC_ARRAY

typedef struct PersonDA
{
    char *name;
    int age;
} PersonDA;

void intProcessorDA(void *ptr);
int intFilterDA(void *ptr);
void dynamicArrayTest();

#endif