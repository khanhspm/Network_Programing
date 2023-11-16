
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

void saveFile(int new_socket, const char *directory) {
    char filename[MAX_FILENAME_SIZE];
    size_t size;
    
    recv(new_socket, filename, MAX_FILENAME_SIZE, 0);
    recv(new_socket, &size, sizeof(size_t), 0);

    char target_path[MAX_FILENAME_SIZE + strlen(directory) + 2]; // +2 for '/' and null terminator
    strcpy(target_path, directory);
    strcat(target_path, "/");
    strcat(target_path, filename);

    FILE *file = fopen(target_path, "wb");
    if (file == NULL) {
        perror("File open error");
        exit(EXIT_FAILURE);
    }

    size_t total_bytes = 0;
    char buffer[BUFFER_SIZE];

    while (total_bytes < size) {
        size_t bytes_to_read = (size - total_bytes < BUFFER_SIZE) ? size - total_bytes : BUFFER_SIZE;
        ssize_t bytes_received = recv(new_socket, buffer, bytes_to_read, 0);
        if (bytes_received <= 0) {
            perror("File read error");
            exit(EXIT_FAILURE);
        }
        fwrite(buffer, 1, bytes_received, file);
        total_bytes += bytes_received;
    }

    fclose(file);
    send(new_socket, "+OK Successful upload", sizeof("+OK Successful upload"), 0);
}

// Tương tự cho hàm main và các phần khác của mã nguồn


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Port_Number> <Directory_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port_number = atoi(argv[1]);
    const char *directory = argv[2];

    struct sockaddr_in server_addr, client_addr;
    int server_socket, new_socket;
    socklen_t client_addr_len;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_number);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding error");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listening error");
        exit(EXIT_FAILURE);
    }

    client_addr_len = sizeof(client_addr);
    new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

    if (new_socket < 0) {
        perror("Accept error");
        exit(EXIT_FAILURE);
    }

    send(new_socket, "+OK Welcome to file server", sizeof("+OK Welcome to file server"), 0);
    saveFile(new_socket, directory);

    close(new_socket);
    close(server_socket);

    return 0;
}

