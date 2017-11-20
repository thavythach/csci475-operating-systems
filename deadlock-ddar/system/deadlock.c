#include <xinu.h>
#include <stdio.h>
#include <stdbool.h>

// processes are stored first in graph
int node_proc(int pid) {
	return pid;
}

// processes are stored in graph with indices lower than locks
int node_lock(int lockid) {
	return NPROC + lockid;
}

// is node a lock?
int is_lock(int node) {
	return (node >= NPROC);
}

int *get_el_ptr(int i, int j) {
int index = i * (NRAGNODE) + j;
  return &rag.graph[index];
}

void rag_request(int pid, int lockid){
	*get_el_ptr(node_proc(pid),node_lock(lockid)) = TRUE;
}

void rag_alloc(int pid, int lockid){
	*get_el_ptr(node_lock(lockid),node_proc(pid)) = TRUE;
	*get_el_ptr(node_proc(pid),node_lock(lockid)) = FALSE;
}

void rag_dealloc(int pid, int lockid){
	*get_el_ptr(node_lock(lockid),node_proc(pid)) = FALSE;
}

void rag_print(){

	int i,j;
	for (i=0; i<NRAGNODE; i++){
		for (j=0; j<NRAGNODE; j++){
			kprintf("%d ", *get_el_ptr(i,j));
		}
		kprintf("\n");
	}
}

/*
 * Return the next adjacent node in the RAG or -1 if no further adjacent
 * nodes can be found.
 */
int get_next_adjacent(int node, int start) {
	int i;
	for (i = start; i < NRAGNODE; i++) {
		if (*get_el_ptr(node,i)) return i;
	}
	return -1;
}


void print_node(int node) {
	if (node >= NPROC) {
		 kprintf(" lockid=%d ", node - NPROC);
	} else {
		kprintf(" pid=%d ", node);
	}
}

// perform actions on a deadlock node
void act_on_deadlock_node(int node) {

	print_node(node);
	// if node is a lock, record it in deadlockid and set deadlockflag
	// this might have already been done, but doing it again doesn't hurt
	if(is_lock(node)) {
		deadlockid = node - NPROC;
		deadlockflag = TRUE;
	}
	
}
// recursive depth-first-search
int recurse_on_node(int node, int *perm_marked, int *temp_marked, int *cycle_stack, int cstack_cur) {
	temp_marked[node] = TRUE;

	int i;
	int res = FALSE;

	for(i = get_next_adjacent(node, 0); i != -1; i = get_next_adjacent(node, i + 1)) {
		cycle_stack[cstack_cur] = node;
		if(!perm_marked[i] && !temp_marked[i]) {
			res |= recurse_on_node(i, perm_marked, temp_marked, cycle_stack, cstack_cur + 1);
		} else if (temp_marked[i]) {
			kprintf("DEADLOCK DETECTED");
			int j;
			// trace back deadlock path
			for(j = cstack_cur; cycle_stack[j] != i; j --) {
				act_on_deadlock_node(cycle_stack[j]);
			}
			act_on_deadlock_node(i);
			
			kprintf("\n");
			
			// we found a deadlock
			res = TRUE;
		}
	}

	temp_marked[node] = FALSE;
	perm_marked[node] = TRUE;

	return res;

}

// returns TRUE if cycle detected, otherwise FALSE
int deadlock_detect(void){

	// initialization
	int perm_marked[NRAGNODE];
	int temp_marked[NRAGNODE];
	int cycle_stack[NRAGNODE];
	int i;
	for (i=0; i < NRAGNODE; i ++){
		perm_marked[i] = FALSE;
		temp_marked[i] = FALSE;
	}

	int res = FALSE;

	// run cycle detection
	for (i=0; i < NRAGNODE; i ++){
		if (!perm_marked[i]) res |= recurse_on_node(i, perm_marked, temp_marked, cycle_stack, 0);
	}

	return res;

}

int deadlock_recover(void){
	 	

	// Grab the lockentry using a lockid that is involved in the deadlock,
	pid32 tmp_pid = locktab[deadlockid].wait_queue->head->pid;

	int i;
	// Was the victim process waiting on other locks? (Probably). Remove it from those locks' wait queues.
	for (i=0;i<NPROC;i++){
		// check to make sure that the addresses we will be dereferencing are
		// valid before dereferencing them
		if (locktab[i].wait_queue && (locktab[i].wait_queue->head) && (tmp_pid == locktab[i].wait_queue->head->pid)){
			// Call mutex_unlock() the lock's mutex member
			mutex_unlock(&(locktab[i].lock));
		}

		remove(tmp_pid, &(locktab[i].wait_queue));

	}

	//Find the ID of the process that currently holds this lock. Kill that process.
	kill(tmp_pid);

	// Update the RAG and zero out all the allocation and request edges associated with the victim process, as it has been vanquished from the system.
	
	for(i=0; i < NRAGNODE; i ++) {
		*get_el_ptr(node_proc(tmp_pid), i) = FALSE;
		*get_el_ptr(i, node_proc(tmp_pid)) = FALSE;
	}

	// Print out a message: "DEADLOCK RECOVER" followed by the victim's pid and lockid.
	kprintf("DEADLOCK RECOVER\t\t killing pid=%d to release lockid=%d\n", tmp_pid, deadlockid);
}
