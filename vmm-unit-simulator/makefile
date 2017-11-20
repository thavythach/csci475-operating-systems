# cat Makefile
all:	queue.o policy.o instruction.o input.o vmm.o main.c
		gcc -Wall -o vmm main.c vmm.o input.o instruction.o policy.o queue.o -lm

vmm.o: vmm.h vmm.c
		gcc -Wall -c vmm.c -lm

input.o: vmm.h input.c
		gcc -Wall -c input.c -lm

instruction.o: vmm.h instruction.c
		gcc -Wall -c instruction.c -lm

policy.o: vmm.h policy.c
		gcc -Wall -c policy.c -lm

queue.o: vmm.h queue.c
		gcc -Wall -c queue.c -lm

clean: 
		rm -f *.o vmm