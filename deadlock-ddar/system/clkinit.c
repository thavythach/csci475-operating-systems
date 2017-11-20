/* clkinit.c - clkinit */

#include <xinu.h>
#include <interrupt.h>
#include <clock.h>

uint32	clktime;		/* seconds since boot			*/
uint32	ctr1000 = 0;		/* milliseconds since boot		*/
uint32	preempt;		/* preemption counter			*/

/**
 * Initialize the clock at system start up
 */
void	clkinit(void)
{
	//  program the timer!
	// 	Use Counter 0, 16-bit binary counter, rate generator mod, read/write
	//  binary value  00 11 010 0  -> decimal value 52
	uint16 controlword_value = 52;
	outb(CLKCNTL, controlword_value);

	//  set initial value of the countdown!
	//	We want to set countdown in such a way that
	//	the timer goes off every 1ms
	//  decimal value 1193
	//  binary value 0000 0100 1010 1001
	uint16	countdown = 1193;

  // mask binary value 0000 0000 1111 1111
	// gets us the least significant 8 bits
	uint16 mask = 0xFF;

	// mask for least significant 8 bits
	uint16 initialcount_leastsig_value = countdown & mask;
	
	// move most siginifcant 8 bits to least signifcant 8 bits
	// mask for least signifcant 8 bits
	uint16 initialcount_mostsig_value = (countdown >> 8) & mask;
	

	//  Now program the initial value for countdown
	// 	must write in two operations
	outb(CLOCK0, initialcount_leastsig_value);	//write least significant byte of countdown
	outb(CLOCK0, initialcount_mostsig_value);	//write most significant byte of countdown


	// Set interrupt vector for clock to invoke clkint
	set_evec(IRQBASE, (uint32)clkint);

	preempt = QUANTUM;	// initial time quantum

	clktime = 0;		// start counting seconds

	return;
}
