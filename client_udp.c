/*Simple udp client*/

#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "10.21.202.141"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int main(void)
{
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    char buf_recv[BUFLEN], buf_sent[BUFLEN];
    char message[BUFLEN];
    WSADATA wsa;

    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");

    //create socket
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        printf("socket() failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    //setup address structure
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

    //start communication
    while(1)
    {
        printf("Enter message : ");
        gets(buf_sent);

        //send the message
        if (sendto(s, buf_sent, strlen(message) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf_recv,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf_recv, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        printf("sunucu: %s\n",buf_recv);
    }
    closesocket(s);
    WSACleanup();

    return 0;
}
