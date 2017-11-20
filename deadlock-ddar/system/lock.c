#include <xinu.h>

local	lid32	newlock(void);

/**
 * Create a new semaphore and return the ID to the caller
 * @return ID of the mutex to caller, or SYSERR
 */
lid32	lock_create()
{
	intmask	mask;		/* saved interrupt mask	*/
	lid32	lockid;		/* lock ID to return	*/

	mask = disable();

	if ((lockid = newlock()) == SYSERR)
	{
		restore(mask);
		return SYSERR;
	}

	restore(mask);
	return lockid;
}

/**
 * Allocate an unused lock and return its index
 * @return	ID of free lock
 */
local	lid32	newlock(void)
{
	static	lid32	nextlock = 0;	/* next lockid to try	*/
	lid32	lockid;			/* ID to return	*/
	int32	i;			/* iterate through # entries	*/

	// START

	//loop through each element in the lock table.
	for (i = 0; i < NLOCK; i ++) {
		// and find a lock that is free to use
		if(locktab[i].state == LOCK_FREE) {
				// set its state to used, and reset the mutex to FALSE
				locktab[i].state = LOCK_USED;
				// return its lockid
				return i;
		}

	}

	// if there is no such lock, return SYSERR
	return SYSERR;
	
	// END
}


/**
 * Delete a lock by releasing its table entry
 * @param ID of lock to release
 */
syscall	lock_delete(lid32 lockid)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	lockentry *lptr;	/* ptr to lock table entry	*/

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}

	lptr = &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	// START

	// reset lock's state to free and the mutex to FALSE
	lptr->state = LOCK_FREE;
	lptr->lock = FALSE;
	
	// remove all processes waiting on its queue, and send them to the ready queue
	pid32 cur;
	struct queue *q = lptr->wait_queue;
	for (cur = dequeue(q); cur != EMPTY; cur = dequeue(q)) {
		ready(cur, FALSE);
	}
	
	int i;
	for (i = 0; i < NRAGNODE; i ++) {
		*get_el_ptr(node_lock(lockid),i) = FALSE;
		*get_el_ptr(i,node_lock(lockid)) = FALSE;
	}

	// END

	resched();
	restore(mask);
	return OK;
}


/**
 * Acquires the given lock, or cause current process to wait until acquired
 * @param lockid	lock on which to wait
 * @return status
 */
syscall	acquire(lid32 lockid)
{
	intmask mask;			// saved interrupt mask
	struct	lockentry *lptr;	// ptr to sempahore table entry

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}

	lptr = &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	// START
	// enqueue the current process ID on the lock's wait queue
	enqueue(currpid, lptr->wait_queue, 0);
	rag_request(currpid, lockid);
	// END

	restore(mask);				//reenable interrupts

	// START
	// lock the mutex!
	mutex_lock(&(lptr->lock));
	// END

	mask = disable();			//disable interrupts

	// START
	// (RAG) - we reache this point. Must've gotten the lock! Transform request edge to allocation edge
	rag_alloc(currpid, lockid);
	// END

	restore(mask);				//reenable interrupts
	return OK;
}

/**
 * Unlock, releasing a process if one is waiting
 * @param lockid ID of lock to unlock
 * @return status
 */
syscall	release(lid32 lockid)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	lockentry *lptr;	/* ptr to lock table entry	*/

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}
	lptr= &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	//START
	// remove current process' ID from the lock's queue
	dequeue(lptr->wait_queue);

	// unlock the mutex
	mutex_unlock(&(lptr->lock));
	
	// (RAG) - remove allocation edge from RAG
	rag_dealloc(currpid, lockid);
	// END

	restore(mask);
	return OK;
}
