/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <test.h>

mutex_t lock = FALSE;

// Prof Chiu provided five test functions to run
int chiu_test_counter = 2;

void    printchar(char c)
{
    mutex_lock(&lock);
    int i;
    for (i=0; i<10; i++)
        kprintf("%c", c);
    mutex_unlock(&lock);
    chiu_test_counter --;
}

int wait_tests()
{
	while (chiu_test_counter > 0) resched();
	return runtests();
}

int main(uint32 argc, uint32 *argv)
{
    //priority of process is input as the 3rd argument of create()
    ready(create((void*) printchar, INITSTK, 15, "P1", 1, 'A'), FALSE);
    ready(create((void*) printchar, INITSTK, 15, "P2", 1, 'B'), FALSE);

    // run tests.. wait for David's tests to complete first
   	ready(create((int*) wait_tests, INITSTK, 0, "run tests", 2, 0, NULL), FALSE);

    return 0;
}
