all:
	gcc -o dd_test dd.c dd_test.c
	gcc -o parse dd.c parse.c

test: all
	./dd_test

clean:
	rm dd_test parse
