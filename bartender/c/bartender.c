/*
 * bartender.c
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "bartender.h"

/**
 * Code for bartender thread.
 * Do not touch.
 */
void* bartender(void* args)
{
	int i;
	for (i = 0; i<num_threads; i++)
	{
		waitForCustomer();
		makeDrink();
		receivePayment();
	}
	return NULL;
}

/**
 * Waits in a closet until a customer enters.
 */
void waitForCustomer()
{
	// synchronize)
	sem_wait(s1); // travel wait
	sem_wait(s2); // customer here
	sem_post(s3); // bar empty
	printf("\t\t\t\t\t\t\t\t\t\t\t| Bartender\n");
}


/**
 * When a customer places an order it takes the Bartender
 * a random amount of time between 5 ms and 1000 ms to make the drink.
 */
void makeDrink()
{
	sem_post(s4); // make order
	sem_wait(s5); // browse

	// synchronize
	double time = 5 + (rand() % 1000);
	usleep(time);
	
	sem_wait(s6); // getdrink

	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\tBartender\n");
}


/**
 * Gets payment from the correct customer
 */
void receivePayment()
{
	// synchronize

	sem_wait(s7); //at register
	//at the register waiting for customer to pay
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\tBartender\n");

	sem_post(s8); // pay
	//got the payment from the right customer!
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\t\t\tBartender\n");

}
