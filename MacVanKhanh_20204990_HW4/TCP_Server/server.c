/* TCP Echo Server with Multi-threading */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<sys/wait.h>

#define BACKLOG 10 // Number of allowed connections

/**
 * @function createFolerStorge: check folder which is used to storge received data from client to server exist?
 *                              if not exist yet -> create new folder
 * @param folder: name of folder need to create to storge
*/
void createFolerStorge(char *folder){
    if(access(folder, F_OK) != -1){
        printf("Welcome to file server\n");
    }else{
        if(mkdir(folder, 0777) == 0){
            printf("Welcome to file server\n");
        }else{
            printf("Make folder error!!\n");
            exit(1);
        }
    }
}

void breakString(char *str, char *str1, char *str2, int num){
    int result = sscanf(str, "%s %s %d", str1, str2, &num);
    if(result!= 3){
        printf("Wrong input\n");
    }
}

void saveFile(char *folder, char *filename, int size){
    FILE *f_source, *f_target;
    unsigned char buffer[size];
    size_t bytes_read;

    f_source = fopen(filename, "rb");
    if (f_source == NULL) {
        perror("\n Error opening file");
    }

    char target_path[sizeof(folder)+sizeof(filename)+2];
    snprintf(target_path, sizeof(folder)+sizeof(filename)+5, "./%s/%s", folder, filename);

    f_target = fopen("target_path", "ab");

    while((bytes_read = fread(buffer, 1, sizeof(buffer), f_source)) > 0){
        fwrite(buffer, 1, bytes_read, f_target);
    }

    fclose(f_source);
    fclose(f_target);
}

int main(int argc, char *argv[]) {
    
    int num_port = atoi(argv[1]); // convert port from string to number

    int listen_sock, conn_sock; //file descriptions
    char clientIP[INET_ADDRSTRLEN], recv_data[1024];

    int bytes_sent, received_bytes;
    struct sockaddr_in server; // server's address information
    struct sockaddr_in client; // client's address information
    int client_port;

    // Step 1: Construct a TCP socket to listen connection request
    if((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("\nError: ");
        exit(EXIT_FAILURE);
    }

    // Step 2: Bind address to socket
    memset(&server, 0, sizeof(server)); // Use memset to clear the structure
    server.sin_family = AF_INET;
    server.sin_port = htons(num_port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listen_sock, (struct sockaddr *) &server, sizeof(server)) == -1){
        perror("\nError: ");
        exit(EXIT_FAILURE);
    }

    // Step 3: Listen request from client
    if(listen(listen_sock, BACKLOG) == -1){
        perror("\nError: ");
        exit(EXIT_FAILURE);
    }

    createFolerStorge(argv[2]);
    socklen_t sin_size = sizeof(struct sockaddr_in);
    while(1){
        // accept request
        conn_sock = accept(listen_sock, (struct sockaddr *) &client, &sin_size);
        if(conn_sock == -1){
            perror("\nError: ");
            exit(EXIT_FAILURE);
        }
        inet_ntop(AF_INET, &client.sin_addr, clientIP, sizeof(clientIP));
        client_port = ntohs(client.sin_port);

        pid_t child_pid = fork();

        if(child_pid == -1){
            perror("Error: ");
            exit(EXIT_FAILURE);
        }else if(child_pid == 0){
            close(listen_sock);
            while(1){
                send(conn_sock, "Please send file ", sizeof("Please send file "), 0);

                // receive message from client
                received_bytes = recv(conn_sock, recv_data, sizeof(recv_data), 0);
                recv_data[received_bytes] = '\0';

                char *str1, *file_name;
                int size;
                printf("%s\n", recv_data);
                
                breakString(recv_data, str1, file_name, size);

                printf("%s %s %d\n", str1, file_name, size);

                if(received_bytes < 0){
                    perror("\nError: ");
                }else{
                    saveFile(argv[2], file_name, size);
                    send(conn_sock,"Successful upload ",sizeof("Successful upload "), 0);
                }
            }
        }else{
            close(conn_sock);
        }
    }

    return 0;
}
