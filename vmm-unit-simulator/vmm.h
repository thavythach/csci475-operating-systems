// LIBRARIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>

// DEFINITIONS
#define RANDOM 'r'
#define FIFO 'f'
#define LRU 'l'
#define CLOCK 'c' 

uint random_pol();
uint fifo_pol();
uint lru_pol();
uint clock_pol();

uint *randomList;
struct queue *midnight;
struct queue *dusk;

// GLOBALS
int PAGES;
int FRAMES;
int BLK_SIZE;
char R_POLICY;
char *FILENAME;
char *FILEPATH; 

uint LOG2_PAGES;
uint LOG2_FRAMES;
uint LOG2_BLKSIZE;

// vmm.c
void populate_logs();
void p_i_rw(char op, uint addr);
void p_i_pppf(char op);
void HDprint(uint addr);

// input.c
void print_usage();
void input(int argc, char *argv[]);

// instruction.c
void instruction_file();
void tokenize_instruction(char *line);
char determineOP(char *op);
uint handle_address(char *addr);

#define READ 'R'
#define WRITE 'W'
#define PRINT_GENERAL 'P'
#define PRINT_PAGE_TABLE 'P'
#define PRINT_FRAME_TABLE 'F'

#define MAX_STR_LEN strlen("0x00000000")

// pagetable.c
uint *pagetable;
uint *frametable;

// bits 
#define v_check 2147483648 // and
#define m_check 1073741824 // and
#define r_check 536870912 // and
#define f_check 3758096384 // xor

// color codes
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define ARCHITECTURE 32

// replacement.c
uint replacement_policy(char policy);
uint find_frame();
void output_instruction(char op, uint vaddr, uint paddr, uint segfault, uint page_number, uint pagefault, uint evicted_page);
void showbits(unsigned int x);

#define SEGFAULT "ERROR: Segmentation Fault.\n"
#define BITMASK 0x0000FFFF
#define FRAME_SIG_BIT 0x80000000

uint createMask(uint a, uint b);

// amounts
double instruction_count;
double read_rate;
double write_rate;
double replace_rate;
double pagefault_rate;

/* queue.h */

#define	EMPTY NULL		/* null value for qnext or qprev index	*/
#define TRUE 1
#define FALSE 0

// define queue's data members
struct queue
{
	struct qentry *head;
	struct qentry *tail;
	int size;
};


// define a queue entry's data members
struct qentry
{
	uint pg;
	struct qentry *next;
	struct qentry *prev;
};


/* Queue function prototypes (don't touch!) */
void    printqueue(struct queue *q);

int	isempty(struct queue *q);
int	nonempty(struct queue *q);
int	isfull(struct queue *q);

uint 	getfirst(struct queue *q);
uint	getlast(struct queue *q);
uint	dequeue(struct queue *q);
uint 	enqueue(uint pg, struct queue *q);
struct queue *newqueue();

uint createMask(uint a, uint b);
uint removePage(uint pg, struct queue *q);