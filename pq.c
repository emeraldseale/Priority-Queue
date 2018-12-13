/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Emerald Seale
 * Email: sealee@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq_node { //helper struct, holds priority and value variables
  int priority;
  void* value;
};

struct pq {
  struct dynarray* heap;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* pq = malloc(sizeof(struct pq)); //create space for pq
  pq->heap = dynarray_create(); //create dynamic array
  return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  assert(pq);
  int i;
  struct pq_node* temp;
  for (i = 0; i < dynarray_size(pq->heap); i++) { //free nodes in array
    free(dynarray_get(pq->heap, i));
  }
  dynarray_free(pq->heap); //free array
  free(pq); //free pq struct
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  assert(pq);
  if (dynarray_size(pq->heap) == 0) { //check if array is empty
    return 1;
  } else {
    return 0;
  }
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void percolate_up(struct dynarray* heap, int index) {
  int parent_index = ((index - 1) / 2); //get parent index
  struct pq_node* temp; //set temp node for swap
  struct pq_node* curr = dynarray_get(heap, index); //get current node
  struct pq_node* par = dynarray_get(heap, parent_index); //get parent of current node
  if ((curr->priority < par->priority) && index != 0) { //keeps index from going out of bounds
    dynarray_set(heap, parent_index, curr); //swap current for parent
    dynarray_set(heap, index, par); //swap parent for current
    percolate_up(heap, parent_index); //recursive call passing parent index as current index
  }
}

void pq_insert(struct pq* pq, void* value, int priority) {
  assert(pq); //make sure that pq != NULL
  struct pq_node* node = malloc(sizeof(struct pq_node)); //create priority queue node
  node->priority = priority; //copy over values to new node
  node->value = value; //copy over values to new node
  dynarray_insert(pq->heap, -1, node); //insert new node into the back of the array
  percolate_up(pq->heap, (dynarray_size(pq->heap) - 1)); //maintain binary heap structure
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  assert(pq);
  struct pq_node* temp = dynarray_get(pq->heap, 0); //gets first element from array
  return temp->value;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  assert(pq);
  struct pq_node* temp = dynarray_get(pq->heap, 0); //gets first element from array
  return temp->priority;
}
 
 
/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void percolate_down(struct dynarray* heap, int index) {
  struct pq_node* temp;// set temp node for swap
   if (((index * 2) + 1) < dynarray_size(heap) && ((index * 2) + 2) < dynarray_size(heap)) { //keeps recursion in bounds of array
    int child_index; //set up int to get the index of whichever child is smallest
    struct pq_node* curr = dynarray_get(heap, index); //get current node
    struct pq_node* left_child = dynarray_get(heap, ((index * 2) + 1)); //get left child
    struct pq_node* right_child = dynarray_get(heap, ((index * 2) + 2)); //get right child
    struct pq_node* smallest_child; //get parent of current node
    if (left_child->priority < right_child->priority) { //check to see which child is smallest
      smallest_child = left_child;
      child_index = ((index * 2) + 1);
    } else {
      smallest_child = right_child;
      child_index = ((index * 2) + 2);
    }
    if ((curr->priority > smallest_child->priority)) { //if parent is bigger than child
      dynarray_set(heap, child_index, curr); //swap current for parent
      dynarray_set(heap, index, smallest_child); //swap parent for current
      percolate_down(heap, child_index); //recursive call passing child index as current index
    } 
  } else if (dynarray_size(heap) == 2){ //if down to two final elements, just swap them
    temp = dynarray_get(heap, 0);
    dynarray_set(heap, 0, dynarray_get(heap, 1));
    dynarray_set(heap, 1, temp);
  }
}

void* pq_remove_first(struct pq* pq) {
  assert(pq); //make sure pq != NULL
  void* temp = pq_first(pq); //copy value to a temp
  struct pq_node* temp_2 = dynarray_get(pq->heap, 0); //copy first pq_node in array to a temp
  dynarray_set(pq->heap, 0, dynarray_get(pq->heap, -1)); //replace first value with last value
  dynarray_remove(pq->heap, -1); //remove the last value
  percolate_down(pq->heap, 0); //maintain binary heap
  free(temp_2); //free temp_2 from the heap
  return temp;
}
