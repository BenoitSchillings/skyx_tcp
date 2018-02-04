//---------------------------------------------
#include "sky_ip.h"

#define MAX_SCRIPT  2048

//---------------------------------------------------

class Scope {
	SkyIP	connection;
public:	
	float	ra;
	float	dec;
	float   alt;
	float   az;
        
        char    cmd_result[8192];

	void Stop();
        void Start();
	void UpdateRaDec();
	void UpdateAltAz(); 
	     Scope();

	void Bump(float dx, float dy);	
	void Move(float dx, float dy);		//in arcsec
	void Log();
	void APCommand(char *cmd);
        void APCommandResult(const char *cmd);
	void Rate(float dx, float dy);
	void Goto(float ra, float dec);

};

//---------------------------------------------------
