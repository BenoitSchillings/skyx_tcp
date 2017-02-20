#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

//---------------------------------------------

#ifndef SKYIP
#define SKYIP

class SkyIP
{
private:
    int		sock;
    char	address[128];
    int		port;
    struct	sockaddr_in server;
   
    char	in_buf[8192];
    char	out_buf[8192]; 
public:
    SkyIP();
    bool conn(char *, int);
    bool send_data(char * data);
    char *receive(int);
};

#endif

