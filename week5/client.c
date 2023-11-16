#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_FILE_SIZE 4294967296 // 2^32 bytes
#define MAX_FILENAME_SIZE 256
#define BUFFER_SIZE 1024

void sendFile(int socket, const char *file_path) {
    char filename[MAX_FILENAME_SIZE];
    size_t size;
    
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        perror("File open error");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    send(socket, file_path, sizeof(file_path), 0);
    send(socket, &size, sizeof(size_t), 0);

    char buffer[BUFFER_SIZE];
    size_t total_bytes = 0;
    while (total_bytes < size) {
        size_t bytes_to_read = fread(buffer, 1, BUFFER_SIZE, file);
        if (bytes_to_read <= 0) {
            perror("File read error");
            exit(EXIT_FAILURE);
        }
        send(socket, buffer, bytes_to_read, 0);
        total_bytes += bytes_to_read;
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP_Addr> <Port_Number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip_addr = argv[1];
    int port_number = atoi(argv[2]);

    struct sockaddr_in server_addr;
    int client_socket;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    inet_pton(AF_INET, ip_addr, &server_addr.sin_addr);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

    char welcome_msg[256];
    recv(client_socket, welcome_msg, sizeof(welcome_msg), 0);
    printf("%s\n", welcome_msg);

    char file_path[MAX_FILENAME_SIZE];
    size_t size;
    while (1) {
        printf("Enter file path: ");
        fgets(file_path, MAX_FILENAME_SIZE, stdin);
        file_path[strcspn(file_path, "\n")] = 0;

        if (strlen(file_path) == 0) {
            break;
        }

        sendFile(client_socket, file_path);

        char success_msg[256];
        recv(client_socket, success_msg, sizeof(success_msg), 0);
        printf("%s\n", success_msg);
    }

    close(client_socket);

    return 0;
}

