#include <stdlib.h>
#include "../include/data_structures/queue.h"

Queue *newQueue()
{
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL)
    {
        return NULL;
    }
    LinkedList *ll = newLinkedList();
    if (ll == NULL)
    {
        free(queue);
        return;
    }

    queue->list = ll;
    return queue;
}

int enqueue(Queue *queue, void *const ptr)
{
    return appendToLL(ptr, queue->list);
}

int dequeue(Queue *queue)
{
    
}