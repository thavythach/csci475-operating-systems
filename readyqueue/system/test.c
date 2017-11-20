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
  res &= (enqueue(-1, q) == SYSERR);
  res &= (enqueue(NPROC, q) == SYSERR);

  // try filling the queue
  pid32 i;
  for(i = 0; i < NPROC; i ++) {
    res &= (i == enqueue(i, q));
  }

  // try filling past capacity
  res &= (enqueue(0, q) == SYSERR);

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
  enqueue(0, q);
  res &= (0 == getfirst(q));
  res &= isempty(q);
  enqueue(0, q);
  res &= (0 == getlast(q));
  res &= isempty(q);

  // fill the queue
  pid32 i;
  for(i = 0; i < NPROC; i ++) {
    res &= (i == enqueue(i, q));
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
    enqueue(i, q);
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
    enqueue(i, q);
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
    enqueue(i, q);
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

  kprintf("----------------------------\n");
  kprintf("Test battery status: %s.\n", pass ? "PASSED" : "FAILED");

  // restore proctab
  memcpy(&proctab, proctab_backup, sizeof(struct procent) * NPROC);

  return pass ? OK : NULL;
}
