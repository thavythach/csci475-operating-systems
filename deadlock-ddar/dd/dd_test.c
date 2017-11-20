#include <stdio.h>
#include <stdbool.h>

#include "dd.h"

/**
  * main method for testing deadlock detection
  **/
int main(){
	
  int res = true;
  
  res &= !deadlock_detect();
  
  rag_request(1,1);
  res &= !deadlock_detect();

  
  rag_alloc(1,1);
  res &= !deadlock_detect();


  rag_request(2,2);
  res &= !deadlock_detect();
  rag_alloc(2,2);
  res &= !deadlock_detect();


  rag_request(2,1);
  res &= !deadlock_detect();


  rag_request(1,2);
  res &= deadlock_detect();

  rag_request(3,3);
  res &= deadlock_detect();
  rag_alloc(3,3);
  res &= deadlock_detect();

  rag_dealloc(2,2);
  res &= !deadlock_detect();

  printf("TEST %s\n", res ? "passed" : "failed");

	return 0;
}

void print_test() {
  // init
  int i,j;
  for (i=0; i < NRAGNODE; i++){
    for (j=0; j < NRAGNODE; j++){
      rag[i][j] = 0;
    }
  }

  rag_print();

  printf("\n\n");

  rag_request(1,1);
  rag_print();

  printf("\n\n");

  rag_alloc(1,1);
  rag_print();

  printf("\n\n");

  rag_request(0,1);
  rag_print();

  printf("\n\n");

  rag_dealloc(1,1);
  rag_print();

  printf("\n\n");

  rag_alloc(0,1);
  rag_print();

  printf("\n\n");

  rag_dealloc(0,1);
  rag_print();

  printf("\n\n");
}
