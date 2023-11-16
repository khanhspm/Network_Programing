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
        fprintf(stderr, "Sử dụng: %s <IP_Addr> <Port_Number>\n", argv[0]);
        exit(1);
    }

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    char buffer[256];

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("Lỗi khi mở socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
        error("Lỗi khi chuyển đổi địa chỉ IP");

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr) < 0))
        error("Lỗi khi kết nối");

    bzero(buffer, 256);
    read(sockfd, buffer, 255);
    printf("%s\n", buffer);

    char filename[256];
    int file_size;
    printf("Nhập đường dẫn file và kích thước file (theo cú pháp 'UPLD Ten_file Kich_thuoc_file'): ");
    fgets(buffer, 255, stdin);
    write(sockfd, buffer, strlen(buffer));

    bzero(buffer, 256);
    read(sockfd, buffer, 255);
    printf("%s\n", buffer);

    if (strncmp(buffer, "+OK Please send file", 21) == 0) {
        sscanf(buffer, "+OK Please send file %s %d", filename, &file_size);
        char file_path[512];
        snprintf(file_path, 512, "%s", filename);
        FILE *file = fopen(file_path, "rb");
        if (file == NULL) {
            error("Lỗi khi mở file");
        }

        bzero(buffer, 256);
        int bytes_read;
        while ((bytes_read = fread(buffer, 1, 256, file)) > 0) {
            write(sockfd, buffer, bytes_read);
        }

        fclose(file);

        bzero(buffer, 256);
        read(sockfd, buffer, 255);
        printf("%s\n", buffer);
    }
    close(sockfd);

    return 0;
}

