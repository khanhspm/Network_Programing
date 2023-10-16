# Makefile for homework 2

# Name of output file
OUTPUT = resolver

# List file .c
SRCS = main.c

# List file .h 
HEADERS = headers/domain.h headers/ip.h

# Compiler
CC = gcc

# Flags for compiler
CFLAGS = -I./ -lm

# Rules for compiler
$(OUTPUT): $(SRCS) $(HEADERS)
	$(CC) -o $(OUTPUT) $(SRCS) $(CFLAGS)

# Delete files that are not in the main
clean:
	rm -f $(OUTPUT)