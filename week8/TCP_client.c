/* TCP Echo Client */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5550
#define BUFF_SIZE 1024

int main() {
    int client_sock;
    char buff[BUFF_SIZE + 1];
    struct sockaddr_in server_addr; /* server's address information */
    int msg_len, sent_bytes, received_bytes;

    // Step 1: Construct socket
    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("\nError: ");
        exit(EXIT_FAILURE);
    }

    // Step 2: Specify server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // Step 3: Request to connect server
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
        perror("\nError: ");
        exit(EXIT_FAILURE);
    }

    // Step 4: Communicate with server
    printf("\nEnter string to send:");
    memset(buff, '\0', sizeof(buff));
    fgets(buff, BUFF_SIZE, stdin);
    msg_len = strlen(buff);
    sent_bytes = send(client_sock, buff, msg_len, 0);
    if (sent_bytes < 0)
        perror("\nError: ");

    // Receive echo reply
    received_bytes = recv(client_sock, buff, BUFF_SIZE, 0);
    if (received_bytes < 0)
        perror("\nError: ");
    else if (received_bytes == 0)
        printf("Connection closed.\n");
    else {
        buff[received_bytes] = '\0';
        printf("Reply from server: %s", buff);
    }

    // Step 5: Close socket
    close(client_sock);
    return 0;
}
