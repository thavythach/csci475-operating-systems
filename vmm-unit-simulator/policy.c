#include "vmm.h"

uint replacement_policy(char policy){

	uint r;
	switch (policy){
		case CLOCK:
			return clock_pol();
		case FIFO: 
			r = fifo_pol();
			return r;           
		case LRU: 
			r=lru_pol();
			return r;
		case RANDOM:
		default:
			r = random_pol();
			return r;
	}

}

uint random_pol(){
	return (rand() % (FRAMES-1));
}

uint fifo_pol(){
	return pagetable[midnight->head->pg] & createMask(0,28);
}

uint lru_pol(){
	return pagetable[dusk->head->pg] & createMask(0,28);
}
uint clock_pol(){
	return 5;
}