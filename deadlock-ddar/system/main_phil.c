/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <prototypes.h>

#define N 5	//number of philosophers and forks

// locks must be declared and initialized here
lid32 printer_lock;
lid32 forks[N];

void 	init_locks(){
	int i;
	for (i=0; i<N; i++){
		forks[i] = lock_create();
	}
	printer_lock = lock_create();
}


/**
 * Delay for a random amount of time
 * @param alpha delay factor
 */
void	holdup(int32 alpha)
{
	long delay = rand() * alpha;
	while (delay-- > 0)
		;	//no op
}

/**
 * Eat for a random amount of time
 */
void	eat()
{
	holdup(10000);
}

/**
 * Think for a random amount of time
 */
void	think()
{
	holdup(1000);
}

void printMessage(char *message, int phil_id) {
	acquire(printer_lock);
	kprintf(message, phil_id);
	release(printer_lock);
}


/**
 * Philosopher's code
 * @param phil_id philosopher's id
 */
void	philosopher(uint32 phil_id)
{
	// Use srand(uint32) to seed the random number generator for each philosopher
	// (just input the philosopher's ID)
	srand(phil_id);
	
	// init to 0 and 4 for no compilation errors
	uint32 right = phil_id;				// right fork
	uint32 left = (phil_id + N - 1) % N;	// left fork

	while (TRUE)
	{
		uint32 choice = rand() % 10;
		//think 70% of the time
		if (choice < 7) {
			printMessage("Philosopher %d thinking: zzzzzZZZz\n", phil_id);
			think();
		}
		//eat 30% of the time
		else {
			// attempt to get both forks
		
		 	acquire(forks[left]);
			acquire(forks[right]);
			
			// if acquired both forks, eat
			
			printMessage("Philosopher %d eating: nom nom nom\n", phil_id);
			eat();
			
			// if acquired either fork, release it
			
			release(forks[left]);
			release(forks[right]);
			
		}
	}
}

int	main(int argc, char **argv)
{
	init_locks();
	//do not change
	ready(create((void*) philosopher, INITSTK, 15, "Ph1", 1, 0), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph2", 1, 1), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph3", 1, 2), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph4", 1, 3), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph5", 1, 4), FALSE);

	return 0;
}
