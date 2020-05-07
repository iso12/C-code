#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[MAX]
    int n;
    for (;;)
    {
        bzero(buff, sizeof(buff))
        print ("Enter the string :");
        n =0;
        while ((buff[n++] = getchar()) != '\n')

        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff)); 
        printf("From Server : %s", buff); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 

        }
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in server, cli;

    sockfd = socket(AN_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creating failed...\n");
        exit(0);

    }

    else
    printf("Socket successfully created...\n");
    bzero(servaddr, sizeof (servaddr));

    servaddr.sin_family = AF_NET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);


    if ((bind(sockfd,(SA*)&servaddr. sizeof(servaddr))) ! = 0)
    {
        printf("socket bind failed...\n");
        exit(0);



    }
    else
    printf("socket successfully binded..\n");

    if((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }

    else
    printf("server listing..\n");
    len = sizeof(cli);

    connfd = accept(sockfd, (SA*) & cli, & len);
    if(connfd < 0)
    {
        printf("server accept failed..."\n);
        exit(0);

    }

    else
    printf("server accept the client");
    func(connfd);
    close (sockfd);
    


}


























