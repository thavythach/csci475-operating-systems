/*
 * TheBar.c
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "customer.h"
#include "bartender.h"


void printBanner();
void init();
void cleanup();

typedef struct thread_args{
	int tid;
} thread_args;


/**
 * Main function
 */
int main(int argc, char** argv)
{
	printBanner();
	init();		//initialize semaphores
	
	num_threads = atoi(argv[1]);

	struct thread_args args[num_threads];

	pthread_t threads[num_threads+1];

	// fire off customer thread

 	int i;

	for (i=0;i<num_threads;i++){
		args[i].tid = i;

		pthread_create(&threads[i], NULL, customer, &args[i]);
	}

	// fire off bartender thread
	pthread_create(&threads[num_threads], NULL, bartender, NULL);

	// wait for all threads to finish

	for (i=0;i<=num_threads;i++){
		pthread_join(threads[i], NULL);
	}

	cleanup();	//cleanup and destroy semaphores
	return 0;
}


/**
 * Prints the activity banner.
 * Do not touch.
 */
void printBanner() {
	printf("Customer:\t\t\t\t\t\t\t\t\t\t| Employee:\n");
	printf("Traveling\tArrived\t\tOrdering\tBrowsing\tAt Register\tLeaving");
	printf("\t| Waiting\tMixing Drinks\tAt Register\tPayment Recv\n");
	printf("----------------------------------------------------------------------------------------+");
	printf("------------------------------------------------------------\n");
}


/**
 * Create and initialize semaphores
 */
void init()
{
	// unlink semaphores
	sem_unlink("/s1");
	sem_unlink("/s2");
	sem_unlink("/s3");
	sem_unlink("/s4");
	sem_unlink("/s5");
	sem_unlink("/s6");
	sem_unlink("/s7");
	sem_unlink("/s8");

	// create semaphores 
	s1 = sem_open("/s1", O_CREAT, 0600, 0);
	s2 = sem_open("/s2", O_CREAT, 0600, 0);
	s3 = sem_open("/s3", O_CREAT, 0600, 0);
	s4 = sem_open("/s4", O_CREAT, 0600, 0);
	s5 = sem_open("/s5", O_CREAT, 0600, 0);
	s6 = sem_open("/s6", O_CREAT, 0600, 0);
	s7 = sem_open("/s7", O_CREAT, 0600, 0);
	s8 = sem_open("/s8", O_CREAT, 0600, 0);

	// errors SEM_FAILED
	if (s1 == SEM_FAILED) perror("Semaphore initialization");
	if (s2 == SEM_FAILED) perror("Semaphore initialization");
	if (s3 == SEM_FAILED) perror("Semaphore initialization");
	if (s4 == SEM_FAILED) perror("Semaphore initialization");
	if (s5 == SEM_FAILED) perror("Semaphore initialization");
	if (s6 == SEM_FAILED) perror("Semaphore initialization");
	if (s7 == SEM_FAILED) perror("Semaphore initialization");
	if (s8 == SEM_FAILED) perror("Semaphore initialization");
}


/**
 * Cleanup and destroy semaphores
 */
void cleanup()
{
	//close semaphores
	sem_close(s1);
	sem_close(s2);
	sem_close(s3); 
	sem_close(s4); 
	sem_close(s5); 
	sem_close(s6); 
	sem_close(s7); 
	sem_close(s8); 

	// unlink semaphores
	sem_unlink("/s1");
	sem_unlink("/s2");
	sem_unlink("/s3");
	sem_unlink("/s4");
	sem_unlink("/s5");
	sem_unlink("/s6");
	sem_unlink("/s7");
	sem_unlink("/s8");
}