#include "..\..\uCOS_II\SOURCE\ucos_ii.h"

#include "..\includes\s3c64x0.h"
#include "..\includes\config.h"

#include "..\..\uCOS_II\API\app_cfg.h"
#include "..\includes\uart.h"
#include "..\includes\gpio.h"
#include "..\..\Printf\Printf.h"
#include "..\includes\mmu.h"
#include "..\includes\malloc.h"

int timer_init(void);
void ISRInit(void);


// Global declarations
OS_STK  MainTaskStk[MainTaskStkLength];

/**** Don't EDIT the following function, unless you know what you are doing ****/
int main(int argc, char **argv)
{

	Uart_init();
	Port_Init();
	timer_init();

	MMU_Init();

	mem_init();	// init heap memory

	OSInit ();
	OSTimeSet(0);
	OSTaskCreate (MainTask,(void *)0, &MainTaskStk[MainTaskStkLength - 1], MainTaskPrio);
	OSStart();

	for(;;);

}
/*******************************************************************************/
