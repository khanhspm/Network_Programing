# Makefile for homework 4

# Name of output file
OUTPUT_CLIENT = client
OUTPUT_SERVER = server

# List file .h 
HEADERS = TCP_Server/headers/liblist.h TCP_Server/headers/cmn_client.h

# List file .c
SRC_CLIENT = TCP_Client/client.c
SRC_SERVER = TCP_Server/server.c

all: $(OUTPUT_CLIENT) $(OUTPUT_SERVER)

# Compiler
CC = gcc

# Flags for compiler
CFLAGS = -I./ -lm

# Rules for compiler
$(OUTPUT_CLIENT): $(SRC_CLIENT) 
	$(CC) -o $(OUTPUT_CLIENT) $(SRC_CLIENT) $(CFLAGS)
$(OUTPUT_SERVER): $(SRC_SERVER) $(HEADERS)
	$(CC) -o $(OUTPUT_SERVER) $(SRC_SERVER) $(CFLAGS)

# Delete files that are not in the main
clean:
	rm -f $(OUTPUT_CLIENT) $(OUTPUT_SERVER) 