#include "unp.h"


typedef enum 
{
	STOPPED,
	READ,
	WRITE,
	WAIT_CNT,
	SHUTDOWN
} ePortThreadState;


ePortThreadState eGetPort1ThreadState( void );

void SetPort1ThreadState(ePortThreadState eNewState);