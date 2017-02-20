
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

//---------------------------------------------
#include "sky_ip.h"
#include "util.c"

//---------------------------------------------------

class Scope {
	SkyIP	connection;
public:	
	float	ra;
	float	dec;

	void UpdateRaDec();
	     Scope();
};

//---------------------------------------------------

    Scope::Scope()
{
	connection.conn((char*)"localhost", 3040);
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

    s.UpdateRaDec();
 
    printf("%f %f\n", s.ra, s.dec); 
    return 0;
}


//---------------------------------------------

