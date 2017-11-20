/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vmm.h"

/**
 * Prints out contents of a queue
 * @param q	pointer to a queue
 */
void printqueue(struct queue *q)
{
	// printf("[");
	struct qentry *tmp;
	int i=0;
	// if (q->head == NULL) printf("EMPTY!\n");
 	for(tmp = q->head; tmp != NULL; tmp = tmp->next){
		printf("%d - %i\n", i, tmp->pg);
		i++;
		// if (tmp->next) printf(",");
	}

	printf("\n\n");
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
int	isempty(struct queue *q)
{
	// true if q is not NULL and has no entries
	return (q && q->size == 0) ? TRUE : FALSE;
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
int	nonempty(struct queue *q)
{
	return !isempty(q) ? TRUE : FALSE;
}


/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
int	isfull(struct queue *q)
{
	return (q->size >= FRAMES) ? TRUE : FALSE;
}


/**
 * Insert a process at the tail of a queue
 * @param pid	ID process to insert
 * @param q	Pointer to the queue to use
 *
 * @return pid on success, SYSERR otherwise
 */
uint enqueue(uint pg, struct queue *q)
{
	// set up a new node to put at the tail of the queue
	struct qentry *newTail = (struct qentry*) malloc(sizeof(struct qentry));
	// strcpy(newTail->line,theLine);
	newTail->pg = pg;
	newTail->next = NULL;

	if (isempty(q)) {
		// case A: queue is empty
		q->head = newTail;
  } else {
		// case B: queue is nonempty
		struct qentry* oldTail = q->tail;
		newTail->prev = oldTail;
		oldTail->next = newTail;
	}

	// increment size of queue and assign new tail
	q->tail = newTail;
	q->size ++;

	return pg;
}


/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
uint dequeue(struct queue *q)
{
  // return EMPTY if queue is empty
  if (isempty(q)) {
		return NULL;
	}

	// get the head entry of the queue
	struct qentry *oldhead = q->head;
	uint pg = oldhead->pg;

	if (q->size == 1) {
		// case A: only one node in queue
		q->head = NULL;
		q->tail = NULL;
	} else {
		// case B: more than one node in queue
		struct qentry *newhead = oldhead->next;
		newhead->prev = NULL;
		q->head=newhead;
	}

	// in both cases, decrement size of queue by one
	// upon successful removal
	q->size --;

  // free up the space on the heap
  free(oldhead);

	// return the pid on success
	return pg;
}


/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
uint getfirst(struct queue *q)
{
	// dequeue implements this functionality
	return dequeue(q);
}

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
uint getlast(struct queue *q)
{

  // return EMPTY if queue is empty
 //  if (isempty(q)) {
	// 	return EMPTY;
	// }

	// get the tail entry of the queue
	struct qentry *oldtail = q->tail;
	uint pg = oldtail->pg;

	if (q->size == 1) {
		// case A: only one node in queue
		q->head = NULL;
		q->tail = NULL;
	} else {
		// case B: more than one node in queue
		struct qentry *newtail = oldtail->prev;
		newtail->next = NULL;
		q->tail=newtail;
	}

	// in both cases, decrement size of queue by one
	// upon successful removal
	q->size --;

  // free up the space on the heap
  	free(oldtail);

	// return the pid on success
	return pg;

}

/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
uint removePage(uint pg, struct queue *q)
{
	// return EMPTY if queue is empty
	// if (isempty(q)) return EMPTY;

	// remove process identified by pid parameter from the queue
	// and return its pid

	// if the pid occurs at the front or end of the queue, remove it
	// using special rules we already implemented
	if(q->head->pg == pg) {
		return dequeue(q);
	} else if(q->tail->pg == pg) {
		return getlast(q);
	}

	// otherwise, go through and look for it in the middle

	// find the qentry with the given pid
	// cycle through the elements of the queue
	struct qentry *cur;
	for(cur = q->head; cur; cur = cur->next) {
		// until a match is found
		if(cur->pg == pg) {
			// update pointers around the element to be removed
			cur->next->prev = cur->prev;
			cur->prev->next = cur->next;
			// free up the node being removed and return pid
			free(cur);
			return pg;
		}
	}

	// if pid does not exist in the queue, return SYSERR
	return -1;
}