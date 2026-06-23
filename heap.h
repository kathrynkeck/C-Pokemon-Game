#ifndef HEAP_H
#define HEAP_H

#define MAX_SIZE 100

struct heap_node
{
  int dist;
  int row;
  int col;
  int index;
};

class Heap
{
  private:
  void heapify_up(int index);
  void heapify_down(int index);
public:
  heap_node *arr[MAX_SIZE];
  int size;

  Heap();
  int heap_insert(heap_node *node);
  heap_node *heap_peek_min();
  heap_node *heap_remove_min();
  int heap_decrease_dist(heap_node *node, int dist);
};

#endif