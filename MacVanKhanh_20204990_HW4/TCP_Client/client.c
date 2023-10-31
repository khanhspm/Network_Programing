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

#define BUFFSIZE 1024

int main(int argc, char *argv[]) {
    int client_sock;
    char buff[BUFFSIZE + 1];
    struct sockaddr_in server_addr; /* Server's address information */
    int msg_len, sent_bytes, received_bytes;

    // Step 1: Construct socket
    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("\nError: ");
        exit(EXIT_FAILURE);
    }

    int num_port = atoi(argv[2]);
    // Step 2: Specify server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(num_port);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // Step 3: Request to connect to the server
    if (connect(client_sock, (struct sockaddr*) &server_addr, sizeof(struct sockaddr)) < 0) {
        perror("\nError: ");
        exit(EXIT_FAILURE);
    }

    while(1){
        recv(client_sock, buff, BUFFSIZE, 0);
        printf("%s\n", buff);

        // Step 4: Communicate with the server
        printf("Enter string to send: ");
        memset(buff, '\0', (strlen(buff) + 1));
        fgets(buff, BUFFSIZE, stdin);
        size_t msg_len = strlen(buff);

        //delete character '\n' from message
        if (msg_len > 0 && buff[msg_len - 1] == '\n') {
            buff[msg_len - 1] = '\0';
        }

        if(strlen(buff) == 0) break;

        sent_bytes = send(client_sock, buff, msg_len, 0);
        if (sent_bytes < 0)
            perror("\nError: ");

        // Receive echo reply
        received_bytes = recv(client_sock, buff, BUFFSIZE, 0);
        buff[received_bytes] = '\0';
        printf("%s\n", buff);
        
        memset(buff, '\0', (strlen(buff) + 1));
    }

    // Step 5: Close the socket
    close(client_sock);

    return 0;
}
