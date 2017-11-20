#include "vmm.h"

void populate_logs(){
	LOG2_PAGES = (PAGES == -1) ? -1 : log2(PAGES);
	LOG2_FRAMES = (FRAMES == -1) ? -1 : log2(FRAMES);
	LOG2_BLKSIZE = (BLK_SIZE == -1) ? -1 : log2(BLK_SIZE);

	// debug
	// printf("LOG2(PAGES): %i\n",LOG2_PAGES);
	// printf("LOG2(FRAMES): %i\n",LOG2_FRAMES);
	// printf("LOG2(BLKSIZE): %i\n",LOG2_BLKSIZE);
}

void p_i_rw(char op, uint v_addr){
	
	// printf("frametables[i]!!!!!!!\n");
	// HDprint(frametable[0]);
	// HDprint(frametable[1]);
	// HDprint(frametable[2]);
	// HDprint(frametable[3]);

	// vars 
	uint frame, page_number, p_addr, evicted_frame_index, evicted_page_index, segfault, pagefault;
	segfault = 0;
	pagefault = 0;
	evicted_page_index = -1;

	// GET PAGE NUMBER
	page_number = v_addr >> LOG2_BLKSIZE;

	uint offset = v_addr << (32-(LOG2_BLKSIZE)); // pte#of bits
	offset >>= (32-(LOG2_BLKSIZE));

	int rs;
	// for (rs=0; rs<FRAMES; rs++){
	// 	printf("rList: %i\n", randomList[rs]);
	// }

	// pg does not exist
	if (!(page_number >= 0 && page_number < PAGES)){
		segfault = 1;
	} 

	// pg exists && valid bit is valid (1)
	else if ((pagetable[page_number] >> 31) == 1){

		printf("LET ME KNOW WHAT'S UP\n");

		// optimize for dynamic bitmasking
		// frame is the frame number
		frame = pagetable[page_number] & createMask(0,28);
		frame <<= LOG2_BLKSIZE;
		p_addr = frame; 
	} 

	// pg exists && valid bit is invalid (0)
	else {
		// page fault
		pagefault = 1;

		// traverse through frames to find empty frame.
		int i;
		int frame_found = 0;
		for (i=0; i<FRAMES; i++){

			if (frametable[i] == FRAME_SIG_BIT){

				printf("do i get called too?\n");

				frametable[i] = 0x00000000; // used

				frametable[i] |= page_number; // frame table entry is now perfect

				pagetable[page_number] |= 0x80000000; // 100...

				if (op == WRITE) 
					pagetable[page_number] = 0xE0000000; // 111...
				else if (op == READ) 
					pagetable[page_number] = 0xA0000000; // 101...
				
				
				pagetable[page_number] |= i; // adds frame number to PTE

				frame_found = 1; // let things be set to found

				p_addr = i << LOG2_BLKSIZE;
				p_addr |= offset;

				switch (R_POLICY){
					case RANDOM:
						for (rs=0; rs<FRAMES; rs++){
							if (randomList[rs] == -1){
								randomList[rs] = page_number;
								break;
							} 
						}
						break;
					case FIFO:
						enqueue(i,midnight);
						break;

					case LRU:
						enqueue(i,dusk);
						break;

					case CLOCK:

						break;
				}

				// break from loop
				break;
			} 
		}

		// there was no empty frame
		if (!frame_found){

			printf("does this ever get called...\n");

			// pagefault happens
			pagefault = 1;

			// printf("before week replacement .... \n");
			// printqueue(midnight);
			// printf("before replacement .... \n");
			// printqueue(midnight);

			bool isEvictable = false;
			switch (R_POLICY){
				case RANDOM:
					// 1. use policy evicted frame to index into frame table
					evicted_frame_index = replacement_policy(RANDOM);

					// 2. extract the page-index from the frametable at index rand
					// TODO need to shift by some value to get rid of fbit possibly.
					evicted_page_index = frametable[evicted_frame_index];
					for (rs=0; rs<FRAMES; rs++){
						// printf("ffrList: %i vs. %i\n", randomList[rs], evicted_page_index);
						

						if ((randomList[rs] == evicted_page_index)){
							randomList[rs] = page_number;
							// printf("WHAT IS THIS: %i vs. %i\n", randomList[rs], evicted_page_index);
							isEvictable = true;
							break;	
						} 

						if ((rs == FRAMES-1 && !isEvictable) || page_number == evicted_page_index){
							evicted_frame_index = replacement_policy(R_POLICY);
							evicted_page_index = frametable[evicted_frame_index];
							rs=-1;
						}
					}
					break;

				case FIFO:
					evicted_page_index = replacement_policy(FIFO);
					evicted_frame_index = pagetable[evicted_page_index] & createMask(0,28);

					struct qentry *tmp;
					// if (q->head == NULL) printf("EMPTY!\n");
				 	for(tmp = midnight->head; tmp != NULL; tmp = tmp->next){

				 		if (tmp->pg == evicted_page_index){
				 			dequeue(midnight);
				 			enqueue(page_number, midnight);
				 			isEvictable = true;
				 			break;
				 		} 

				 		if ((!isEvictable) || page_number == evicted_page_index){
				 			evicted_page_index = replacement_policy(FIFO);
							evicted_frame_index = pagetable[evicted_page_index] & createMask(0,28);
							tmp = midnight->head;
				 		}

					}
					break;

				case LRU:
					evicted_page_index = replacement_policy(LRU);
					evicted_frame_index = pagetable[evicted_page_index] & createMask(0,28);

					struct qentry *tmmp;

					for (tmmp = dusk->head; tmmp!=NULL;tmmp=tmmp->next){
						
						if (tmmp->pg == evicted_page_index){
							dequeue(dusk);
							enqueue(page_number, dusk);
							isEvictable = true;
							break;
						}

						if (page_number == evicted_page_index){
							evicted_page_index = enqueue(removePage(page_number, dusk), dusk);
							// evicted_page_index = replacement_policy(LRU);
							evicted_frame_index = pagetable[evicted_page_index] & createMask(0,28);
						}

						if (!isEvictable){
							evicted_page_index = replacement_policy(LRU);
							evicted_frame_index = pagetable[evicted_page_index] & createMask(0,28);
							tmmp = dusk->head;
						}
					}
					break;

				case CLOCK:

					break;
			}

			// printf("after replacement.... \n");
			// printqueue(midnight);

			// 3. use the above page index to index into page table
			// 4. evict the PTE @ this index 
			pagetable[evicted_page_index] = 0x0000000;

			// 5. update pgtable @ page number w/ status bits
			if (op == WRITE)
				pagetable[page_number] = 0xE0000000;
			else if (op == READ)
				pagetable[page_number] = 0xA0000000;
			

			// 5.5  update pgtable @ page number w/ frame-table-index
			pagetable[page_number] |= evicted_frame_index;



			// 6. update frametable at index-rand with the new page number
			// 
			frametable[evicted_frame_index] = page_number;	


			p_addr = evicted_frame_index << LOG2_BLKSIZE;
			p_addr |= offset;
		} 

	}

	printf("\n");
	output_instruction(op, v_addr, p_addr, segfault, page_number, pagefault, evicted_page_index);
}


