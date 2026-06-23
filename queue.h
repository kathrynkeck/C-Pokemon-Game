#ifndef QUEUE_H
#define QUEUE_H

#include "character.h"
#include "world.h"

struct queue_item
{
    int value;
    struct queue_item *next;
    struct queue_item *prev;
    int prio;
    Character *c;
    queue_item() : value(0), next(NULL), prev(NULL), prio(0), c(NULL) {}
};

class Queue
{
public:
    queue_item *first;
    queue_item *last;
    int size;
    Queue();
    int queue_destroy();
    int enqueue(int val);
    int dequeue(int *val);
    int queue_view(int *val);
    int queue_size(int *size);

    int pqueue_enqueue(Character *c, int val);
    Character *pqueue_dequeue(int *time);
    Character *pqueue_view(int *time);
    int pqueue_size();
};

#endif