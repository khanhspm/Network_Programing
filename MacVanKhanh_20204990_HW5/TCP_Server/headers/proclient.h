#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/wait.h>
#include<errno.h>
#include<signal.h>
#include"liblist.h"

#define BACKLOG 20
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
 * 
 * @function receiveAndEchoMessage: Receive and echo messages to client
 * @param l: Node
 * @param sockfd: socket descriptor that connects to client
 * @param string: string to check
 * @param status: loggin status
 */
void receiveAndEchoMessage(node l, int sockfd, char *string, int status){
    char message[BUFF_SIZE];
    int received_bytes, sent_bytes;

    while(1){
        received_bytes = recv(sockfd, message, BUFF_SIZE, 0); //blocking
        size_t len_mess = strlen(message);
        //delete character '\n' from message
        if (len_mess > 0 && message[len_mess - 1] == '\n') {
            message[len_mess - 1] = '\0';
        }
        if(received_bytes < 0){
            perror("\nError: ");
        }else if(received_bytes == 0){
            printf("\nConnection closed\n");
        }
        strtok(message, " "); // get TYPE request
        if(strcmp(message, "USER") == 0){
            strcpy(string, message + 5);
            if(status == 1){
                strcpy(message, "213 - You have already logged in!!");
            }else if(status == 0){
                if(checkLogin(l, string) == 0){
                    strcpy(message, "212 - Account does not exist!!");
                }else if(checkLogin(l, string) == 1){
                    strcpy(message, "211 - Account is banned!!");
                }else if(checkLogin(l, string) == 2){
                    status = 1;
                    strcpy(message, "110 - Account is already in use!!");
                }
            }
            
        }else if(strcmp(message, "POST") == 0){
            strcpy(message, checkPost(status));
        }else if(strcmp(message, "BYE") == 0){
            strcpy(message, checkLogout(status));
        }else{
            strcpy(message, "300 - Your message was not accepted!!");
        }
        sent_bytes = send(sockfd, message, strlen(message), 0); // echo to the client
        if(sent_bytes < 0){
            perror("\nError: ");
        }else if(sent_bytes == 0){
            printf("0\n");
        }
    }
close(sockfd);
}