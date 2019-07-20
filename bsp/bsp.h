#ifndef __BSP_H
#define __BSP_H

#include <wraperror.h>
//#include <sqlite3.h>
//#include <wrapsqlite.h>
#include <wrappthread.h>




typedef struct member{
	int index;
        char name[32];
	char driver[16];
	int rem_para1;
	char rem_para2[16];
	char rem_para3[16];
        char loc_para1[16];
	char loc_para2[16];
	int com_index;
	int data_type;
	char rw_type[3];
	char unit_para1[3];
	char unit_para2[3];
	int data_para;
	int start_addr;
	int unit_len;
	int data_hold;
	int scan_cycle;
	int log; 
	int repeat;
} DEV_PARA;


#endif
