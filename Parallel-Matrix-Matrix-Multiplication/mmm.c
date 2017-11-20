/**
  * mmm.c
  * @created 3/24/2017
  * @author Thavy Thach
  **/
#include "mmm.h"


typedef struct thread_args {
	int tid;
 	int size;
 	int begin;
 	int end;
} thread_args;

/** Global vars**/
int **A;// a declaration
int **B; // b declaration
int **C; // c = a+b declaration

int NUM_THREADS;
int psize;
double LARGEST_ERROR;

void* pcsm(void* args);



/**
  * matrix multiplication occurs
  **/
void mmm(int threads, int size){

	// accounts for zero thread edge case
	if (threads == 0){
		printf("%s\n%s\n", PAR_ERROR_4, USAGE);
		return;
	}

	NUM_THREADS = threads;
	psize = size;
	LARGEST_ERROR = 0.0;


	// TODO: there's a bug for accounting for zero 0 for sequential NEED: fix

	/** INITIALIZE SQUARE MATRICES **/
	initSquareMatrices(size); // A, B, and C

	printf("==========\n");
	printf("Mode: %s\n", (threads == 1) ? SEQ_STRING : PAR_STRING);
	printf("Thread Count: %d\n", threads);
	printf("Size: %d\n", size);
	printf("==========\n");

	if (threads == 1){		/** spawn one thread to complete one function **/
		double s = seq(size); 	

		printf("Sequential Time: %.6f sec\n", s);	
	} else { 		/** spawn two threads to complete both functions at the same time **/
		
		int i, N;
		double p = 0.0;
		N = 3;
		for (i=0; i < N + 1; i++){
			p += par(threads, size);
			if (i==0) p = 0.0;
		} 
		p /= N;
		double s = seq(size); 
		double speedup = s/p;

		printf("Sequential Time: %.6f sec\n", s);
		printf("Parallel Time: %.6f sec\n", p);
		printf("Speedup: %.6f sec\n", speedup);
		printf("Verifying... largest error between parallel and sequential matrix: %f\n", LARGEST_ERROR);
	}

	// free heap cols
	int i;
	for (i=0; i<size; i++){
		free(A[i]);
		free(B[i]);
		free(C[i]);
	}

	// free heap rows
	free(A);
	free(B);
	free(C);
}

/**
  * Sequentialized MMM
  **/
double seq(int size){

	/* start timer */
    double clkbegin, clkend, time_sequential;
    clkbegin = rtclock();

	calcSquareMatrix(size);

	clkend = rtclock();
	time_sequential = clkend-clkbegin;

	
	return time_sequential;
}

/**
  * Parallelize MMM
  **/
double par(int threads, int size){

    double clkbegin, clkend, time_threaded;
    clkbegin = rtclock();

    pthread_t par_threads[NUM_THREADS];
    struct thread_args args[NUM_THREADS];

    int i;
    for (i=0; i<NUM_THREADS;i++){
    	args[i].tid = i;
    	args[i].begin = i*(size)/NUM_THREADS+1;
    	args[i].end = (i+1)*(size)/NUM_THREADS;
    	args[i].size = size;

    	pthread_create(&par_threads[i], NULL, pcsm, &args[i]);
    }

	for (i=0; i< NUM_THREADS; i++){
    	// printf("REAPING THREAD MESSAGE: id = %d\n", args[i].tid);
    	pthread_join(par_threads[i], NULL);
    }
   
    clkend = rtclock();

    time_threaded = clkend-clkbegin;

	return time_threaded;
}


/**
  * init single matrix on the heap dynamically with random values from 0-99
  **/
