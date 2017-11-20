/*
 * test.c
 *
 *  Created on: Jan 7, 2017
 *      Author: Matthew Moreno
 */

#include <test.h>
#include <xinu.h>
#include <stdlib.h>

/*
 * Run a unit test, which returns true if it passed, and print the results.
 *
 * @param funptr A pointer to the unit test to run.
 * @param name A string describing the unit test being run.
 * @return A boolean value that is true of the test passed, false otherwise.
 */
bool8 runtest(bool8 (*funptr)(), const char *name) {

  kprintf("Running test %s...\n", name);
  bool8 pass = (*funptr)();
  kprintf("Test %s status: %s.\n", name, pass ? "passed" : "failed");

  return(pass);

}

// unit test enqueue and dequeue
bool8 test_enqueue_dequeue() {
  bool8 res = TRUE;

  struct queue *q = newqueue();

  // try enquing bad pid
  res &= (enqueue(-1, q, 1) == SYSERR);
  res &= (enqueue(NPROC, q, 1) == SYSERR);

  // try filling the queue
  pid32 i;
  for(i = 0; i < NPROC; i ++) {
    res &= (i == enqueue(i, q, 1));
  }

  // try filling past capacity
  res &= (enqueue(0, q, 1) == SYSERR);

  // try emptying the queue
  for(i = 0; i < NPROC; i ++) {
    pid32 deq = dequeue(q);
    res &= (i == deq);
  }

  // try dequeueing on an empty queue
  res &= (EMPTY == dequeue(q));

  // clean up queue
  free(q, sizeof(struct queue));

  return res;
}

// unit test getfirst and getlast
bool8 test_getfirst_getlast() {
  bool8 res = TRUE;

  struct queue *q = newqueue();

  // try getfirst and getlast on queue with only one element
  enqueue(0, q, 1);
  res &= (0 == getfirst(q));
  res &= isempty(q);
  enqueue(0, q, 1);
  res &= (0 == getlast(q));
  res &= isempty(q);

  // fill the queue
  pid32 i;
  for(i = 0; i < NPROC; i ++) {
    res &= (i == enqueue(i, q, 1));
  }

  // getting first should give 0 then 1
  // getting last should give NPROC - then NPROC -2
  res &= (0 == getfirst(q));
  res &= (NPROC - 1 == getlast(q));
  res &= (1 == getfirst(q));
  res &= (NPROC - 2 == getlast(q));

  // clean up queue
  free(q, sizeof(struct queue));

  return res;
}

// unit test isempty, nonempty, isfull
bool8 test_isempty_nonempty_isfull() {
  bool8 res = TRUE;

  struct queue *q = newqueue();

  res &= !nonempty(q);

  pid32 i;
  for(i = 0; i < NPROC; i ++) {
    res &= !isfull(q);
    enqueue(i, q, 1);
    res &= !isempty(q);
    res &= nonempty(q);
  }

  res &= isfull(q);

  // clean up queue
  for(i = 0; i < NPROC; i ++) {
    dequeue(q);
  }
  free(q, sizeof(struct queue));

  return res;
}

// unit test getbypid
bool8 test_getbypid() {
  bool8 res = TRUE;

  struct queue *q = newqueue();

  pid32 i;
  for(i = 0; i < NPROC; i ++) {
    enqueue(i, q, 1);
  }

  struct qentry *qe;

  qe = getbypid(0, q);
  res &= (qe->pid == 0);

  qe = getbypid(NPROC - 1, q);
  res &= (qe->pid == NPROC - 1);

  qe = getbypid(NPROC/2, q);
  res &= (qe->pid == NPROC/2);

  qe = getbypid(NPROC, q);
  res &= (!qe);

  qe = getbypid(-1, q);
  res &= (!qe);

  // clean up queue
  for(i = 0; i < NPROC; i ++) {
    dequeue(q);
  }
  free(q, sizeof(struct queue));

  return res;
}

// unit test remove
bool8 test_remove() {
  bool8 res = TRUE;

  struct queue *q = newqueue();

  pid32 i;
  for(i = 0; i < NPROC - 1; i ++) {
    enqueue(i, q, 1);
  }

  res &= (0 == remove(0, q));



  res &= (NPROC/2 == remove(NPROC/2, q));

  res &= (NPROC - 2 == remove(NPROC - 2, q));

  res &= (SYSERR == remove(NPROC - 1, q));

  res &= (SYSERR == remove(NPROC, q));

  // clean up queue
  for(i = 1; i < NPROC-2; i ++) {
    // skip NPROC/2
    if (i == NPROC/2) continue;
    res &= (i == remove(i, q));

  }

  res &= (SYSERR == remove(0, q));

  res &= (SYSERR == remove(1, q));



  free(q, sizeof(struct queue));

  return res;
}

