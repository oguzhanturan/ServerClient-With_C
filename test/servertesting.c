/*Required Headers*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>
static void error_exit(char *errorMessage) {
    fprintf(stderr,"%s: %d\n", errorMessage, WSAGetLastError());
    break;
}
int main()
{
    char str[100];
    SOCKET listen_fd, comm_fd;
    struct sockaddr_in servaddr;
    /* Initialisiere TCP für Windows ("winsock"). */
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD (1, 1);
    if (WSAStartup (wVersionRequested, &wsaData) != 0)
        printf("Fehler beim Initialisieren von Winsock");
    else
        printf("Winsock initialisiert\n");

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(8888);

    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listen_fd, 10);

    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

    while(1)
    {
        read(comm_fd,str,100);
        printf("Echoing back - %s",str);
        write(comm_fd, str, strlen(str)+1);
    }
    closesocket(listen_fd);
    /* Cleanup Winsock */
    WSACleanup();

}
