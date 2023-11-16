#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define PORT 5550  // Port that will be opened
#define BACKLOG 20  // Number of allowed connections
#define BUFF_SIZE 1024

// Function to receive and echo a message to the client
void *echo(void* arg);

int main() {
    int listenfd, connfd;
    struct sockaddr_in server; // Server's address information
    struct sockaddr_in client; // Client's address information
    socklen_t sin_size = sizeof(struct sockaddr_in);
    pthread_t tid;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("\nError: ");
        return 0;
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY puts your IP address automatically

    if (bind(listenfd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("\nError: ");
        return 0;
    }

    if (listen(listenfd, BACKLOG) == -1) {
        perror("\nError: ");
        return 0;
    }

    while (1) {
        connfd = accept(listenfd, (struct sockaddr*)&client, &sin_size);
        if (connfd == -1) {
            perror("\nError: ");
        }
        printf("You got a connection from %s\n", inet_ntoa(client.sin_addr)); // Prints client's IP ( lỗi )

        // For each client, spawn a thread, and the thread handles the new client
        pthread_create(&tid, NULL, &echo, (void*) connfd);
    }

    close(listenfd);
    return 0;
}

void *echo(void* arg) {
    int connfd = *((int*)arg);
    int sent_bytes, received_bytes;
    char buff[BUFF_SIZE];

    free(arg);
    pthread_detach(pthread_self());

    received_bytes = recv(connfd, buff, BUFF_SIZE, 0); // Blocking

    if (received_bytes < 0) {
        perror("Error: ");
    } else if (received_bytes == 0) {
        printf("Connection closed.\n");
    } else {
        sent_bytes = send(connfd, buff, received_bytes, 0);
        if (sent_bytes < 0) {
            perror("\nError: ");
        }
    }

    close(connfd);
}

