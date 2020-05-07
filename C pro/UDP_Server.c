#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXKLINE 1024

int main()
{
    int sockfd;
    char buffer[MAXKLINE];
    char *hello = "hello from server";
    struct sockaddr_in servaddr, cliaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))< 0 )
    {
        perror("socket creating failed");
        exit(EXIT_FAILURE);

    }

    memset(servaddr, 0, sizeof(servaddr));
    memset(cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if(bind(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit()
    }
}