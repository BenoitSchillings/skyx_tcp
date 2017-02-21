
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

//---------------------------------------------
#include "sky_ip.h"
#include "util.c"

#define MAX_SCRIPT  2048
//---------------------------------------------------

class Scope {
	SkyIP	connection;
public:	
	float	ra;
	float	dec;
	float   alt;
	float   az;

	void Stop();
        void Start();
	void UpdateRaDec();
	void UpdateAltAz(); 
	     Scope();

	void SendAPCommand(const char *cmd);	
	void Move(float dx, float dy);		//in arcsec
};

//---------------------------------------------------

    Scope::Scope()
{
	connection.conn((char*)"localhost", 3040);
}

//---------------------------------------------------

void Scope::SendAPCommand(const char *cmd)
{
    char *input = ReadFile("./js/ap_cmd.txt", 100);
    
    char tmp[MAX_SCRIPT];

    Replace(input, "%1", ":Ms010#", tmp);
    
    printf("%s\n", tmp);
    return;
    connection.send_data(input);

    char *result;

    result = connection.receive(8192);

    printf("%s\n", result);
    free((char *)input);
    free((char *)result);

}

//---------------------------------------------------

void Scope::Move(float dx, float dy)
{
    char *input = ReadFile("./js/move.txt");
    connection.send_data(input);

    char *result;

    result = connection.receive(8192);

    //printf("%s\n", result);
    free((char *)input);
    free((char *)result);
}

//---------------------------------------------------


void Scope::Stop()
{
    char *input = ReadFile("./js/stop.txt");
    connection.send_data(input);

    char *result;

    result = connection.receive(8192);
       
    //printf("%s\n", result); 
    free((char *)input);
    free((char *)result);
}

//---------------------------------------------------


void Scope::Start()
{
    char *input = ReadFile("./js/start.txt");
    connection.send_data(input);

    char *result;

    result = connection.receive(8192);
       
    //printf("%s\n", result); 
    free((char *)input);
    free((char *)result);
}

//---------------------------------------------------


void Scope::UpdateAltAz()
{
    char *input = ReadFile("./js/get_alt_az.txt");
    connection.send_data(input);

    char *result;

    result = connection.receive(8192);


    sscanf(result, "%f|%f|", &alt, &az);

    free((char *)input);
    free((char *)result);
}


//---------------------------------------------------


void Scope::UpdateRaDec()
{
    char *input = ReadFile("./js/get_ra_dec.txt");
    connection.send_data(input);

    char *result;

    result = connection.receive(8192);


    sscanf(result, "%f|%f|", &ra, &dec);
	
    free((char *)input);
    free((char *)result);
}

//---------------------------------------------------

int main(int argc , char *argv[])
{
    Scope s;

    s.Stop();

    for (int i = 0; i < 200; i++) {
    	s.UpdateRaDec();
    	s.UpdateAltAz();
 
    	printf("%f %f\n", s.ra, s.dec); 
    	printf("%f %f\n", s.alt, s.az); 
    	if (i % 20 == 10) s.SendAPCommand("bla"); 
     } 
     s.Stop();
     
     return 0;
}


//---------------------------------------------

