CC = gcc
CFLAGS = -Wall -O3
LIB = -pthread -lm

all: tserver.o pserver.o lib.o
	$(CC) $(CFLAGS) tserver.o lib.o -o tserver $(LIB)
	$(CC) $(CFLAGS) pserver.o lib.o -o pserver $(LIB)

thread: tserver.o lib.o
	$(CC) $(CFLAGS) tserver.o lib.o -o tserver $(LIB)

process: pserver.o lib.o
	$(CC) $(CFLAGS) pserver.o lib.o -o pserver $(LIB)

tserver.o: server.c
	$(CC) $(LIB) -c -D THREAD server.c -o tserver.o

pserver.o: server.c 
	$(CC) $(LIB) -c -D PROCESS server.c -o pserver.o

lib.o: lib.c
	$(CC) -c lib.c

run:
	time ./pserver < ./testcases/input1.txt
	time ./tserver < ./testcases/input1.txt
	echo 1 | python3 main.py
	rm -rf *.out

	time ./pserver < ./testcases/input2.txt
	time ./tserver < ./testcases/input2.txt
	echo 2 | python3 main.py
	rm -rf *.out

	time ./pserver < ./testcases/input3.txt
	time ./tserver < ./testcases/input3.txt
	echo 3 | python3 main.py
	rm -rf *.out

	time ./pserver < ./testcases/input4.txt
	time ./tserver < ./testcases/input4.txt
	echo 4 | python3 main.py
	rm -rf *.out

	time ./pserver < ./testcases/input5.txt
	time ./tserver < ./testcases/input5.txt
	echo 5 | python3 main.py
	rm -rf *.out

clean:
	rm -rf *.out *.o *server
