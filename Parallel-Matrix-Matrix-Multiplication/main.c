#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mmm.h"

/**
  * Parallel Matrix-Matrix Multiplication Program that runs both: sequentially and by parallel.
  */
int main(int argc, char *argv[])
{

	if (argv[1] != NULL){

		// account for anything not p or s
		if (! ( strcmp("S", argv[1]) == 0 || strcmp("s", argv[1]) == 0 || strcmp("P", argv[1]) == 0 || strcmp("p", argv[1]) == 0) ){

			printf("%s\n%s\n", GEN_ERROR_1, USAGE);
			return 0; //terminate program
		}
		
		/** 
		first arg: S or s
		second arg: size 
		limits: can't have three+ arg
		**/
		while( (strcmp("S", argv[1]) == 0 || strcmp("s", argv[1]) == 0) ){

			// checks if args are maxed
			if (argv[3] != NULL){
				printf("%s\n%s\n", SEQ_ERROR_2, USAGE);
				return 0; // terminate program
			}

			// checks if second arg has valid value 			
			int digitArg = alldigit(argv[2]);			
			if (digitArg == 0){
				printf("%s\n%s\n", SEQ_ERROR_1, USAGE);
				return 0; // terminate program
			}

			// runs program sequentially using one thread && argv[2]
			mmm(SEQ_THREAD, atoi(argv[2]));
			return 0; // terminate program
		}

		/**
		first arg: P or p
		second arg: num threads
		third arg: size
		limts: can't have four+ args
		**/
		while( (strcmp("P", argv[1]) == 0 || strcmp("p", argv[1]) == 0) ){

			// check if you have all arguments
			if (argv[2] != NULL && argv[3] == NULL){
				printf("%s\n%s\n", PAR_ERROR_2, USAGE);
				return 0; // terminate program
			}

			// check if you have too many arguments 
			if (argv[4] != NULL){
				printf("%s\n%s\n", PAR_ERROR_1, USAGE);
				return 0; // terminate program
			}

			// check if second and third arg has valid values
			int digitArg2 = alldigit(argv[2]);
			int digitArg3 = alldigit(argv[3]);

			if (digitArg2 == 0 || digitArg3 == 0){
				printf("%s\n%s\n", PAR_ERROR_3, USAGE);
				return 0; // terminate program
			}

			mmm(atoi(argv[2]), atoi(argv[3]));
			return 0; // terminate program
		}

	} else {
		printf("%s",USAGE);		
	}

	return 0; // terminate program
}

