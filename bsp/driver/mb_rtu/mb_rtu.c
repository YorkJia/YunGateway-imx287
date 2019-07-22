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

	DriverThreadPara_TypeDef *pdev;
	Modbus_ReadRegsTypeDef *pread_para;
	Modbus_WriteRegsTypeDef *pwrite_para;


	pdev = (DriverThreadPara_TypeDef *)arg;
	pread_para = (Modbus_ReadRegsTypeDef *)pdev->read_data;
    pwrite_para = (Modbus_WriteRegsTypeDef *)pdev->write_data;

	Pthread_detach(pthread_self());
	/* print the arg */
	for(i = 0; i < 5; i++){
		printf("pdev[%d].slave_id:%d\n", i, pread_para[i].slave_id);
		printf("pdev[%d].data_type:%d\n", i, pread_para[i].data_type);
		printf("pdev[%d].data_para:%d\n", i, pread_para[i].data_para);
		printf("pdev[%d].start_addr:%d\n", i, pread_para[i].start_addr);
		printf("pdev[%d].unit_len:%d\n", i, pread_para[i].unit_len);
		printf("pdev[%d].data_hold:%d\n", i, pread_para[i].data_hold);
		printf("pdev[%d].scan_cycle:%d\n", i, pread_para[i].scan_cycle);
		printf("pdev[%d].log:%d\n", i, pread_para[i].log);
		printf("pdev[%d].repeat:%d\n", i, pread_para[i].repeat);
	}
	//init the libmodbus
	ctx = modbus_new_rtu("/dev/ttySP0", 9600, 'N', 8, 1);
	if(ctx == NULL){
		err_quit("init the libmodbus error");
	}

	modbus_set_debug(ctx, 0);  //enable the debug info
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
			//printf("port1 stop read.\n");
			
			break;
		case READ:
			for(i = 0; i < 5; i++){
				modbus_set_slave(ctx, pread_para[i].slave_id);
				rc = modbus_read_registers(ctx, pread_para[i].start_addr,
									pread_para[i].unit_len, pread_para[i].rx_data);
				if(rc == -1){
					fprintf(stderr, "%s\n", modbus_strerror(errno));
					modbus_close(ctx);
					modbus_free(ctx);
					err_quit("libmodbus read regs error");
				}else{
					for(j = 0; j < 10; j++)
					printf("reg[%d] = %d(0x%x)\n", j,pread_para[i].rx_data[j], pread_para[i].rx_data[j]);
				}
			}
			SetPort1ThreadState( WAIT_CNT );
			cnt = 0;
			break;
		case WRITE:
			modbus_write_registers(ctx, pwrite_para->start_addr, pwrite_para->unit_len, (const uint16_t *)pwrite_para->valid_data);
			cnt = 0;
			SetPort1ThreadState( READ );
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
