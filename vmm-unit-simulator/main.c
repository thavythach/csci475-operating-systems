#include "vmm.h"
#include "queue.h"

int main(int argc, char *argv[]){

	instruction_count = 0;
	read_rate = 0;
	write_rate = 0;
	pagefault_rate = 0;
	replace_rate = 0;

	// FILENAME = "swag.txt";
	input(argc, argv);
	pagetable = (uint *) malloc (PAGES * sizeof(uint));
	frametable = (uint *) malloc (FRAMES * sizeof(uint));
	randomList = (uint *) malloc (FRAMES * sizeof(uint));
	
	midnight = (struct queue *) malloc (sizeof(struct queue));
	midnight->head = NULL;
	midnight->tail = NULL;
	midnight->size = 0;

	dusk = (struct queue *) malloc (sizeof(struct queue));
	dusk->head = NULL;
	dusk->tail = NULL;
	dusk->size = 0;

	int i;
	for (i=0; i<PAGES; i++){
		pagetable[i] = 0x00000000;
	}
	for (i=0; i<FRAMES; i++){
		frametable[i] = 0x80000000; 
		randomList[i] = -1;
	}
	instruction_file();

	printf("inst_count=%.0f ",instruction_count);
	printf("(read=%.2f%%, write=%.2f%%), ", (read_rate/instruction_count)*100, (write_rate/instruction_count)*100);
	printf("page_faults=%.0f (%.2f%%), replacements=%.0f (%.2f%%)", pagefault_rate, ((pagefault_rate/PAGES)*100), replace_rate, ((replace_rate/PAGES)*100));

	free(frametable);
	free(pagetable);

	return 0;
}

