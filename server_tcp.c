#include<stdio.h>
#include<winsock2.h>
#include<pthread.h>


int main(int argc , char *argv[])
{
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c;
    char *message,server_reply[2000];

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }

    printf("Socket created.\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }

    puts("Bind done");

    //Listen to incoming connections
    listen(s , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    new_socket = accept(s , (struct sockaddr *)&client, &c);
    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d" , WSAGetLastError());
    }

    puts("Connection accepted");

    //Reply to client
    message = "Hello Client , I have received your connection. But I have to go now, bye\n";
    send(new_socket , message , strlen(message) , 0);

    getchar();

    closesocket(s);
    WSACleanup();

    return 0;
}
