#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 5550 // Port that will be opened
#define BACKLOG 27 // Number of allowed connections
#define BUFFSIZE 1024

void echo(int sockid);

int main() {
    int listen_sock, conn_sock; /* File descriptors */
    struct sockaddr_in server; /* Server's address information */
    socklen_t sin_size;

    // Step 1: Construct a TCP socket to listen for connection requests
    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("\nError: ");
        return 1;
    }

    // Step 2: Bind address to socket
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY puts your IP address automatically

    if (bind(listen_sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("\nError: ");
        return 1;
    }

    // Step 3: Listen for requests from clients
    if (listen(listen_sock, BACKLOG) == -1) {
        perror("\nError: ");
        return 1;
    }

    printf("Server started!\n");

    // Signal handling
    // Handle child processes that have terminated
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = SA_RESTART | SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sa, 0);

    while (1) {
        sin_size = sizeof(struct sockaddr_in);
        if ((conn_sock = accept(listen_sock, (struct sockaddr *)&server, &sin_size)) == -1) {
            if (errno == EINTR) continue;
            perror("\nError: ");
            return 1;
        }

        if (!fork()) {
            close(listen_sock);
            echo(conn_sock);
            close(conn_sock);
            exit(0);
        }

        close(conn_sock);
    }

    close(listen_sock);

    return 0;
}

void echo(int sockfd) {
    int received_bytes;
    char buff[BUFFSIZE];

    while (1) {
        received_bytes = recv(sockfd, buff, BUFFSIZE, 0);

        if (received_bytes <= 0) {
            return;
        } else {
            buff[received_bytes] = '\0';
            printf("Received: %s", buff);
            send(sockfd, buff, received_bytes, 0);
        }
    }
}

