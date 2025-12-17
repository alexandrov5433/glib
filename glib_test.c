#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./include/linkedList.h"

int selector(void *name) {
    if (strcmp((char*)name, "Alex") == 0) {
        return 1;
    }
    return 0;
}

int main() {
    char nameAlex[5] = "Alex";
    char nameEmo[4] = "Emo";
    char nameNikol[6] = "Nikol";
    LinkedList *ll = newLinkedList();
    appendToLL(&nameAlex, ll);
    appendToLL(&nameEmo, ll);
    appendToLL(&nameNikol, ll);
    Node *node = findLL(selector, ll);
    printf("Name: %s\n", node->value);
    freeLinkedList(ll);
    return EXIT_SUCCESS;
}