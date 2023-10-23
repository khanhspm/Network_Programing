#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

/**
 * @function convertIPv4ToDomain: convert IPv4 to IPv4 Domain
 * @param ip_address: IPv4 address is needed to convert
 * @return: - "Not found information" if IPv4 address is not found
 * @return: name of domain if IPv4 address is found 
*/
char* convertIPv4ToDomain(char ip_address[]) {
    struct sockaddr_in sa; 
    socklen_t salen = sizeof(sa);
    int result; // variabale to check information of IPv4 address

    sa.sin_family = AF_INET; // Definition for IPv4 Protocol
    sa.sin_addr.s_addr = inet_addr(ip_address);

    char* hostname = (char*)malloc(NI_MAXHOST * sizeof(char)); // Dynamically allocate memory for domain names

    result = getnameinfo((struct sockaddr*)&sa, salen, hostname, NI_MAXHOST, NULL, 0, 0);

    /* if result != 0 -> IPv4 address is not found else return name of domain*/
    if (result != 0) {
        return "Not found information";
    } else {
        return hostname;
    }

}