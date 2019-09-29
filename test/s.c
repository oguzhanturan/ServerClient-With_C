#include <windows.h>
#include  <stdlib.h>
#include <stdio.h>
#include <winsock.h>

// our thread for receiving commands
DWORD WINAPI receive_cmds(LPVOID lpParam)
{
  printf("thread created\r\n");

  // set our socket to the socket passed in as a parameter
  SOCKET current_client = (SOCKET)lpParam;

  // buffer to hold our recived data
  char buf[100];
  // buffer to hold our sent data
  char sendData[100];
  // for error checking
  int res;

  // our recv loop
  while(1)
  {
     res = recv(current_client,buf,sizeof(buf),0); // recv cmds

     Sleep(10);

     if(res == 0)
     {
      MessageBox(0,"error","error",MB_OK);
      closesocket(current_client);
      ExitThread(0);
     }

     if(strstr(buf,"hello"))
     { // greet this user
       printf("\nrecived hello cmd");

       strcpy(sendData,"hello, greetings\n");
       Sleep(10);
       send(current_client,sendData,sizeof(sendData),0);
     }
     else if(strstr(buf,"bye"))
     { // disconnect this user
       printf("\nrecived bye cmd\n");

       strcpy(sendData,"cya\n");
       Sleep(10);
       send(current_client,sendData,sizeof(sendData),0);

      // close the socket associted with this client and end this thread
       closesocket(current_client);
       ExitThread(0);
     }
     else
     {
       strcpy(sendData,"Invalid cmd\n");
       Sleep(10);
       send(current_client,sendData,sizeof(sendData),0);
     }

     // clear buffers
       strcpy(sendData,"");
       strcpy(buf,"");
   }
}
