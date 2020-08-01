#ifndef APP_CFG_H
#define APP_CFG_H

#include "app_cfg_atc.h" /* include file for semaphore example */

#define	TaskUartStkLength	1024    	// Define the TaskUart stack length

void    TaskUart(void *pdata);          //Task Uart
#define TaskUartPrio  	TaskLowestPrio

#endif
