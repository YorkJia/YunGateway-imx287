#include "mb_rtu.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "modbus.h"


void *mb_rtu_master_thread(void *arg)
{
	uint16_t tab_reg[100] = {0};
	modbus_t *ctx = NULL;
	int rc, i;

	DEV_PARA *pdev;
	pdev = (DEV_PARA *)arg;

	Pthread_detach(pthread_self());
	/* print the arg */
	printf("%d|", pdev->index);
		printf("%s|", pdev->name);
		printf("%s|", pdev->driver);
		printf("%d|", pdev->rem_para1);
		printf("%s|", pdev->rem_para2);
		printf("%s|", pdev->rem_para3);
		printf("%s|", pdev->loc_para1);
		printf("%s|", pdev->loc_para2);
		printf("%d|", pdev->com_index);
		printf("%d|", pdev->data_type);
		printf("%s|", pdev->rw_type);
		printf("%s|", pdev->unit_para1);
		printf("%s|", pdev->unit_para2);
		printf("%d|", pdev->data_para);
		printf("%d|", pdev->start_addr);
		printf("%d|", pdev->unit_len);
		printf("%d|", pdev->data_hold);
		printf("%d|", pdev->scan_cycle);
		printf("%d|", pdev->log); 
		printf("%d\n", pdev->repeat);
	//init the libmodbus
	ctx = modbus_new_rtu("/dev/ttySP0", 9600, 'N', 8, 1);
	if(ctx == NULL){
		printf("init the libmodbus error.\n");
		return -1;
	}

	modbus_set_debug(ctx, 1);  //enable the debug info
	modbus_set_slave(ctx, 1);  //set slave id

	//connect the device
	if(modbus_connect(ctx) == -1){
		fprintf(stderr, "connect dev failed:%s\n", modbus_strerror(errno));
		modbus_close(ctx);
		modbus_free(ctx);
		return -1;
	}


	while(1){
		printf("\n---------------------------\n");
		rc = modbus_read_registers(ctx, 0, 10, tab_reg);
		if(rc == -1){
			fprintf(stderr, "%s\n", modbus_strerror(errno));
			modbus_close(ctx);
			modbus_free(ctx);
			return -1;
		}

		for(i = 0; i < 10; i++)
			printf("reg[%d] = %d(0x%x)\n", i, tab_reg[i], tab_reg[i]);

		sleep(2);
	}

	modbus_close(ctx);
	modbus_free(ctx);

	return 0;
}
