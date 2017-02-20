#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

//---------------------------------------------
#include "sky_ip.h"
//---------------------------------------------

char* ReadFile(const char *filename)
{
    char *buffer = NULL;
    int string_size, read_size;
    FILE *handler = fopen(filename, "r");

    if (handler) {
        fseek(handler, 0, SEEK_END);
        string_size = ftell(handler);
        rewind(handler);
        buffer = (char*) malloc(sizeof(char) * (string_size + 1) );
        read_size = fread(buffer, sizeof(char), string_size, handler);
        buffer[string_size] = '\0';
    
        if (string_size != read_size) {
            free(buffer);
            buffer = NULL;
        }
    
        fclose(handler);
    }

    return buffer;
}


//---------------------------------------------

int main(int argc , char *argv[])
{
    char in[512];
    char *input;


    SkyIP c;

    c.conn((char*)"localhost" , 3040);

    
    input = ReadFile("./js/get_ra_dec.txt");
    c.send_data(input);
    

    char *result;
    
    result = c.receive(8192);

    float ra, dec;

    sscanf(result, "%f|%f|", &ra, &dec);

    printf("%f %f\n",  ra , dec);
        

    free(result);
    free(input);
        
    return 0;
}


//---------------------------------------------

