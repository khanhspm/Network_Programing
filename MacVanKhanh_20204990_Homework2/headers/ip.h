#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

/**
 * @function convertIPv4ToDomain: convert IPv4 to IPv4 Domain
 * @param ip_address: IPv4 address is needed to convert
*/
void convertIPv4ToDomain(char ip_address[]) {
    struct sockaddr_in sa; 
    socklen_t salen = sizeof(sa);
    int result; // variabale to check information of IPv4 address

    sa.sin_family = AF_INET; // Definition for IPv4 Protocol
    sa.sin_addr.s_addr = inet_addr(ip_address);

    char hostname[NI_MAXHOST]; // name of domain

    result = getnameinfo((struct sockaddr*)&sa, salen, hostname, NI_MAXHOST, NULL, 0, 0);

    /* if result != 0 -> IPv4 address is not found else ip_address is IPv4 address*/
    if (result != 0) {
        printf("Not found information\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Result:\n");
        printf("%s\n", hostname);
    }

}
