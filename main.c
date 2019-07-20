#include <unp.h>
#include <bsp.h>
#include "mb_rtu.h"

//#include <sqlite3.h>


//sqlite3 *gDb = NULL;




int
main(void)
{
	
	DEV_PARA *thread_para;
	pthread_t tid[10];
	int cnt = 0;
	thread_para = (DEV_PARA *)malloc(sizeof(DEV_PARA)*1);

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
	
	pthread_create(&tid[0], NULL, mb_rtu_master_thread, (void *)thread_para);

	while(1){
		cnt++;
		printf("the main app running:%d\n", cnt);
		sleep(5);
	}
	return 0;
}
