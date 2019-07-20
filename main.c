#include <unp.h>
#include <bsp.h>
#include "mb_rtu.h"

//#include <sqlite3.h>


//sqlite3 *gDb = NULL;

int
main(void)
{
	int cnt = 0;
	pthread_t tid[10];
	int i;
	_THREAD_PARA *thread_para = NULL;
	
	thread_para = (_THREAD_PARA *)malloc(sizeof(_THREAD_PARA));

	thread_para->dev_num = 5;
	thread_para->rtu_data = (_RTU_DRIVER **)malloc(5*sizeof(_RTU_DRIVER *));

	for(i = 0; i < 5; i++)
		thread_para->rtu_data[i] = (_RTU_DRIVER *)malloc(sizeof(_RTU_DRIVER));


	for(i = 0; i < 5; i++){
		 
		thread_para->rtu_data[i]->slave_id = i + 1;
		thread_para->rtu_data[i]->start_addr = 0;
		thread_para->rtu_data[i]->unit_len = 10;
		thread_para->rtu_data[i]->cycle = 2;
		thread_para->rtu_data[i]->rx_buf = (u16 *)malloc(10*sizeof(u16));
	}
	/*
	thread_para->index = 1;

    thread_para->name[0] = 'd';
    thread_para->name[1] = 'e';
    thread_para->name[2] = 'v';
    thread_para->name[3] = '1';
    thread_para->name[4] = '\0';
	
    thread_para->driver[0] = 'R';
	thread_para->driver[1] = 'T';
	thread_para->driver[2] = 'U';
	thread_para->driver[3] = '\0';
	thread_para->rem_para1 = 1;
	thread_para->rem_para2[16] = "test";
	thread_para->rem_para3[16] = "test";
    thread_para->loc_para1[16] = "test";
	thread_para->loc_para2[16] = "test";
	thread_para->com_index = 1;
	thread_para->data_type = 3;
	thread_para->rw_type[3] = "WR";
	thread_para->unit_para1[3] = "U";
	thread_para->unit_para2[3] = "U";
	thread_para->data_para = 1;
	thread_para->start_addr = 1;
	thread_para->unit_len = 100;
	thread_para->data_hold = 1;
	thread_para->scan_cycle = 3000;
	thread_para->log = 0; 
	thread_para->repeat = 3;
	*/

	pthread_create(&tid[0], NULL, mb_rtu_master_thread, (void *)thread_para);

	while(1){
		cnt++;
		printf("main task cnt:%d\n", cnt);
		

		sleep(5);
	}
	return 0;
}
