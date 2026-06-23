#include <cstdlib>
#include <cstdio>

#include "queue.h"

Queue::Queue()
{
    first = NULL;
    last = NULL;
    size = 0;
}

int Queue::queue_destroy()
{
    queue_item *tmp;

    while ((tmp = first))
    {
        first = first->next;
        free(tmp);
    }

    last = NULL;

    size = 0;

    return 0;
}

int Queue::enqueue(int val)
{
    queue_item *tmp = new queue_item();

    if (size == 0)
    {
        tmp->value = val;
        tmp->next = NULL;
        first = tmp;
        last = tmp;
        size++;
    }

    else
    {
        tmp->value = val;
        tmp->next = NULL;
        last->next = tmp;
        last = tmp;
        size++;
    }

    return 0;
}

int Queue::dequeue(int *val)
{
    queue_item *tmp;

    if (!first)
    {
        // stack is empty
        return -1;
    }

    tmp = first;
    *val = first->value;
    first = first->next;

    if (first == NULL)
    {
        last = NULL;
    }

    delete (tmp);

    size--;

    return 0;
}

int Queue::queue_view(int *val)
{
    if (!first)
    {
        return -1;
    }

    *val = first->value;

    return 0;
}

int Queue::queue_size(int *size)
{
    *size = this->size;

    return 0;
}

int Queue::pqueue_enqueue(Character *c, int val)
{
    queue_item *item = new queue_item();
    item->prio = c->priority;
    item->c = c;
    item->value = val;
    item->next = NULL;
    item->prev = NULL;
    if (size == 0)
    {
        first = item;
        last = item;
    }
    else if (val < first->value || (val == first->value && item->prio < first->prio))
    {
        item->next = first;
        first->prev = item;
        first = item;
    }
    else
    {
        queue_item *curr = first;

        while (curr->next != NULL && (curr->next->value < val || (curr->next->value == val && curr->next->prio <= item->prio)))
        {
            curr = curr->next;
        }

        item->next = curr->next;
        item->prev = curr;

        if (curr->next != NULL)
        {
            curr->next->prev = item;
        }
        else
        {
            last = item;
        }
        curr->next = item;
    }

    size++;

    return 0;
}

Character *Queue::pqueue_dequeue(int *time)
{

    if (size == 0)
        return NULL;

    queue_item *tmp = first;
    Character *c = tmp->c;
    *time = tmp->value;

    first = first->next;
    if (first == NULL)
    {
        last = NULL;
    }
    else
    {
        first->prev = NULL;
    }

    delete (tmp);
    size--;
    return c;
}

Character *Queue::pqueue_view(int *time)
{
    if (size == 0)
    {
        return NULL;
    }
    *time = first->value;
    return first->c;
}
int Queue::pqueue_size()
{
    return size;
}
