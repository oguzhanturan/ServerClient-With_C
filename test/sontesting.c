#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8888"
void *connection_handler(void *);
static SOCKET ClientSocket = INVALID_SOCKET;
int iSendResult;
int iResult;

// class Authenticate {
//     bool getAuthenticated() {

//     }
// };





int __cdecl main(void)
{
    WSADATA wsaData;
    int *new_sock;

    SOCKET ListenSocket = INVALID_SOCKET;


    struct addrinfo *result = NULL;
    struct addrinfo hints;
    pthread_t sniffer_thread[5];
    int i = 0;

    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    //freeaddrinfo(result);

        printf("starting\n");
        iResult = listen(ListenSocket, 5);
        if (iResult == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
        int c = sizeof(struct sockaddr_in);
        // Accept a client socket
    while( i < 5) {
        ClientSocket = accept(ListenSocket, (struct sockaddr *)&hints, (socklen_t*)&c);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        printf("Connection accepted\n");

        if( pthread_create( &sniffer_thread[i] , NULL ,  connection_handler , NULL) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        //cout << sniffer_thread[i].get_id();
         i++;
        //Now join the thread , so that we dont terminate before the thread
    }
        printf("Handler assigned\n");
        int i;
        for (i = 0; i < 5; ++i)
        {
             pthread_join(sniffer_thread[i] , NULL);
        }


    if (ClientSocket < 0)
    {
        perror("accept failed");

        return 1;
    }

    return 0;
}
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    char recvbuf[2000];

     do {
        cout << GetCurrentThreadId()<<endl;
        iResult = recv(ClientSocket, recvbuf, 2000, 0);
        printf("%s%d\n",recvbuf,strlen(recvbuf) );
            if(strcmp(recvbuf,"quit") == 0)
                closesocket(ClientSocket);
        recvbuf[iResult] = '\0';
        //fflush(stdout);
        if (iResult > 0) {
            printf("Client Says: \t%s\n", recvbuf);
            fflush(stdin);
            fgets(recvbuf,100,stdin);
            int ln = strlen(recvbuf)-1;
            if(recvbuf[ln] == '\n')
                recvbuf[ln] = '\0';

            iSendResult = send( ClientSocket, recvbuf, strlen(recvbuf), 0 );
            fflush(stdout);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
            }
            printf("Bytes sent from server: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());

        }

    } while (iResult > 0);
//Free the socket pointer
    closesocket(ClientSocket);
    return 0;
}
