CC = gcc
CFLAGS = -c -Wall

all: homework1

homework1: homework1.o
	${CC} homework1.o -o homework1

homework1.o: homework1.c
	${CC} ${CFLAGS} homework1.c

clean:
	rm -f *.o *~