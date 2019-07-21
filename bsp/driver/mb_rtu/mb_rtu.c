#include "mb_rtu.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "modbus.h"
#include "portevent.h"
#include "unp.h"


void *mb_rtu_master_thread(void *arg)
{

	modbus_t *ctx = NULL;
	int rc, cnt = 0;
	int i,j;

	Port_modbus_ParaType *pdev;
	pdev = (Port_modbus_ParaType *)arg;

	Pthread_detach(pthread_self());
	/* print the arg */
	for(i = 0; i < 5; i++){
		printf("pdev[%d].slave_id:%d\n", i, pdev[i].slave_id);
		printf("pdev[%d].data_type:%d\n", i, pdev[i].data_type);
		printf("pdev[%d].data_para:%d\n", i, pdev[i].data_para);
		printf("pdev[%d].start_addr:%d\n", i, pdev[i].start_addr);
		printf("pdev[%d].unit_len:%d\n", i, pdev[i].unit_len);
		printf("pdev[%d].data_hold:%d\n", i, pdev[i].data_hold);
		printf("pdev[%d].scan_cycle:%d\n", i, pdev[i].scan_cycle);
		printf("pdev[%d].log:%d\n", i, pdev[i].log);
		printf("pdev[%d].repeat:%d\n", i, pdev[i].repeat);
	}
	//init the libmodbus
	ctx = modbus_new_rtu("/dev/ttySP0", 9600, 'N', 8, 1);
	if(ctx == NULL){
		err_quit("init the libmodbus error");
	}

	modbus_set_debug(ctx, 1);  //enable the debug info
	modbus_set_slave(ctx, 1);  //set slave id

	//connect the device
	if(modbus_connect(ctx) == -1){
		fprintf(stderr, "connect dev failed:%s\n", modbus_strerror(errno));
		modbus_close(ctx);
		modbus_free(ctx);
		err_quit("libmodbus connect error");
	}
	while(1){
		switch(eGetPort1ThreadState()){
		case STOPPED:
			printf("port1 stop read.\n");
			
			break;
		case READ:
			for(i = 0; i < 5; i++){
				modbus_set_slave(ctx, pdev[i].slave_id);
				rc = modbus_read_registers(ctx, pdev[i].start_addr,
									pdev[i].unit_len, pdev[i].data);
				if(rc == -1){
					fprintf(stderr, "%s\n", modbus_strerror(errno));
					modbus_close(ctx);
					modbus_free(ctx);
					err_quit("libmodbus read regs error");
				}else{
					for(j = 0; j < 10; j++)
					printf("reg[%d] = %d(0x%x)\n", j, pdev[i].data[j], pdev[i].data[j]);
				}
			}
			SetPort1ThreadState( WAIT_CNT );
			cnt = 0;
			break;
		case WRITE:
			break;
		case WAIT_CNT:
			cnt++;
			if(cnt > 2){
				cnt = 0;
				SetPort1ThreadState( READ );
			}
			break;
		case SHUTDOWN:
			printf("shutdown .\n");
			modbus_close(ctx);
			modbus_free(ctx);
			err_quit("port1 shutdown");
			break;
		}

		sleep(1);
	}

	modbus_close(ctx);
	modbus_free(ctx);

	err_quit("port1 thread quit.");
}
