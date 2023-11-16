#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>  

#define BUFF_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_address> <server_port>\n", argv[0]);
        exit(1);
    }

    int client_sock;
    struct sockaddr_in server;
    char buffer[BUFF_SIZE];

    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket error");
        exit(1);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &server.sin_addr) <= 0) {
        perror("Invalid server address");
        exit(1);
    }

    if (connect(client_sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Connect error");
        exit(1);
    }else{
        recv(client_sock, buffer, BUFF_SIZE, 0);
        printf("\nResult: %s\n\n", buffer);
        buffer[BUFF_SIZE] = '\0';
    }

    while (1) {
        printf("------------ MENU --------------\n");
        printf("- Login: USER username\n");
        printf("- Post article: POST article\n");
        printf("- Logout: BYE\n");
        printf("- End: type empty string\n");
        printf("--------------------------------\n\n");
        printf("Enter message: ");
        fgets(buffer, BUFF_SIZE, stdin);
        size_t len = strlen(buffer);
        //delete character '\n' from message
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        if(strlen(buffer)==0){
            printf("\nSuccessfully end processing!!\n\n");
            break;
        }
        if (send(client_sock, buffer, strlen(buffer), 0) == -1) {
            perror("Send error");
            exit(1);
        }

        buffer[0] = '\0';

        int received_bytes = recv(client_sock, buffer, BUFF_SIZE, 0);
        if (received_bytes <= 0) {
            if (received_bytes == 0) {
                printf("Server closed the connection\n");
            } else {
                perror("Receive error");
            }
            break;
        }

        buffer[received_bytes] = '\0';
        printf("\nResult: %s\n\n", buffer);
    }

    close(client_sock);
    return 0;
}
