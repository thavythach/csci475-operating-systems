#include "vmm.h"

void print_usage(){
	printf("Usage: vmm [-p num_pages] [-f num_frames] [-b block_size] [-r rep_policy] <inst_file>\n");
}

void input(int argc, char *argv[]){
	int option = 0;

	// TODO map filename to instruction file sometime
	FILENAME = argv[argc-1];

	PAGES = -1; FRAMES = -1; BLK_SIZE = -1; R_POLICY = RANDOM; 

	// TODO account for negatives values in readme or in program
	while ((option = getopt(argc, argv, "p:f:b:r:")) != -1){

		if (optarg == FILENAME){
			print_usage();
			exit(EXIT_FAILURE);
		}

		// split options
		// TODO account for things other than numbers
		switch(option){
			case 'p':
				PAGES = atoi(optarg);

				// num_pages must be a multiple of 2
				// TODO generate an error @ wrong input
				if (!(PAGES % 2 == 0))
						PAGES = 8;

				break;
			case 'f':
				FRAMES = atoi(optarg);

				// num_frames must be a multiple of 2
				// TODO generate an error @ wrong input
				if (!(FRAMES % 2 == 0))
						FRAMES = 4; // defaults to 4

				break;
			case 'b': 
				BLK_SIZE = atoi(optarg);

				// block_size must be a multiple of 2
				// TODO generate an error @ wrong input
				if (!(BLK_SIZE % 2 == 0))
					BLK_SIZE = 1024; // defaults to 1024

				break;
			case 'r':
				R_POLICY = optarg[0];

				// TODO optimize if time for code readability
				switch(R_POLICY){
					case RANDOM:
						printf("REPLACEMENT POLICY: %c\n", RANDOM);
						break;
					case FIFO: 
						printf("REPLACEMENT POLICY: %c\n", FIFO);
						break;
					case LRU:
						printf("REPLACEMENT POLICY: %c\n", LRU);
						break;
					case CLOCK:
						printf("REPLACEMENT POLICY: %c\n", CLOCK);
						break;
					default:
						printf("REPLACEMENT POLICY: %c\n", RANDOM);
						R_POLICY = RANDOM;
				}
				break;
			default: 
				print_usage();
				exit(EXIT_FAILURE);
		}

	}

	populate_logs();

	// TODO generate an error @ wrong
	if (!((LOG2_PAGES != -1) && (LOG2_BLKSIZE != -1) && ((LOG2_PAGES+LOG2_BLKSIZE) <= 32))){
		print_usage();
		exit(EXIT_FAILURE);
	}

	// debug
	printf("PAGES: %d\n", PAGES);
	printf("FRAMES: %d\n", FRAMES);
	printf("BLK_SIZE: %d\n", BLK_SIZE);
	printf("R_POLICY: %c\n", R_POLICY);
	printf("FILENAME: %s\n", FILENAME);
}

