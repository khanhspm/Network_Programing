#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

/**
 * @function convertDomainToIPv4: convert domain to IPv4 address
 * @param domain_name: domain is needed to convert
*/

void convertDomainToIPv4(char *domain_name) {

    struct addrinfo hints; // suggest to get result
    struct addrinfo *result, *rp;
    int status; // variabale to check information of domain name

    // Set parameters for getaddrinfo function
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // Definition for IPv4 Protocol
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(domain_name, NULL, &hints, &result);

    // if status != 0 -> domain_name is not domain, else print IPv4 address.
    if (status != 0) {
        printf("Not found information\n");
        exit(EXIT_FAILURE);
    }

    // Loop through the results and print out the IPv4 address
    printf("Result:\n");
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr; // IPv4 address is returned from domain
        char ip_address[INET_ADDRSTRLEN];  // IPv4 address was converted
        inet_ntop(AF_INET, &(ipv4->sin_addr), ip_address, INET_ADDRSTRLEN);
        printf("%s\n", ip_address);
    }

    freeaddrinfo(result); // Free result after used.

}
