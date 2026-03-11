#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../include/data_structures/linkedList.h"
#include "../include/linked_list.h"

int firstSelector(void *arg)
{
    if (strcmp((char *)arg, "first") == 0)
    {
        return 1;
    }
    return 0;
}

int thirdSelector(void *arg)
{
    if (strcmp((char *)arg, "third") == 0)
    {
        return 1;
    }
    return 0;
}

int nonExistendSelector(void *arg)
{
    if (strcmp((char *)arg, "nonExistent") == 0)
    {
        return 1;
    }
    return 0;
}

void linkedListTest()
{
    char first[6] = "first";
    char second[7] = "second";
    char third[6] = "third";
    char fourth[7] = "fourth";

    LinkedList *ll = newLinkedList();
    appendToLL(&first, ll);
    appendToLL(&second, ll);
    appendToLL(&third, ll);
    appendToLL(&fourth, ll);

    Node *firstNode = findLL(firstSelector, ll);
    Node *thirdNode = findLL(thirdSelector, ll);
    Node *nonExistentNode = findLL(nonExistendSelector, ll);

    if (firstNode == NULL)
    {
        puts("Node not found: firstNode.");
    }
    else
    {
        printf("firstNode: %s\n", firstNode->value);
    }

    if (thirdNode == NULL)
    {
        puts("Node not found: thirdNode.");
    }
    else
    {
        printf("thirdNode: %s\n", thirdNode->value);
    }

    if (nonExistentNode == NULL)
    {
        puts("Node not found: nonExistentNode.");
    }
    else
    {
        printf("firstNode: %s\n", nonExistentNode->value);
    }

    Node *removedFirstNode = removeNodeLL(firstSelector, ll);
    if (removedFirstNode == NULL)
    {
        puts("Could not remove first node.");
    }
    else
    {
        printf("removedFirstNode: %s\n", removedFirstNode->value);
    }

    freeLinkedList(ll);
}


