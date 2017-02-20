#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

//---------------------------------------------
 
using namespace std;
 
class tcp_client
{
private:
    int sock;
    char address[128];
    int port;
    struct sockaddr_in server;
     
public:
    tcp_client();
    bool conn(char *, int);
    bool send_data(char * data);
    char *receive(int);
};

//----------------------------------------------
 
tcp_client::tcp_client()
{
    sock = -1;
    port = 0;
    strcpy(address, "localhost");
}

//----------------------------------------------
 
bool tcp_client::conn(char *address , int port)
{
    if(sock == -1)
    {
        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            perror("Could not create socket");
        }
         
    }
    else    {   /* OK , nothing */  }
     
    //setup address structure
    if(inet_addr(address) == -1)
    {
        struct hostent *he;
        struct in_addr **addr_list;
         
        //resolve the hostname, its not an ip address
        if ( (he = gethostbyname( address) ) == NULL)
        {
            //gethostbyname failed
            herror("gethostbyname");
            printf("Failed to resolve hostname\n");
             
            return false;
        }
         
        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;
 
        for(int i = 0; addr_list[i] != NULL; i++)
        {
            //strcpy(ip , inet_ntoa(*addr_list[i]) );
            server.sin_addr = *addr_list[i];
             
             
            break;
        }
    }
     
    //plain ip address
    else
    {
        server.sin_addr.s_addr = inet_addr( address);
    }
     
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
     
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    return true;
}
 
//---------------------------------------------

bool tcp_client::send_data(char *data)
{
    //Send some data
    if( send(sock , data , strlen( data) , 0) < 0)
    {
        perror("Send failed : ");
        return false;
    }
     
    return true;
}
 
//---------------------------------------------


char *tcp_client::receive(int size=8192)
{
    char tmp[256];
 
    char *buffer = (char *)malloc(size);
    buffer[0] = 0;

    usleep(10000);

    int cnt;
    int max = 1000;
    do { 
    	cnt = recv(sock , tmp , sizeof(size) , MSG_DONTWAIT);
	if (cnt > 0) { 
		tmp[cnt] = 0;
   		strcat(buffer, tmp);	
		if (strstr(buffer, "Error =") != 0) {
			max = 10;
		}	
	} 
	usleep(10);
    } while (max--);
 
    return buffer;
}
 
//---------------------------------------------

char* ReadFile(char *filename)
{
   char *buffer = NULL;
   int string_size, read_size;
   FILE *handler = fopen(filename, "r");

   if (handler)
   {
       // Seek the last byte of the file
       fseek(handler, 0, SEEK_END);
       // Offset from the first to the last byte, or in other words, filesize
       string_size = ftell(handler);
       // go back to the start of the file
       rewind(handler);

       // Allocate a string that can hold it all
       buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

       // Read it all in one operation
       read_size = fread(buffer, sizeof(char), string_size, handler);

       // fread doesn't set it so put a \0 in the last position
       // and buffer is now officially a string
       buffer[string_size] = '\0';

       if (string_size != read_size)
       {
           // Something went wrong, throw away the memory and set
           // the buffer to NULL
           free(buffer);
           buffer = NULL;
       }

       // Always remember to close the file.
       fclose(handler);
    }

    return buffer;
}


//---------------------------------------------

int main(int argc , char *argv[])
{
    char in[512];
    char *input;


    input = ReadFile("./x.txt"); 
    //printf("%s\n", input);
 
    tcp_client c;
   
    c.conn((char*)"localhost" , 3040);

    for (int i =0 ; i < 30; i++) { 
    	c.send_data(input);
    
 
    	char *result;
    
    	result = c.receive(8192);
 
    	printf("%s\n", result);
    	free(result);
     } 
     return 0;
}


//---------------------------------------------

