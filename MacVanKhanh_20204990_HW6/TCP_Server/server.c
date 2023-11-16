#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "headers/liblist.h"

#define BACKLOG 20  // Number of allowed connections
#define BUFF_SIZE 1024

/**
 * @function checkLogin: checking account exist and is bannedned.
 * 
 * @param head: Node 
 * @param username: A input string.
 *  
 * @return: 0 if account not exist.
 *          1 if account exist and is bannedned.
 *          2 if account exist and is not bannedned.
*/
int checkLogin(node head, char username[]){
    int check = 0;
	if(checkEmptyList(head)){
        return 0;
    }else{
        struct Node* p = head;
        while(p != NULL){
            if(strcmp(username, p->data.username) == 0){
                if(p->data.banned == 0){
                    check = 1;
                }
                if(p->data.banned == 1){
                    check = 2;
                }
            }
            p = p->next;
        }
    }

    return check;
}

/**
 * @@function checkPost: check post successfully or not
 * 
 * @param status : loggin status
 * @return char* 
 */
char *checkPost(int status){
    if(status == 1){
        return "120 - Successful post!!";
    }else{
        return "221 - You have not logged in yet!!";
    }
}

/**
 * @@function checkPost: check logout successfully or have not login yet
 * 
 * @param status : loggin status
 * @return char* 
 */
char *checkLogout(int status){
    if(status == 1){
        status = 0;
        return "130 - Successfully logged out!!";
    }else{
        return "221 - You have not logged in yet!!";
    }
}

// Function to receive and echo a message to the client
void *echo(void *);

int main(int argc, char **argv){

    if(argc != 2){
        printf("Usage: ./server <number port>\n");
        exit(1);
    }

    int listenfd, *connfd;
    struct sockaddr_in server; // Server's address information
    struct sockaddr_in *client; // Client's address information
    socklen_t sin_size = sizeof(struct sockaddr_in);
    pthread_t tid;

    //Step 1: Construct socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("\nError: ");
        return 0;
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY puts your IP address automatically

    if (bind(listenfd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("\nError: ");
        return 0;
    }

    if (listen(listenfd, BACKLOG) == -1) {
        perror("\nError: ");
        return 0;
    }

    client = malloc(sin_size);

    while (1) {
        connfd = malloc(sizeof(int));
        *connfd = accept(listenfd, (struct sockaddr*) client, &sin_size);
        if (*connfd == -1) {
            perror("\nError: ");
        }

        // For each client, spawn a thread, and the thread handles the new client
        pthread_create(&tid, NULL, &echo, connfd);
    }

    close(listenfd);

    return 0;
}

void *echo(void* arg) {
    node h = NULL;
    user x;
    char buf[BUFF_SIZE]; // initialize the string variable to read file
    FILE *fp;
    fp = fopen("TCP_Server/account.txt", "r");

    if(fp == NULL){
        perror("Error: ");
        exit(EXIT_FAILURE);
    }
    while(fgets(buf, 255, fp) != NULL){
        if(sscanf(buf, "%s %d", x.username, &x.banned) == 2){
        	addNode(&h, x);
		}
    }

    fclose(fp);
    if(h == NULL) {
        printf("Can not get data!!\n");
    }
    while(h != NULL) {
        printf("%s\n", x.username);
        h = h->next;
    }
    int connfd = *((int*) arg);
    int sent_bytes, received_bytes;
    char buff[BUFF_SIZE];

    free(arg);  
    pthread_detach(pthread_self());

    int status = 0;

    while(1){
        received_bytes = recv(connfd, buff, BUFF_SIZE, 0); // Blocking
        buff[received_bytes-1] = '\0';
        char buff_cpy[BUFF_SIZE], string[BUFF_SIZE];
        strcpy(buff_cpy, buff);
        strtok(buff, " "); // get TYPE request

    if (received_bytes < 0) {
        perror("Error: ");
    } else if (received_bytes == 0) {
        break;
    } else {
        strtok(buff, " "); // get TYPE request
        if(strcmp(buff, "USER") == 0){
            strcpy(string, buff_cpy + 5);
            printf("%s\n", string);
            if(status == 1){
                strcpy(buff, "213 - You have already logged in!!");
            }else if(status == 0){
                if(checkLogin(h, string) == 0){
                    strcpy(buff, "212 - Account does not exist!!");
                }else if(checkLogin(h, string) == 1){
                    strcpy(buff, "211 - Account is banned!!");
                }else if(checkLogin(h, string) == 2){
                    status = 1;
                    strcpy(buff, "110 - Account is already in use!!");
                }
            }
            
        }else if(strcmp(buff, "POST") == 0){
            strcpy(buff, checkPost(status));
        }else if(strcmp(buff, "BYE") == 0){
            strcpy(buff, checkLogout(status));
        }else{
            strcpy(buff, "300 - Your message was not accepted!!");
        }
        sent_bytes = send(connfd, buff, strlen(buff), 0);
        if (sent_bytes < 0) {
            perror("\nError: ");
        }
    }
    }

    close(connfd);
}