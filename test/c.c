int main()
{
 printf("Starting up multi-threaded TCP server\r\n");

 // our masterSocket(socket that listens for connections)
 SOCKET sock;

 // for our thread
 DWORD thread;

 WSADATA wsaData;
 SOCKADDR_IN server;

 // start winsock
 int ret = WSAStartup(0x101,&wsaData); // use highest version of winsock avalible

 if(ret != 0)
 {
    return 0;
 }

 // fill in winsock struct ...
 server.sin_family=AF_INET;
 server.sin_addr.s_addr=INADDR_ANY;
 server.sin_port=htons(123); // listen on telnet port 23

 // create our socket
 sock=socket(AF_INET,SOCK_STREAM,0);

 if(sock == INVALID_SOCKET)
 {
    return 0;
 }

 // bind our socket to a port(port 123)
 if( bind(sock,(SOCKADDR*)&server,sizeof(server)) !=0 )
 {
    return 0;
 }

 // listen for a connection
 if(listen(sock,5) != 0)
 {
    return 0;
 }

 // socket that we send & receive data on
 SOCKET client;

 SOCKADDR_IN from;
 int fromlen = sizeof(from);

 // loop forever
 while(1)
 {
  // accept connections
  client = accept(sock,(struct SOCKADDR*)&from,&fromlen);
  printf("Client connected\r\n");

  // create our recv_cmds thread and parse client socket as a parameter
  CreateThread(NULL, 0,receive_cmds,(LPVOID)client, 0, &thread);
 }

 // shutdown winsock
 closesocket(sock);
 WSACleanup();

 // exit
 return 0;
}
