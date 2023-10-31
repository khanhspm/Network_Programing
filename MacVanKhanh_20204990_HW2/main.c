#include<stdio.h>
#include"headers/domain.h"
#include"headers/ip.h"

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Typing : './resolver 'domain or IPv4'' to find result.\n");
        exit(EXIT_FAILURE);
    }

    struct in_addr addr;

    if (inet_pton(AF_INET, argv[1], &addr)) {
        convertIPv4ToDomain(argv[1]);
    } else {
        convertDomainToIPv4(argv[1]);
    }
    return 0;
}
