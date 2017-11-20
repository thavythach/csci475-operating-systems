/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <prototypes.h>

#define N 5	//number of philosophers and forks

// locks must be declared and initialized here
mutex_t *forks;

void 	init_phil(){
	forks = malloc(N * sizeof(mutex_t));

	int i;
	for (i=0; i<N; i++){
		forks[i] = FALSE;
	}
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

mutex_t printlock = FALSE;

void printMessage(char **message, int phil_id) {
	mutex_lock(&printlock);
	kprintf(message, phil_id);
	mutex_unlock(&printlock);
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
			bool8 res_l = !test_and_set(&forks[left]);
			bool8 res_r = !test_and_set(&forks[right]);
			
			// if acquired both forks, eat
			if (res_l && res_r)
				printMessage("Philosopher %d eating: nom nom nom\n", phil_id);
				eat();
			
			// if acquired either fork, release it
			if (res_l)
				mutex_unlock(&forks[left]);
				
			if (res_r)
				mutex_unlock(&forks[right]);
		}
	}
}

int	main(int argc, char **argv)
{
	init_phil();
	//do not change
	ready(create((void*) philosopher, INITSTK, 15, "Ph1", 1, 0), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph2", 1, 1), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph3", 1, 2), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph4", 1, 3), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph5", 1, 4), FALSE);

	return 0;
}
