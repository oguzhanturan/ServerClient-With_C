#include "stdio.h"
#include "winsock.h"

#define PORT 8888

void main(int argc,char *argv[]){

WSADATA wsda;
WSAStartup(0x0101,&wsda);

struct sockaddr_in server;
int sockfd;
struct hostent *h;
char *message="Hello Server";


if(argc!=2){
printf("Usage : client ");
exit(1);
}

if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
printf("Socket Error...");
exit(1);
}

if((h=gethostbyname(argv[1]))==NULL){

fprintf(stderr,"Host Name Error...");
exit(1);
}

server.sin_addr=*((struct in_addr*)h->h_addr);
server.sin_port=htons(PORT);
server.sin_family=AF_INET;

if(connect(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){

fprintf(stderr,"Connection out...");
exit(1);
}

send(sockfd,message,strlen(message),0);


WSACleanup();
closesocket(sockfd);
}
