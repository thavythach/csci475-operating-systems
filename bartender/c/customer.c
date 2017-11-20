/*
 * customer.c
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
#include "customer.h"

/**
 * This is what the thread will call.
 * Do not touch.
 */
void* customer(void* args)
{
	unsigned int *custID = (unsigned int*) args;
	custTravelToBar(*custID);
	custArriveAtBar(*custID);
	custPlaceOrder();
	custBrowseArt();
	custAtRegister();
	custLeaveBar();
	return NULL;
}


/**
 * Each customer takes a random amount of time between 20 ms and 5000 ms to travel to the bar.
 */
void custTravelToBar(unsigned int custID)
{
	// synchronize
	double time = 20 + (rand() % 5000);
	usleep(time);

	sem_post(s1);

	printf("Cust %u\t\t\t\t\t\t\t\t\t\t\t|\n", custID);
}


/**
 * If there is already another customer in the bar the current customer has
 * to wait until bar is empty before entering.
 */
void custArriveAtBar(unsigned int custID)
{
	// synchronize

	sem_post(s2); // customer here

	sem_wait(s3); // bar empty

	now_serving = custID;


	printf("\t\tCust %u\t\t\t\t\t\t\t\t\t|\n", custID);
}


/**
 * The customer in the bar places an order
 */
void custPlaceOrder()
{
	// synchronize

	sem_wait(s4); // make order

	printf("\t\t\t\tCust %u\t\t\t\t\t\t\t|\n", now_serving);
}


/**
 * The customer in the bar can browse the wall art for a random amount of time between 3ms and 4000ms.
 */
void custBrowseArt()
{
	// synchronize

	sem_post(s5); // browse

	double time = 3 + (rand() % 4000);
	usleep(time);

	sem_post(s6); // getdrink


	printf("\t\t\t\t\t\tCust %u\t\t\t\t\t|\n", now_serving);
}


/**
 * If their drink is not ready by the time they are done admiring the art they must wait
 * until the bartender has finished. When the bartender is finished, the customer pays.
 *
 */
void custAtRegister()
{
	// synchronize

	sem_post(s7); // at register

	printf("\t\t\t\t\t\t\t\tCust %u\t\t\t|\n", now_serving);
}


/**
 * The customer in the bar leaves the bar.
 */
void custLeaveBar()
{
	// synchronize

	sem_wait(s8); // pay
	printf("\t\t\t\t\t\t\t\t\t\tCust %u\t|\n", now_serving);

}
