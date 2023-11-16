#include<stdio.h>
#include"headers/proclient.h"

int main(int argc, char **argv){
    node l = NULL; // initialize the node
    user x; // initialize the user variable
    char buf[BUFF_SIZE]; // initialize the string variable to read file
    FILE *fp;
    fp = fopen("TCP_Server/account.txt", "r");

    if(fp == NULL){
        perror("Error: ");
        exit(1);
    }
    while(fgets(buf, 255, fp) != NULL){
        if(sscanf(buf, "%s %d", x.username, &x.banned) == 2){
        	addNode(&l, x);
		}
    }

    fclose(fp);

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

        // folk() is called in child process
        if(pid == 0){
            close(listen_sock);
            send(conn_sock, "100", 4, 0);
            int status = 0;
            char string[BUFF_SIZE];
            receiveAndEchoMessage(l, conn_sock, string, status);
            exit(0);
        }

        // The parent closes the connected socket since the child handles the new client
        close(conn_sock);
    }
    close(listen_sock);
    return 0;
}