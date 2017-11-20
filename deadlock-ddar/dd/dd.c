/**
  * Deadlock Detection (off-xinu)
  *
  **/
#include <stdio.h>
#include <stdbool.h>
#include "dd.h"

int TRUE = true;
int FALSE = false;

// processes are stored first in graph
int node_proc(int pid) {
	return pid;
}

// processes are stored in graph with indices lower than locks
int node_lock(int lockid) {
	return NPROC + lockid;
}

void rag_request(int pid, int lockid){
	rag[node_proc(pid)][node_lock(lockid)] = TRUE;
}

void rag_alloc(int pid, int lockid){
	rag[node_lock(lockid)][node_proc(pid)] = TRUE;
	rag[node_proc(pid)][node_lock(lockid)] = FALSE;
}

void rag_dealloc(int pid, int lockid){
	rag[node_lock(lockid)][node_proc(pid)] = FALSE;
}

void rag_print(){

	int i,j;
	for (i=0; i<NRAGNODE; i++){
		for (j=0; j<NRAGNODE; j++){
			printf("%d ", rag[i][j]);
		}
		printf("\n");
	}
}

/*
 * Return the next adjacent node in the RAG or -1 if no further adjacent
 * nodes can be found.
 */
int get_next_adjacent(int node, int start) {
	int i;
	for (i = start; i < NRAGNODE; i++) {
		if (rag[node][i]) return i;
	}
	return -1;
}


void print_node(int node) {
	if (node >= NPROC) {
		 printf(" lockid=%d ", node - NPROC);
	} else {
		printf(" pid=%d ", node);
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
			printf("DEADLOCK");
			int j;
			for(j = cstack_cur; cycle_stack[j] != i; j --) {
				print_node(cycle_stack[j]);
			}
			print_node(i);
			printf("\n");
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
