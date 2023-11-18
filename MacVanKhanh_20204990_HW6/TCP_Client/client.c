#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 1024

void displayMenu(){
    printf("-----------------------MENU--------------------\n");
    printf("1. Log in\n");
    printf("2. Post message\n");
    printf("3. Logout\n");
    printf("4. Exit\n");
    printf("-----------------------------------------------\n");
}

int main(int argc, char **argv){

    if(argc != 3){
        printf("Usage: ./client <ip address> <port number>\n");
        exit(1);
    }

    int client_sock;
    char buff[BUFFSIZE + 1];
    struct sockaddr_in server_addr; //server address information
    int msg_len, sent_bytes, received_bytes;

    //Step 1: Construct socket
    if((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Error: ");
        exit(EXIT_FAILURE);
    }

    //Step 2: Specify server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    //Step 3: Request to connect server
    if(connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0){
        perror("Error: ");
        exit(EXIT_FAILURE);
    }

    char welcome_message[BUFFSIZE + 1];
    recv(client_sock, welcome_message, BUFFSIZE, 0);
    printf("%s\n", welcome_message);

    //Step 4: Communicate with server
    while(1){
        char choice[BUFFSIZE + 1];
        char message[BUFFSIZE + 1];
        displayMenu();

        printf("\nEnter choice: ");
        memset(choice, '\0', (strlen(choice) + 1));
        fgets(choice, BUFFSIZE, stdin);
        int choice_len = strlen(choice);
        if(choice_len == 1){
            exit(EXIT_SUCCESS);
        }
        if(choice[choice_len - 1] == '\n'){
            choice[choice_len - 1] = '\0';
        }

        memset(message, '\0', (strlen(message)+1));

        if(strcmp(choice, "1") == 0){
            strcpy(message, "USER ");

            printf("Enter username: ");
            memset(buff, '\0', (strlen(buff)+1));
            fgets(buff, BUFFSIZE, stdin);

            strcat(message, buff);
        }else if(strcmp(choice, "2") == 0){
            strcpy(message, "POST ");

            printf("Enter message: ");
            memset(buff, '\0', (strlen(buff)+1));
            fgets(buff, BUFFSIZE, stdin);

            strcat(message, buff);
        }else if(strcmp(choice, "3") == 0){
            strcpy(message, "BYE ");
        }else if(strcmp(choice, "4") == 0){
            exit(1);
        }else{
            strcpy(message, "ERROR ");
        }

        sent_bytes = send(client_sock, message, strlen(message), 0);
        if(sent_bytes < 0){
            perror("Error: ");
        }

        //receive message from server
        received_bytes = recv(client_sock, buff, BUFFSIZE, 0);
        if(received_bytes < 0){
            perror("Error: ");
        }else if(received_bytes == 0){
            printf("Connection closed\n");
        }else{
            buff[received_bytes + 1] = '\0';
            printf("%s\n", buff);
        }
    }

    //Step 5: Close socket
    close(client_sock);

    return 0;
}