// unit test priority sort
bool8 test_prioritysort() {
  bool8 res = TRUE;

  struct queue *q = newqueue();


  // enqueue some processes in a non-sorted priority
  // where pid is priority
  pid32 i = NPROC - 1;
  enqueue(i, q, NPROC - 1);
  for(i = (i - 27 + NPROC) % NPROC; i != NPROC - 1; i = (i - 27 + NPROC) % NPROC)  {
    enqueue(i, q, i);
  }

  // clean up queue
  // make sure that they come off sorted by pid from highest to lowest
  // because priority was set equal to pid
  int last = NPROC;
  pid32 cur = EMPTY;
  do {
    cur = dequeue(q);
    res &= (cur == EMPTY || cur <= last);
    last = cur;
  } while(cur != EMPTY);

  free(q, sizeof(struct queue));

  return res;
}


/* AGING TEST */

bool8 medpri_complete = FALSE;
bool8 lowpri_complete = FALSE;
bool8 highpri_complete = FALSE;
bool8 highpri_success = FALSE;

/* this won't get run in time without AGING */
void lowpri() {
  lowpri_complete = TRUE;
  return;
}

// wait for the low priority task to complete until timeout
void medpri(int maxtries) {

  ready(create((void*) lowpri, INITSTK, 1, "low priority test", 1, maxtries), FALSE);
  int try;
  for (try = 0; try < maxtries; try ++) {
    if (lowpri_complete) {
      medpri_complete = TRUE;
      break;
    } else {
      resched();
    }
  }
  return;
}

// wait for the medium prioiryt task to complete until timeout
void highpri(int maxtries) {

  ready(create((void*) medpri, INITSTK, 2, "medium priority test", 1, maxtries), FALSE);
  int try;
  for (try = 0; try < maxtries; try ++) {
    if (medpri_complete) {
      highpri_success = TRUE;
      break;
    }
    else {
      resched();
    }
  }
  highpri_complete = TRUE;
  return;
}

// unit test priority sort
bool8 test_agingpolicy() {
  bool8 res = TRUE;

  int maxtries = 10000;
  ready(create((void*) highpri, INITSTK, 3, "high priority test", 1, maxtries), TRUE);

  // block to wait for high priority to complete
  while(!highpri_complete) resched();

  // highpri_success should be TRUE with AGING and FALSE without it
  res &= AGING ? highpri_success : !highpri_success;

  return res;
}


void instance_func(mutex_t *lock, int *shared_val, int instance_val, int nreps, bool8 *res, bool8 *done)
{
  *res &= TRUE;
  
  int i;
  for (i=0; i<nreps; i++) {
    mutex_lock(lock);
    *res &= (*shared_val == 0);
    *shared_val = instance_val;
    *res &= (*shared_val == instance_val);
    
    int j;
    for (j = 0; j < nreps; j ++) resched();
    
    *res &= (*shared_val == instance_val);
    *shared_val = 0;
    *res &= (*shared_val == 0);
    mutex_unlock(lock);
  }
  *done = TRUE;
}

// unit test locks
bool8 test_locks() {
  
  mutex_t lock = FALSE;
  bool8 res = TRUE;
  int shared_val = 0;
  int nreps = 20;

  int i;
  for (i=0; i<nreps; i++) {
    bool8 res1 = TRUE;
    bool8 res2 = TRUE;
    bool8 done1 = FALSE;
    bool8 done2 = FALSE;
    
    ready(create((void*) instance_func, INITSTK, 3, "test instance function 1", 6, &lock, &shared_val, 1, nreps, &res1, &done1), TRUE);
    ready(create((void*) instance_func, INITSTK, 3, "test instance function 2", 6, &lock, &shared_val, 2, nreps, &res2, &done2), TRUE);
    
    while (!(done1 && done2)) resched();
    
    res &= res1;
    res &= res2;
  }

  return res;
}

/*
 * Run all registered unit tests, log passults
 */
int runtests()
{
  // save proctab -- tests require modification to it
  struct procent proctab_backup[NPROC];
  memcpy(proctab_backup, &proctab, sizeof(struct procent) * NPROC);

  // set all values to PR_SLEEP
  pid32 j;
  for(j = 0; j < NPROC; j ++) {
    proctab[j].prstate = PR_SLEEP;
  }


  bool8 pass = TRUE;
  pass &= runtest(&test_enqueue_dequeue, "enqueue, dequeue");
  pass &= runtest(&test_getfirst_getlast, "getfirst, getlast");
  pass &= runtest(&test_isempty_nonempty_isfull, "isempty, nonempty, isfull");
  pass &= runtest(&test_getbypid, "getbypid");
  pass &= runtest(&test_remove, "remove");
  pass &= runtest(&test_prioritysort, "priority sort");

  // restore proctab
  memcpy(&proctab, proctab_backup, sizeof(struct procent) * NPROC);

  pass &= runtest(&test_agingpolicy, "aging policy");
  pass &= runtest(&test_locks, "mutex lock");
  kprintf("----------------------------\n");
  kprintf("Test battery status: %s.\n", pass ? "PASSED" : "FAILED");


  return pass ? OK : NULL;
}
