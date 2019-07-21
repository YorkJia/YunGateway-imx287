#include "portevent.h"
#include "pthread.h"

static ePortThreadState ePort1ThreadState = STOPPED;
static pthread_mutex_t xPort1Lock = PTHREAD_MUTEX_INITIALIZER;


ePortThreadState eGetPort1ThreadState( void )
{
	ePortThreadState eCurState;
	( void )pthread_mutex_lock( &xPort1Lock );
	eCurState = ePort1ThreadState;
	( void )pthread_mutex_unlock( &xPort1Lock );

	return eCurState;
}

void SetPort1ThreadState(ePortThreadState eNewState)
{
	( void )pthread_mutex_lock( &xPort1Lock );
	ePort1ThreadState = eNewState;
	( void )pthread_mutex_unlock( &xPort1Lock );
} 
