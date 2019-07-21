#include <unp.h>
#include <bsp.h>
#include "mb_rtu.h"
#include "portevent.h"

//#include <sqlite3.h>


//sqlite3 *gDb = NULL;


Port_modbus_ParaType *pPort1ModbusPara = NULL;
static char bDoExit;
int
main(void)
{
	int cnt = 0;
	pthread_t tid[10];
	int i;
	char cmd;

	pPort1ModbusPara = (Port_modbus_ParaType *)malloc(5*sizeof(Port_modbus_ParaType));

	for(i = 0; i < 5; i++){
		pPort1ModbusPara[i].slave_id = i + 1;
		pPort1ModbusPara[i].data_type = 3;
		pPort1ModbusPara[i].data_para = 1;
		pPort1ModbusPara[i].start_addr = 0;
		pPort1ModbusPara[i].unit_len = 10;
		pPort1ModbusPara[i].data_hold = 0;
		pPort1ModbusPara[i].scan_cycle = 2;
		pPort1ModbusPara[i].log = 0;
		pPort1ModbusPara[i].repeat = 3;

		pPort1ModbusPara[i].data = (u16 *)malloc(10*sizeof(u16));
	}

	pthread_create(&tid[0], NULL, mb_rtu_master_thread, (void *)pPort1ModbusPara);

	bDoExit = 0;

	do{
		printf("> ");
		cmd = getchar();

		switch( cmd ){
		case 'q':
			bDoExit = 1;
			break;
		case 'd':
			break;
		case 'e':
			SetPort1ThreadState( READ );
			break;
		case 's':
			break;
		case 'h':
			printf( "Gateway test help:\n" );
            printf( "  'd' ... disable protocol stack.\n" );
            printf( "  'e' ... enabled the protocol stack.\n" );
            printf( "  's' ... show current status.\n" );
            printf( "  'q' ... quit application.\n" );
            printf( "  'h' ... this information.\n" );
            printf( "\n" );
            printf( "*********** Copyright 2019 **************\n" );
			break;
		default:
			if( !bDoExit && (cmd != '\n'))
				printf("illegal command '%c'!\n", cmd);
			break;
		}

		while( !bDoExit && (cmd != '\n'))
			cmd = getchar();

	}while( !bDoExit );

	return 0;
}
