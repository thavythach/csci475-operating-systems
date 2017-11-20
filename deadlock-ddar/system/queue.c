/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <xinu.h>
#include <stdlib.h>

/**
 * Prints out contents of a queue
 * @param q	pointer to a queue
 */
void	printqueue(struct queue *q)
{
	kprintf("[");
	struct qentry *tmp;
	for(tmp = q->head; tmp != NULL; tmp = tmp->next){
		kprintf("(pid=%d), (key=%d)", tmp->pid, tmp->key);
		if (tmp->next) kprintf(",");
	}
	kprintf("]\n");
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	isempty(struct queue *q)
{
	// true if q is not NULL and has no entries
	return (q && q->size == 0) ? TRUE : FALSE;
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	nonempty(struct queue *q)
{
	return !isempty(q) ? TRUE : FALSE;
}


/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	isfull(struct queue *q)
{
	return (q->size >= NPROC) ? TRUE : FALSE;

}


/**
 * Switch a target queue entry with the queue entry immediately preceding
 * it in the queue.
 * @param target the queue entry to move foward in the queue
 * @param q the queue which is being altered
 * @return the new qentry containg the pid in question if another switchup
 *  might be performed, otherwise NULL
 */
struct qentry *switchup(struct qentry *target) {
// if target is at the front of the queue ...
if (!target->prev) {
	// ... do nothing
	return NULL;
}
// if the entry preceding has greater or equal priority ...
else if (target->prev->key >= target->key) {
	// ... do nothing
	return NULL;
}
// if the entry preceding exists and has lower priority...
 else {
	// switch the two entries by switching their pids
	pid32 temp = target->prev->pid;
	target->prev->pid = target->pid;
	target->pid = temp;
	//... and switching their keys
	int32 tempkey = target->prev->key;
	target->prev->key = target->key;
	target->key = tempkey;

	return target->prev;
}
}

/**
 * Move a target queue entry up in the queue until reaches the front of the
 * queue or encounters an immediately preceding entry of equal or greater
 * priority.
 * @param target the queue entry to move forward in the queue
 * @param q the queue which is being altered
 */
void percolateup(struct qentry *target) {
	struct qentry *cur = target;
	// while we should switchup, do it
	while(cur) cur = switchup(cur);
}


/**
 * Insert a process at the tail of a queue
 * @param pid	ID process to insert
 * @param q	Pointer to the queue to use
 * @param key the effective priority of the process in the ready queue
 * @return pid on success, SYSERR otherwise
 */
pid32 enqueue(pid32 pid, struct queue *q, int32 key)
{
	// return error if invalid PID or queue is full
	if (isfull(q) || isbadpid(pid)) return SYSERR;

	// set the current prioirty of the process to the user argument value

	// set up a new node to put at the tail of the queue
	struct qentry *newTail = (struct qentry*) malloc(sizeof(struct qentry));
	newTail->pid = pid;
	newTail->next = NULL;
	newTail->key = key;

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

	percolateup(q->tail);

	return pid;
}


/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
pid32 dequeue(struct queue *q)
{
  // return EMPTY if queue is empty
  if (isempty(q)) {
		return EMPTY;
	}

	// get the head entry of the queue
	struct qentry *oldhead = q->head;
	pid32 oldPID = oldhead->pid;

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
  free(oldhead, sizeof(struct qentry));

	// return the pid on success
	return oldPID;
}


/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbypid(pid32 pid, struct queue *q)
{
	// return NULL if pid is invalid
	if (isbadpid(pid)) return NULL;

	// find the qentry with the given pid
	// cycle through the elements of the queue
	struct qentry *cur;
	for(cur = q->head; cur; cur = cur->next) {
		// until a match is found
		if(cur->pid == pid) return cur;
	}
	// pid wasn't found
	return NULL;

}

/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
pid32	getfirst(struct queue *q)
{
	// dequeue implements this functionality
	return dequeue(q);
}

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
pid32	getlast(struct queue *q)
{

  // return EMPTY if queue is empty
  if (isempty(q)) {
		return EMPTY;
	}

	// get the tail entry of the queue
	struct qentry *oldtail = q->tail;
	pid32 oldPID = oldtail->pid;

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
  	free(oldtail, sizeof(struct qentry));

	// return the pid on success
	return oldPID;

}



/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32	remove(pid32 pid, struct queue *q)
{
	// return EMPTY if queue is empty
	if (isempty(q)) return EMPTY;
	// return SYSERR if pid is illegal
	if (isbadpid(pid)) return SYSERR;

	// remove process identified by pid parameter from the queue
	// and return its pid

	// if the pid occurs at the front or end of the queue, remove it
	// using special rules we already implemented
	if(q->head->pid == pid) {
		return dequeue(q);
	} else if(q->tail->pid == pid) {
		return getlast(q);
	}

	// otherwise, go through and look for it in the middle

	// find the qentry with the given pid
	// cycle through the elements of the queue
	struct qentry *cur;
	for(cur = q->head; cur; cur = cur->next) {
		// until a match is found
		if(cur->pid == pid) {
			// update pointers around the element to be removed
			cur->next->prev = cur->prev;
			cur->prev->next = cur->next;
			// free up the node being removed and return pid
			free(cur, sizeof(struct qentry));
			return pid;
		}
	}
	// if pid does not exist in the queue, return SYSERR
	return SYSERR;
}
