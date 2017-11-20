
/** Libraries **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "rtclock.h"

/** const strings **/
#define SEQ_STRING "sequential"
#define PAR_STRING "parallel"

#define USAGE "Usage: ./mmm <mode> [num threads] <size>\n"
#define GEN_ERROR_1 "ERROR: Mode argument is required."
#define SEQ_ERROR_1 "ERROR: Sequential mode requires proper value."
#define SEQ_ERROR_2 "ERROR: Sequential mode arguments are incorrect."
#define SEQ_THREAD 1
#define PAR_ERROR_1 "ERROR: Parallel mode arguments are incorrect."
#define PAR_ERROR_2 "ERROR: Parallel mode requires [num threads] argument."
#define PAR_ERROR_3 "ERROR: Parallel mode requires proper value."
#define PAR_ERROR_4 "ERROR: Parallel mode requires at least one thread."

/** Function prototypes **/
void mmm(int threads, int size); /** matrix multiplication decision **/
double seq(int size); /** sequentialized MMM **/
double par(int threads, int size); /** parallelzied MMM **/
void initSquareMatrices(int size); /** init a,b,c, matrices **/
void calcSquareMatrix(int size); /** calculate "c" square matrix function **/
int alldigit(char argv[]); /** Helper Function: checks if char array in the args are actually numbers**/