CC=g++
CFLAGS=-I. -std=c++11

all: AliasMethod main exec

AliasMethod:
	$(CC) $(CFLAGS) -c AliasMethod.cc	

main:
	$(CC) $(CFLAGS) -c main.cc

exec:
	$(CC) $(CFLAGS) -o dice AliasMethod.o main.o
	 
clean:
	rm *.o dice 
