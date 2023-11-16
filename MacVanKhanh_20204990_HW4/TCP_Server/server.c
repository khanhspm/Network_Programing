#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "headers/hw4.h"

#define MAX_FILE_SIZE 4294967296 // 2^32 bytes
#define MAX_FILENAME_SIZE 256
#define BUFFER_SIZE 1024
#define BACKLOG 10 //Number of allowed connections

/**
 * @function saveFile: save the file to the folder storage
 * 
 * @param new_socket : socket that server listen from client
 * @param message : message from client
 * @param folder_storage : folder storage to save file
 */
void saveFile(int new_socket, char *message, char *folder_storage, char *stt){
    char target_path[BUFFER_SIZE] = "./";
    strcat(target_path, folder_storage);
    strcat(target_path, "/");

    char filename[MAX_FILENAME_SIZE];
    size_t size;
    char *str[3];
    breakString(message, str);
    strcpy(filename, str[1]);
    FILE *f_check = fopen(filename, "rb");

    if(f_check == NULL){
        strcpy(stt, "-ERR File Not Found");
        send(new_socket, "File Not Found", sizeof("File Not Found"), 0);
    }else{
        strcat(target_path, filename);
        filename[0] = '\0';
        size = strtol(str[2], NULL, 10);

        FILE *fp = fopen(target_path, "ab");
        if(fp == NULL){
            fp = fopen(target_path, "wb");
            if(fp == NULL){
                printf("File open error!!\n");
            }
        }   

        size_t total_bytes = 0;
        char buf[BUFFER_SIZE];

        while(total_bytes < size){
            size_t bytes_to_read = (size - total_bytes < BUFFER_SIZE) ? size - total_bytes : BUFFER_SIZE;
            size_t bytes_received = recv(new_socket, buf, bytes_to_read, 0);
            if(bytes_received <= 0){
                break;
            }
            fwrite(buf, 1, bytes_received, fp);
            buf[0] = '\0';
            if(total_bytes >= MAX_FILE_SIZE){
                break;
            }
            total_bytes += bytes_received;
        }

        fclose(fp);

        if(total_bytes != 0){
            strcpy(stt, "+OK Successful upload");
            send(new_socket, "Successful upload", sizeof("Successful upload"), 0);
        }else{
            strcpy(stt, "-ERR Failed upload");
            send(new_socket, "Failed upload", sizeof("Failed upload"), 0);
        }
    }

}

int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("Usage: ./server <Port_Number> <Folder_save>\n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]); //port number that will be opened
    char *folder_storage = argv[2]; // folder that will be saved sent file

    struct sockaddr_in server_addr, client_addr; // address information
    int server_socket, new_socket; // file descriptors
    socklen_t client_addr_len;
    int clientPort; // port number of client
    char clientIP[INET_ADDRSTRLEN];

    // Step 1: Construct a TCP socket to listen connections request
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    //Step 2: Bind address to socket
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY puts your IP address automatically
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding error");
        exit(EXIT_FAILURE);
    }

    //Step 3: Listen request from client
    if (listen(server_socket, BACKLOG) < 0) {
        perror("Listening error");
        exit(EXIT_FAILURE);
    }

    createFolerStorage(folder_storage);

    //Step 4: Communicate with client
    while(1){

        //accept request
        client_addr_len = sizeof(client_addr);
        new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

        if (new_socket < 0) {
            perror("Accept error");
            exit(EXIT_FAILURE);
        }

        inet_ntop(AF_INET, &client_addr.sin_addr, clientIP, sizeof(clientIP));
        clientPort = ntohs(client_addr.sin_port);
        char clientPortString[BUFFER_SIZE];
        sprintf(clientPortString, "%d", clientPort);

        // Send welcome message to the client
        send(new_socket, "Welcome to file server", sizeof("Welcome to file server"), 0);
        saveLog(clientIP, clientPortString, " ", "+OK Welcome to file server");

        while(1){
            send(new_socket, "Please send file", sizeof("Please send file"), 0);
            saveLog(clientIP, clientPortString, " ", "+OK Please send file");

            char buffer[BUFFER_SIZE];
            char input[BUFFER_SIZE];

            int recv_bytes = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
            if(recv_bytes <= 0){
                break;
            }

            buffer[recv_bytes] = '\0';
            strcpy(input, buffer);

            // Send the result upload file
            char status[BUFFER_SIZE];
            saveFile(new_socket, buffer, folder_storage, status);
            saveLog(clientIP, clientPortString, input, status);

        }

    }

    return 0;
}