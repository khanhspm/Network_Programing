#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

/**
 * @function convertDomainToIPv4: convert domain to IPv4 address
 * @param domain_name: domain is needed to convert
 * @param ip_count: number IPv4 can be converted from domain
*/

char** convertDomainToIPv4(char *domain_name, int *ip_count) {

    struct addrinfo hints; // suggest to get result
    struct addrinfo *result, *rp;
    int status; // variabale to check information of domain name

    // Set parameters for getaddrinfo function
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // Definition for IPv4 Protocol
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(domain_name, NULL, &hints, &result);

    // if status != 0 -> domain_name is not domain, return NULL
    if (status != 0) {
        *ip_count = 0;
        return NULL;
    }

    int count = 0;
    char** ip_addresses = NULL;
    ip_addresses = (char**)malloc(count * sizeof(char*));
    // Loop through the results and print out the IPv4 address
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr; // IPv4 address is returned from domain
        char* ip_address = (char*)malloc(INET_ADDRSTRLEN * sizeof(char));
        inet_ntop(AF_INET, &(ipv4->sin_addr), ip_address, INET_ADDRSTRLEN);
        ip_addresses[count] = ip_address;
        count++;
    }
    *ip_count = count;

    freeaddrinfo(result); // Free result after used.

    return ip_addresses;
}