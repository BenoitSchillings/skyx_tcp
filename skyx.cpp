
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

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

	void Bump(float dx, float dy);	
	void Move(float dx, float dy);		//in arcsec

	void APCommand(char *cmd);
};

//---------------------------------------------------

    Scope::Scope()
{
	connection.conn((char*)"localhost", 3040);
}


//---------------------------------------------------

void Scope::APCommand(char *cmd)
{
   char *input = ReadFile("./js/ap_cmd.txt", 100);

    char tmp[MAX_SCRIPT];

    Replace(input, "%1", cmd, tmp);

    printf("%s\n", tmp);

    connection.send_data(tmp);

    char *result;

    result = connection.receive(8192);

    //printf("%s\n", result);
    free((char *)input);
    free((char *)result);

}

//---------------------------------------------------
// move the mount for dx seconds.

void Scope::Bump(float dx, float dy)
{
    char cmd[32];
    
    dx *= 1000.0;
    dy *= 1000.0;

    if (fabs(dx) > 999 || fabs(dy) > 999) return;

    if (fabs(dy) > 4) {
    	if (dy > 0) {
    		sprintf(cmd, ":Ms%03d#", (int)(dy));
	}
	else {
		sprintf(cmd, ":Mn%03d#", (int)(-dy));	
	}
    	APCommand(cmd);
    }

    if (fabs(dx) > 4) {
        if (dx > 0) {
                sprintf(cmd, ":Me%03d#", (int)(dx));
        }
        else {
                sprintf(cmd, ":Mw%03d#", (int)(-dx));
        }
        APCommand(cmd);
    }

}

//---------------------------------------------------

void Scope::Move(float dx, float dy)
{
/*
    char *input = ReadFile("./js/move.txt");
    connection.send_data(input);

    char *result;

    result = connection.receive(8192);

    //printf("%s\n", result);
    free((char *)input);
    free((char *)result);
*/
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

    printf("%s\n", result);
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

    //s.Bump(-0.1, 0);
    for (int i =0 ; i < 10; i++) {
    	s.UpdateRaDec();
    	printf("%f %f\n", s.ra, s.dec); 
   	//usleep(300000); 
    	if (i == 5) s.Bump(0.0, -0.5);
   	if (i == 8) s.Bump(0.0, 0.5); 
    }

    //s.Stop();
 
    return 0;
}


//---------------------------------------------

