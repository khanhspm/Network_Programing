#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){

    if(argc != 3){
        printf("Type './client ip_address number_port' to start client!!\n");
        exit(1);
    }

    // Init socket UDP
    int num_port = atoi(argv[2]);
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Init Address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(num_port);
    
    char buf[256];
    while (1){
        printf("Enter message: ");
        fgets(buf, sizeof(buf), stdin);
        size_t len = strlen(buf);
            //delete character '\n' from message
            if (len > 0 && buf[len - 1] == '\n') {
                buf[len - 1] = '\0';
            }
        if(strlen(buf)==0) break;
        sendto(sender, buf, strlen(buf), 0, (struct sockaddr *)&addr, sizeof(addr));

        // received respone fromm server
        socklen_t server_addr_len = sizeof(addr);
        int recv_len = recvfrom(sender, buf, 256, 0, (struct sockaddr *)&addr, &server_addr_len);
        buf[recv_len] = '\0';

        printf("%s\n", buf);

    }
    
    return 0;
}