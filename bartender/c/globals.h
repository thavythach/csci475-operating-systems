/*
 * globals.h
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

int num_threads;	// number of customer threads
int now_serving;	// customer's ID who is being served


//declare some semaphores
sem_t* s1; // trravel wait
sem_t* s2; // customer here
sem_t* s3; // bar empty
sem_t* s4; // make orde
sem_t* s5; // browse
sem_t* s6; // getdrink
sem_t* s7; // at register
sem_t* s8; // pay

#endif /* GLOBALS_H_ */