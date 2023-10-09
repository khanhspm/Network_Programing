# Makefile for project

# Name of output file
OUTPUT = homework1

# List file .c
SRCS = main.c void.c

# List file .h 
HEADERS = liblist.h

# Compiler
CC = gcc

# Flags for compiler
CFLAGS = -I./ -lm

# Rules for compiler
$(OUTPUT): $(SRCS) $(HEADERS)
	$(CC) -o $(OUTPUT) $(SRCS) $(CFLAGS)

# Delete files that are not in the main
clean:
	rm -f $(OUTPUT) *.exe
