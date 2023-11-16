#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "headers/hw4.h"

#define MAX_FILE_SIZE 4294967296 // 2^32 bytes
#define MAX_FILENAME_SIZE 256
#define BUFFER_SIZE 1024

/**
 * @function sendFile : Send a file to the server
 * 
 * @param socket : Socket to connect to server
 * @param file_path : Path of the file to send
 * 
 * @return : if file not found then break function
 */
void sendFile(int socket, char *file_path) {
    size_t size;
    
    size_t len_file_path = strlen(file_path);
    if(file_path[len_file_path - 1] == '\n'){
        file_path[len_file_path - 1] = '\0';
    }

    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        return;
    }else{
        fseek(file, 0, SEEK_END);  // seek to end of file
        size = ftell(file); // get size of file
        rewind(file); // seek to beginning of file

        char buf[BUFFER_SIZE];
        size_t total_bytes = 0;
        while (total_bytes < size) {
            size_t bytes_to_read = fread(buf, 1, BUFFER_SIZE, file);
            if (bytes_to_read <= 0) {
                perror("File read error");
                break;
            }
            send(socket, buf, bytes_to_read, 0);
            total_bytes += bytes_to_read;
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]){
    if(argc != 3) {
        printf("Usage: ./client <IP_Addr> <Port_Number>\n");
        exit(EXIT_FAILURE);
    }

    char *ip_addr = argv[1];
    int port_number = atoi(argv[2]);

    struct sockaddr_in server_addr;
    int client_socket;

    // Step 1: Construct socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Step 2: Specify server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    inet_pton(AF_INET, ip_addr, &server_addr.sin_addr);

    // Step 3: Request to connect server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

    // receive server welcome
    char welcome_msg[256];
    recv(client_socket, welcome_msg, sizeof(welcome_msg), 0);
    printf("%s\n", welcome_msg);

    char buffer[BUFFER_SIZE];

    //Step 4: Communicate with server
    while(1){
        memset(buffer, '\0', sizeof(buffer));
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("%s\n", buffer);

        while(1){
            printf("Enter message: ");
            memset(buffer, '\0', (strlen(buffer)+1));
            fgets(buffer, BUFFER_SIZE, stdin);

            // Check format
            if((strncmp(buffer, "UPLD ", 5) == 0) || (strlen(buffer) == 1)){
                break;
            }else{
                printf("You need to enter with format: UPLD file_name file_size\n");
            }            
        }

        // if empty string then close client
        if(strlen(buffer) == 1){
            break;
        }

        send(client_socket, buffer, strlen(buffer), 0);

        char *str[3];
        breakString(buffer, str);

        sendFile(client_socket, str[1]);

        // receive result upload file
        char result_msg[BUFFER_SIZE];
        recv(client_socket, result_msg, sizeof(result_msg), 0);
        printf("%s\n", result_msg);
    }

    // Step 5: Close socket
    close(client_socket);

    return 0;
}