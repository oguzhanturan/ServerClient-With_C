#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <winsock.h>
 void error_exit(char *errorMessage) {
        fprintf(stderr,"%s: %d\n", errorMessage, WSAGetLastError());
        break;
    }

int main(int argc,char **argv)
{
    SOCKET sockfd,n;
    char sendline[100];
    char recvline[100];
    struct sockaddr_in servaddr;

    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD (1, 1);
    if (WSAStartup (wVersionRequested, &wsaData) != 0)
        error_exit( "Fehler beim Initialisieren von Winsock");
    else
        printf("Winsock initialisiert\n");

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(8888);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    while(1)
    {
        fgets(sendline,100,stdin); /*stdin = 0 , for standard input */
        write(sockfd,sendline,strlen(sendline)+1);
        read(sockfd,recvline,100);
        printf("%s",recvline);
    }
        closesocket(sockfd);
    /* Cleanup Winsock */
    WSACleanup();

}