void p_i_pppf(char op){

	int i=0,v=0;

	if (op == PRINT_PAGE_TABLE){


		printf("Page\t|V,M,R,Frame\n");

		for (i=0; i<PAGES; i++){
			
			printf("%i\t|",i); // pages

			if ((pagetable[i] >> 31) == 1){
				v = 1;
				printf("%d,",v);

				// vmr
				if ((pagetable[i] & createMask(29,31)) == 0xE0000000)
					printf("%i,%i",1,1);
				else if ((pagetable[i] & createMask(2,31)) == 0xA0000000)
					printf("%i,%i",0,1);
				
				// frame 
				printf(",%i", pagetable[i] & createMask(0,28));
			} else {
				v=0;
				printf("%d,",v);
				printf("-,");
				printf("-,");
				printf("-");
			}

			printf("\n");
		}

	} else if (op == PRINT_FRAME_TABLE){
		
		printf("Frame\t|F,Page\n");

		for (i=0;i<FRAMES;i++){
			printf("%i\t|",i);
			if (frametable[i] != 0x80000000){
				printf("%i,%i\n",((frametable[i] & createMask(30,31)) == 1 ? 1 : 0), frametable[i] & createMask(0,5));
			} else {
				printf("1,-\n");
			}
		}

	}
}

uint createMask(uint a, uint b){
   uint r = 0;
   for (uint i=a; i<=b; i++)
       r |= 1 << i;

   return r;
}

void output_instruction(char op, uint v_addr, uint p_addr, uint segfault, uint page_number, uint pagefault, uint evicted_page_index){

	// is it read op?
	if (op == READ){
		printf("%sBEGIN_READ\n", KWHT);
		// if (!segfault) read_rate++;
		read_rate++;
	}

	// is it write op?
	else {
		printf("%sBEGIN_WRITE\n%s", KWHT, KMAG);
		// if (!segfault) write_rate++;
		write_rate++;
	}


	printf("\tvirt_addr=0x%.8x\n",v_addr);


	// debug
	// segfault = 1;
	// pagefault = 1;
	// evicted_page_index = 1;

	if (segfault){
		printf("\tSegmentation Fault: illegal_page=%i\n",page_number);
	} else {
		if (pagefault){
			printf("\tPage fault: page=%i\n", page_number);
			pagefault_rate++;
		}

		if (evicted_page_index != -1){
			// TODO what is writeout
			// if there was already page in there, did we overwrite it
			// if modified bit is 1 and the current page and you're replacing, then it is a write out
			printf("\tPage replacement: evicted_page=%i, writeout=%s\n", evicted_page_index, ((op == WRITE) ? "true" : "false"));
			replace_rate++;
		}

		printf("\tphys_addr=0x%.8x\n",p_addr);		
	}

	// is it read op?
	if (op == READ){
		printf("%sEND_READ\n", KWHT);
	}

	// is it write op?
	else if (op == WRITE){
		printf("%sEND_WRITE\n", KWHT);
	}

}



void HDprint(uint addr){
	printf("\t\thex: 0x%.8X\n",addr); // printing hex
	printf("\t\tdec: %i\n",addr); // printing in decimal
	showbits(addr);
	printf("\n");
}

void showbits(uint x){
    int i; 
    printf("\t\tbin: ");
    for(i=(sizeof(int)*8)-1; i>=0; i--)
        (x&(1u<<i))?putchar('1'):putchar('0');

	printf("\n");
}