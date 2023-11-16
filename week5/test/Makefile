# Makefile for homework 4

# Name of output file
OUTPUT_CLIENT = client
OUTPUT_SERVER = server

# List file .c
SRC_CLIENT = client.c
SRC_SERVER = server.c

all: $(OUTPUT_CLIENT) $(OUTPUT_SERVER)

# Compiler
CC = gcc

# Flags for compiler
CFLAGS = -I./ -lm

# Rules for compiler
$(OUTPUT_CLIENT): $(SRC_CLIENT) 
	$(CC) -o $(OUTPUT_CLIENT) $(SRC_CLIENT) $(CFLAGS)
$(OUTPUT_SERVER): $(SRC_SERVER)
	$(CC) -o $(OUTPUT_SERVER) $(SRC_SERVER) $(CFLAGS)

# Delete files that are not in the main
clean:
	rm -f $(OUTPUT_CLIENT) $(OUTPUT_SERVER) 
