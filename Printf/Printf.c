
//********************************************************************
//OSPrintf post message to UartQ
//TaskUart send message to Uart from UartQ
//tangxiaofeng xidian 503
//********************************************************************

#include <stdarg.h>
#include "..\uCOS_II\SOURCE\ucos_ii.h"
#include <stdio.h>
#include "..\uCOS_II\API\app_cfg.h"
#include "..\S3C6410\includes\uart.h"

#define UartMsgNum  1024
#define UartMsgBufLength 512

OS_STK	TaskUartStk [TaskUartStkLength];       // Define the TaskUart stack

static OS_MEM *pUartMem;
static INT8U  UartBuf[UartMsgNum][UartMsgBufLength];
static OS_EVENT *pUart_Q;
static void *MsgUart[UartMsgNum];


void OSPrintfInit(void)
{
	INT8U err;
	pUartMem=OSMemCreate(UartBuf,UartMsgNum,UartMsgBufLength,&err); //Create a mem for Uart post Msg

	pUart_Q=OSQCreate(&MsgUart[0],UartMsgNum);	//Create a Queue for Uart

	OSTaskCreate(TaskUart,(void*)0,&TaskUartStk[TaskUartStkLength-1],TaskUartPrio);//Creat a Task to sent Msg to Uart
}

void OSPrintf(const char *fmt,...)//post massage to UartQ
{

    char *pUartBuf;
    INT8U err;
    va_list ap;

    pUartBuf=OSMemGet(pUartMem,&err);//Reqire a Mem for Msg
    va_start(ap,fmt);
    vsprintf(pUartBuf,fmt,ap);//Msg formate
    va_end(ap);
    //Uart_SendString(string);
    OSQPost(pUart_Q,pUartBuf);//Post Msg
}

void TaskUart(void *pdata) //send message to uart from uartQ
{

	char *pUartMsg;
	INT8U err;
	pdata=pdata;



	while(1)
	{
	    pUartMsg=OSQAccept(pUart_Q,&err);//Accept the Msg from Queue; no wait
		while(pUartMsg)
			{
				Uart_SendString(pUartMsg);//seng str to Uart
				OSMemPut(pUartMem,pUartMsg);
				pUartMsg=OSQAccept(pUart_Q,&err);
			}


		OSTimeDly(OS_TICKS_PER_SEC/20);

    }
}
