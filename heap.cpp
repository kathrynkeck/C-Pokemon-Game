#include "heap.h"
#include <cstdlib>

void swap(heap_node **a, heap_node **b)
{
    heap_node *tmp = *a;
    *a = *b;
    *b = tmp;

    int i = (*a)->index;
    (*a)->index = (*b)->index;
    (*b)->index = i;
}

void Heap::heapify_up(int index)
{
    int parent;
    while (index > 0)
    {
        parent = (index - 1) / 2;
        if (arr[index]->dist >= arr[parent]->dist)
        {
            break;
        }
        swap(&arr[index], &arr[parent]);
        index = parent;
    }
}

void Heap::heapify_down(int index)
{
    int left;
    int right;
    int small;
    while (1)
    {
        left = (2 * index) + 1;
        right = (2 * index) + 2;
        small = index;
        if ((left < size) && (arr[left]->dist < arr[small]->dist))
        {
            small = left;
        }
        if ((right < size) && (arr[right]->dist < arr[small]->dist))
        {
            small = right;
        }
        if (small == index)
        {
            break;
        }
        swap(&arr[index], &arr[small]);
        index = small;
    }
}

Heap::Heap()
{
    size = 0;
}

int Heap::heap_insert(heap_node *node)
{
    if (size >= MAX_SIZE)
    {
        return -1;
    }
    arr[size] = node;
    node->index = size;
    size++;
    heapify_up(node->index);

    return 0;
}

heap_node *Heap::heap_peek_min()
{
    if (size == 0)
    {
        return NULL;
    }
    return arr[0];
}

heap_node *Heap::heap_remove_min()
{
    if (size == 0)
    {
        return NULL;
    }

    heap_node *min = arr[0];

    size--;
    if (size > 0)
    {
        arr[0] = arr[size];
        arr[0]->index = 0;
        heapify_down(0);
    }

    return min;
}

int Heap::heap_decrease_dist(heap_node *node, int dist)
{
    if (dist <= node->dist)
    {
        return -1;
    }

    node->dist = dist;
    heapify_up(node->index);
    return 0;
}
