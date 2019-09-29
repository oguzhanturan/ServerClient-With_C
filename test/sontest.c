#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include <winsock.h>
#include<pthread.h> //for threading , link with lpthread
typedef uint32_t socklen_t;

//the thread function
void *connection_handler(void *);

int main(int argc , char *argv[])
{
    SOCKET socket_desc , client_sock;
    int c=0;
    struct sockaddr_in server , client;
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD (1, 1);

    if (WSAStartup (wVersionRequested, &wsaData) != 0)
        printf( "Fehler beim Initialisieren von Winsock");
    else
        printf("Winsock initialisiert\n");
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);


    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
	pthread_t thread_id;

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client,(socklen_t*)&c)) )
    {
        puts("Connection accepted");

        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}
