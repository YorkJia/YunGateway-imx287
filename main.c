#include <unp.h>
#include <bsp.h>
#include "mb_rtu.h"
#include "portevent.h"

//#include <sqlite3.h>


//sqlite3 *gDb = NULL;

DriverThreadPara_TypeDef Port1ThreadPara;

static char bDoExit;
int
main(void)
{
	int cnt = 0;
	pthread_t tid[10];
	int i,j;
	char cmd;

	Modbus_ReadRegsTypeDef *pread_para;
	Modbus_WriteRegsTypeDef *pwrite_para;

	Port1ThreadPara.read_data = (Modbus_ReadRegsTypeDef *)malloc(5*sizeof(Modbus_ReadRegsTypeDef));  //5 read para pointer
	Port1ThreadPara.write_data = (Modbus_WriteRegsTypeDef *)malloc(sizeof(Modbus_WriteRegsTypeDef)); //1 write para pointer

	pread_para = (Modbus_ReadRegsTypeDef *)Port1ThreadPara.read_data;
	pwrite_para = (Modbus_WriteRegsTypeDef *)Port1ThreadPara.write_data;
	//init the read para pointer
	for(i = 0; i < 5; i++){
		pread_para[i].slave_id = i + 1;
		pread_para[i].data_type = 3;
		pread_para[i].data_para = 1;
		pread_para[i].start_addr = 0;
		pread_para[i].unit_len = 10;
		pread_para[i].data_hold = 0;
		pread_para[i].scan_cycle = 2;
		pread_para[i].log = 0;
		pread_para[i].repeat = 3;
		pread_para[i].rx_data = (uint16_t *)malloc(10*sizeof(uint16_t));
	}

	pwrite_para->slave_id = 5;
	pwrite_para->data_type = 3;
	pwrite_para->data_para = 1;
	pwrite_para->start_addr = 0;
	pwrite_para->unit_len = 10;
	for(i = 0; i < 10; i++)
		pwrite_para->valid_data[i] = 10*i;


	pthread_create(&tid[0], NULL, mb_rtu_master_thread, (void *)&Port1ThreadPara);

	bDoExit = 0;

	do{
		printf("please input cmd > ");
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
		case 'w':
			cnt++;
			for(i = 0; i < 10; i++)
				pwrite_para->valid_data[i] = 10*i + cnt;

			SetPort1ThreadState( WRITE );
			break;
		case 'p':
			for(i = 0; i < 5; i++){
				for(j = 0; j < 10; j++)
					printf("reg[%d] = %d(0x%x)\n", j,pread_para[i].rx_data[j], pread_para[i].rx_data[j]);
			}
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
