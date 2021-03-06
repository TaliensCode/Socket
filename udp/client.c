/* 19:22 2015-04-22 Wednesday */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// ./server 127.0.0.1 8888
int main(int argc,  const char *argv[])
{
    int ret = 0;
    int sockfd;
    char packer[1024];
    struct sockaddr_in server_addr;

    socklen_t addrlen = sizeof(server_addr);
    
    if (argc < 3){
        fprintf(stderr, "Usage: %s <ip> <port>\n",argv[0]);
        exit(EXIT_FAILURE);
        }

    //create and open socket;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sockfd){
        perror("Fail to socket.");
        exit(EXIT_FAILURE);
    }

    //init
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    //bind ip and port;
    /*ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));*/
    /*if (-1 == ret){*/
        /*perror("Fail to bind");*/
        /*exit(EXIT_FAILURE);*/
    /*}*/

    //recv and send packet;
    sendto(sockfd, "hello", 5, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        printf("Input msg:\n");
    while (1)
    {

        //send
        fgets(packer,sizeof(packer),stdin);
        printf("+++++++++++++++++++++++\n");
        ret = sendto(sockfd, packer , strlen(packer)-1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (-1 == ret){
            perror("Fail to sendto");
            exit(EXIT_FAILURE);
        }
        if (strncmp(packer,"quit",4) == 0)
        {
            break;
        }
        printf("ret = %d\n",ret);
        //recv
        ret = recvfrom(sockfd, packer, sizeof(packer), 0, (struct sockaddr *)&server_addr, &addrlen);
        if (-1 == ret)
        {
            perror("Fail to recvfrom");
            break;
        }
        packer[ret] = '\0';

        printf("---------------------------\n");
        printf("ip      :%s\n", inet_ntoa(server_addr.sin_addr));
        printf("prot    :%d\n", ntohs(server_addr.sin_port));
        printf("recv[%d]:%s\n", ret, packer);
        printf("---------------------------\n");

    }
    close(sockfd);

    return 0;
}
