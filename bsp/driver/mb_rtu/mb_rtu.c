#include "mb_rtu.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "modbus.h"


void *mb_rtu_master_thread(void *arg)
{

	modbus_t *ctx = NULL;
	int rc;
	int i,j;

	_THREAD_PARA *pdev;
	_RTU_DRIVER *rtu_para = NULL;
	pdev = (_THREAD_PARA *)arg;

	Pthread_detach(pthread_self());
	/* print the arg */
	for(i = 0; i < 5; i++){
		rtu_para = pdev->rtu_data[i];
		printf("pdev[%d]->slave_id:%d\n", i, rtu_para->slave_id);
		printf("pdev[%d]->start_addr:%d\n", i, rtu_para->start_addr);
		printf("pdev[%d]->unit_len:%d\n", i, rtu_para->unit_len);
		printf("pdev[%d]->cycle:%d\n", i, rtu_para->cycle);
	}
	//init the libmodbus
	ctx = modbus_new_rtu("/dev/ttySP0", 9600, 'N', 8, 1);
	if(ctx == NULL){
		printf("init the libmodbus error.\n");
		return -1;
	}

	modbus_set_debug(ctx, pdev->slave_id);  //enable the debug info
	modbus_set_slave(ctx, 1);  //set slave id

	//connect the device
	if(modbus_connect(ctx) == -1){
		fprintf(stderr, "connect dev failed:%s\n", modbus_strerror(errno));
		modbus_close(ctx);
		modbus_free(ctx);
		return -1;
	}


	while(1){
		
		for(i = 0; i < 5; i++){
			rtu_para = pdev->rtu_data[i];
			modbus_set_slave(ctx, rtu_para->slave_id);
			rc = modbus_read_registers(ctx, rtu_para->start_addr,
									rtu_para->unit_len, rtu_para->rx_buf);
			if(rc == -1){
				fprintf(stderr, "%s\n", modbus_strerror(errno));
				modbus_close(ctx);
				modbus_free(ctx);
				return -1;
			}else{
				for(j = 0; j < 10; j++)
					printf("reg[%d] = %d(0x%x)\n", j, rtu_para->rx_buf[j], rtu_para->rx_buf[j]);
			}

		sleep(2);
	}

	modbus_close(ctx);
	modbus_free(ctx);

	return 0;
}
