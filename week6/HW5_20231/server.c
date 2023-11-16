#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/wait.h>
#include<errno.h>
#include<signal.h>

#define BACKLOG 20
#define BUFF_SIZE 1024

/* Handler process signal*/
void sig_chld(int signo){
    pid_t pid;
    int stat;

    /* Wait the child process terminate */
    while((pid = waitpid(-1, &stat, WNOHANG))>0){
        printf("\nChild %d terminated\n", pid);
    }
}

/**
 * Receive and echo messages to client
 * [IN] sockfd: socket descriptor that connects to client
 */
void echo(int sockfd){
    char buff[BUFF_SIZE];
    int sent_bytes, received_bytes;

    received_bytes = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if(received_bytes < 0){
        perror("\nError: ");
    }else if(received_bytes == 0){
        printf("\nConnection closed\n");
    }else{
        sent_bytes = send(sockfd, buff, received_bytes, 0);
        if(sent_bytes < 0){
            perror("\nError: ");
        }
    }
    close(sockfd);
}

int main(int argc, char **argv){
    int listen_sock, conn_sock; // file descriptor
    struct sockaddr_in server, client; // address structure
    pid_t pid;
    int sin_size;

    if((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        printf("socket error\n");
        return 0;
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listen_sock, (struct sockaddr *)&server, sizeof(server)) == -1){
        perror("Error: ");
        return 0;
    }
    if(listen(listen_sock, BACKLOG) == -1){
        perror("Error: ");
        return 0;
    }

    // Establish a signal handler to catch SIGCHLD
    signal(SIGCHLD, sig_chld);

    while(1){
        sin_size = sizeof(struct sockaddr_in);
        if((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size)) == -1){
            if(errno == EINTR){
                continue;
            }else{
                perror("Error: ");
                return 0;
            }
        }

        // For each client, folk spawns a child, and the child handles the new client
        pid = fork();

        // fork() is called in child process
        if(pid == 0){
            close(listen_sock);
            printf("You are already connected\n");
            echo(conn_sock);
            exit(0);
        }

        // The parent closes the connected socket since the child handles the new client
        close(conn_sock);
    }
    close(listen_sock);
    return 0;
}