void initSquareMatrices(int size){
	
	int i,j;
	
	/** allocate rows dynamically **/
	A = (int**) malloc(size * sizeof(int*));
	B = (int**) malloc(size * sizeof(int*)); 
	C = (int**) malloc(size * sizeof(int*)); 

	/** allocate cols dynamically **/
	for (i=0; i<size; i++){
		A[i] = (int *) malloc (size * sizeof(int));
		B[i] = (int *) malloc (size * sizeof(int));
		C[i] = (int *) malloc (size * sizeof(int));
	}

	/** randomize matrices a & b with values from 0-99 && populate C w/ all 0's **/
	for (i=0; i<size; i++){
		for (j=0; j<size;j++){
			A[i][j] = rand() % 100;
			B[i][j] = rand() % 100;
			C[i][j] = 0;
		}
	}
}


void* pcsm(void* args){

	struct thread_args *params = args;

	// printf("PCSM ENTERD - THREAD VAL @ %d.\n", params->tid);

	int i,j,k;
	for (i = params->begin-1; i <= params->end-1; i++){

		// everything is distributed well
		// printf("\t(%d) < %d\n", i, params->end);
		// if (i == params->end-1) printf("\tI complete.\n");

		for (j = 0; j < params->size; j++){

			// everything is distributed well
			// printf("\t\t[%d][%d] -- (%d) < %d\n", i, j, i, params->size);
			// if (i == params->size-1) printf("\t\tJ complete.\n");

			// printf("\t\t\tA[%d][%d] = %d\n",i,j, A[i][j]);
			// printf("\t\t\tB[%d][%d] = %d\n",i,j, B[i][j]);
			// printf("\t\t\tC[%d][%d] = %d\n",i,j, C[i][j]);

			int sum = 0;
			for (k=0; k < params->size; k++){

				// printf("\t\t\t\t[%d][%d] -- (%d) < %d\n", i, j, k, params->size);
				// if (i == params->size-1) printf("\t\t\t\tK complete.\n");

				// printf("\t\t\t\t\tBEFORE:\n");
				// before
				// printf("\t\t\t\t\tA[%d][%d] = %d\n",i,k, A[i][k]);
				// printf("\t\t\t\t\tB[%d][%d] = %d\n",k,j, B[k][j]);
				// printf("\t\t\t\t\tC[%d][%d] = %d\n",i,j, sum);

				// summing this shit up
				sum += A[i][k] * B[k][j];

				// printf("\n\t\t\t\t\tAFTER:\n");

				// after
				// printf("\t\t\t\t\tA[%d][%d] = %d\n",i,k, A[i][k]);
				// printf("\t\t\t\t\tB[%d][%d] = %d\n",k,j, B[k][j]);
				// printf("\t\t\t\t\tC[%d][%d] = %d\n",i,j, sum);
			}

			double error = abs(C[i][j]-sum);
			// printf("\t%d - %d\n", C[i][j], sum);

			LARGEST_ERROR = (LARGEST_ERROR > error) ? LARGEST_ERROR : error;

			C[i][j] = sum;

			// printf("\t\t\tA[%d][%d] = %d\n",i,j, A[i][j]);
			// printf("\t\t\tB[%d][%d] = %d\n",i,j, B[i][j]);
			// printf("\t\t\tC[%d][%d] = %d\n",i,j, C[i][j]);

		}

		
		// printf("Exiting pcsm.\n");
	}	

	pthread_exit(0);
}

/**
  * calculates square matrix generally
  **/	
void calcSquareMatrix(int size){

	int i,j,k;

	for (i=0; i<size;i++){
		for (j=0;j<size;j++){
			// printf("Start: %d\n",C[i][j]);
			int sum = 0;
			for (k=0;k<size;k++){
				sum += A[i][j] * B[k][j];
				// printf("\t%d * %d --> %d\n",A[i][j], B[k][j], sum);
			}
			C[i][j] = sum;
			// printf("End: %d\n",C[i][j]);
		}
	}
}


/**
  * Helper Function: checks if string is all digits.
  * @return int 1 if true, 0 if at least one character is not a digit
  **/
int alldigit(char args[]){
	
	int i;

	// traverse through char array (string)
	for (i=0; args[i] != '\0'; i++)
		if(isdigit(args[i]) == 0) return 0;

	return 1;
}

