
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//---------------------------------------------
#include "sky_ip.h"
#include "skyx.h"
#include "util.c"


    Scope::Scope()
{
	connection.conn((char*)"localhost", 3040);
}

//---------------------------------------------------

void Scope::Rate(float dx, float dy)
{
    char *input = ReadFile("./js/rate.txt", 100);
    char  var[32];
   
    sprintf(var, "%3.3f, %3.3f", dx, dy);
 
    char tmp[MAX_SCRIPT];

    Replace(input, "%1", var, tmp);

    //printf("%s\n", tmp);

    connection.send_data(tmp);

    free((char *)input);

    char *result;

    result = connection.receive(8192);

    free((char *)result);

}

//---------------------------------------------------

void Scope::Goto(float ra, float dec)
{
    char *input = ReadFile("./js/goto.txt", 100);
    char  var[32];
   
    sprintf(var, "%3.7f, %3.7f", ra, dec);
 
    char tmp[MAX_SCRIPT];

    Replace(input, "%1", var, tmp);

    printf("%s\n", tmp);

    connection.send_data(tmp);

    free((char *)input);

    char *result;

    result = connection.receive(8192);

    free((char *)result);

}

//---------------------------------------------------

void Scope::APCommandResult(const char *cmd)
{
   char *input = ReadFile("./js/ap_cmd_result.txt", 100);
    char tmp[MAX_SCRIPT];

    Replace(input, "%1", ":GA#", tmp);

    printf("%s\n", tmp);

    connection.send_data(tmp);

    char *result;

    result = connection.receive(8192);
    strcpy(cmd_result, result);

    printf("%s\n", result);
    free((char *)input);
    free((char *)result);

}

//---------------------------------------------------

void Scope::APCommand(char *cmd)
{
   char *input = ReadFile("./js/ap_cmd.txt", 100);

    char tmp[MAX_SCRIPT];

    Replace(input, "%1", cmd, tmp);

    //printf("%s\n", tmp);

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
    char cmd[128];
    
    dx *= 1000.0;
    dy *= 1000.0;
    printf("ba\n");

    if (dx > 999) dx = 999;
    if (dy > 999) dy = 999;
    if (dx < -999) dx = -999;
    if (dy < -999) dy = -999;
 

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

    //printf("%s\n", result);
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

void Scope::Log()
{
     UpdateRaDec();
     printf("%f %f\n", ra, dec);
}


//---------------------------------------------------

int umain(int argc, char **argv)
{
	Scope	s;
	int	cnt = 0;
	float	ira;
	float	idec;

	s.Start();	
	s.UpdateRaDec();
   	//s.Rate(-0.004, -0.061111);
	//s.Rate(0.001, 0);
   	//s.Rate(0*-0.034, 0.5*0.0061111);
 	
	printf("%f %f\n", s.ra, s.dec); 
	ira = s.ra;
	idec = s.dec;	
	while(1) {
		s.UpdateAltAz();
		usleep(1000*1000);	
		//s.UpdateRaDec();	
		printf("%d, %f %f, %f %f, %f %f\n", (cnt), s.alt, s.az, s.ra, s.dec, s.ra - ira, s.dec - idec);
		
		if ((cnt > 6600)/* || s.alt < 20.0*/) {
			s.Stop();
			while(1) {
				printf("stop\n");
				usleep(1000*1000);
			}	
		}
		usleep(1000*1000*3);
		cnt++;
		/*	
		if (cnt%80 == 0) {
        		s.Rate(1.5, 0);
		}
		if (cnt%80 == 19) {
			s.Rate(0, 1.5);	
		}
		if (cnt%80 == 39) {
			s.Rate(-1.5, 0);
		}
		if (cnt%80 == 69) {
			s.Rate(0, -1.5);
		}
		*/
			
		if (cnt%300 == 39) {
			usleep(1000*1000*3);	
			//if (s.az < 180)	
				//s.Goto(ira, idec);
			usleep(1000*1000*10);	
			//cnt = 0;
		}
			
	}
}



int eqwmain(int argc , char *argv[])
{
    Scope s;

    s.Start();
    s.Rate(0*-0.034, 0.5*0.0061111);
    s.UpdateRaDec();

    float ra0 = s.ra;
    float dec0 = s.dec;


    for (int i =0 ; i < 60; i++) {
    	s.UpdateRaDec();
    	printf("%f %f, %f %f\n", s.ra, s.dec, (s.ra - ra0)*15.0*3600.0, 3600 * (s.dec - dec0)); 
   	usleep(10000000); 
    }

    float d_dec = 3600.0 * (s.dec - dec0);
    d_dec = d_dec / 600.0;
    
    float d_ra = (15*3600)*(s.ra-ra0);
    d_ra = d_ra / 600.0;
 
    printf("delta %f, %f\n", d_ra, d_dec);

    //s.Rate(d_ra, d_dec);
    //s.Stop();

    s.UpdateRaDec();

    ra0 = s.ra;
    dec0 = s.dec;


    for (int i =0 ; i < 10; i++) {
        s.UpdateRaDec();
        printf("%f %f, %f %f\n", s.ra, s.dec, (s.ra - ra0)*15.0*3600.0, 3600 * (s.dec - dec0));
        usleep(10000000);
    }
 
    return 0;
}


//---------------------------------------------

