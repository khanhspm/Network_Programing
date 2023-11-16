#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Sử dụng: %s <Port_Number> <Directory_name>\n", argv[0]);
        exit(1);
    }

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[256];

    portno = atoi(argv[1]);
    char *directory_name = argv[2];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("Lỗi khi mở socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr) < 0))
        error("Lỗi khi bind");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0) 
        error("Lỗi khi accept");

    bzero(buffer, 256);
    strcpy(buffer, "+OK Welcome to file server");
    write(newsockfd, buffer, strlen(buffer));

    bzero(buffer, 256);
    read(newsockfd, buffer, 255);
    if (strncmp(buffer, "UPLD", 4) == 0) {
        char filename[256];
        int file_size;
        sscanf(buffer, "UPLD %s %d", filename, &file_size);

        char file_path[512];
        snprintf(file_path, 512, "%s/%s", directory_name, filename);
        FILE *file = fopen(file_path, "wb");
        if (file == NULL) {
            error("Lỗi khi mở file");
        }

        int total_bytes_received = 0;
        while (total_bytes_received < file_size) {
            bzero(buffer, 256);
            int bytes_received = read(newsockfd, buffer, 255);
            if (bytes_received < 0) {
                error("Lỗi khi đọc từ socket");
            }
            total_bytes_received += bytes_received;
            fwrite(buffer, 1, bytes_received, file);
        }

        fclose(file);

        bzero(buffer, 256);
        strcpy(buffer, "+OK Successful upload.");
        write(newsockfd, buffer, strlen(buffer));
    }
    close(newsockfd);
    close(sockfd);

    return 0;
}

