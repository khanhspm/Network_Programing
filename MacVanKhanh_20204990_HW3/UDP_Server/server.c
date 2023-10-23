#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<poll.h>
#include"headers/ip.h"
#include"headers/domain.h"

/**
 * @function saveLog: Record activity log with format [dd/mm/yyyy hh:mm:ss] $ message from client $ status respone from server
 * @param input1: message from client to server
 * @param input2: respone from server to client
 * @param status: message from client error or OK
*/
void saveLog(char *input1, char *input2, char *status){
    time_t rawtime; //Change to Local Timezone
   struct tm *info;
   char buffer[20];
   time( &rawtime ); //Get current time and save to rawtime
   info = localtime( &rawtime ); //Convert rawtime to local time information
   strftime(buffer,80,"%d/%m/%Y %X", info); //Save to string

   FILE *fl;
    fl = fopen("log_20204990.txt", "a+");

    if(fl == NULL){
        printf("Error opening file log_20204990.txt\n");
        exit(1);
    }

    fprintf(fl, "[%s] $ %s $ %s%s\n", buffer, input1, status, input2);

    fclose(fl);

}


int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Type './server number_port' to start server!!\n");
        exit(1);
    }

    int num_port = atoi(argv[1]);
    // Init socket for UDP
    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Init address receiver
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(num_port);

    bind(receiver, (struct sockaddr *)&addr, sizeof(addr));

    // Nhận tin nhắn
    char buf[256];
    struct sockaddr_in client_addr; // Address of client
    socklen_t client_addr_len = sizeof(client_addr); // Length of address of client

    char oke[] = "+"; 
    char err[] = "-";
    while (1){
        socklen_t client_addr_len = sizeof(client_addr);
        int ret = recvfrom(receiver, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (ret == -1) {
            printf("recvfrom() failed\n");
            break;
        }
        else {
            buf[ret] = '\0';
            struct in_addr addr;
            if (inet_pton(AF_INET, buf, &addr)){
                char* domain = convertIPv4ToDomain(buf);
                sendto(receiver, domain, strlen(domain), 0, (struct sockaddr *)&client_addr, client_addr_len); 
                if(strcmp(domain, "Not found information") == 0){
                    saveLog(buf, domain, err);
                }else{
                    saveLog(buf, domain, oke);
                }
                free(domain); // Free memory for domain
            }else{
                int ip_count;
                char** ips = convertDomainToIPv4(buf, &ip_count);
                if(ips != NULL && ip_count >0){
                    char server_res[INET_ADDRSTRLEN];
                    for (int i = 0; i < ip_count; i++) {
                        strcat(server_res, ips[i]);
                        strcat(server_res, " ");
                        free(ips[i]); // free memory for each IP address
                    }
                    sendto(receiver, server_res, strlen(server_res), 0, (struct sockaddr *)&client_addr, client_addr_len);
                    saveLog(buf, server_res, oke);
                    server_res[0] = '\0';
                }else{
                    char server_res[] = "Not found information";
                    sendto(receiver, server_res, strlen(server_res), 0, (struct sockaddr *)&client_addr, client_addr_len);
                    saveLog(buf, server_res, err);
                    server_res[0] = '\0';
                }
                free(ips); // Free memory for array ips
            }
        }
    }
     
    return 0;
}