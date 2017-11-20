# cat Makefile
all:	rtclock.o mmm.o main.c
		gcc -Wall -o mmm main.c mmm.o rtclock.o -lpthread

mmm.o: mmm.h mmm.c
		gcc -Wall -c mmm.c -lpthread

rtclock.o: rtclock.h rtclock.c
		gcc -Wall -c rtclock.c -lpthread

clean: 
		rm -f *.o mmm