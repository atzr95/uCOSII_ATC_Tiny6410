#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "..\includes\config.h"
#include "..\includes\typedef1.h"
#include "..\includes\s3c64x0.h"
#include "..\..\uCOS_II\SOURCE\ucos_ii.h"



#ifdef CONFIG_SERIAL1
#define UART_NR	S3C64XX_UART0

#elif defined(CONFIG_SERIAL2)
#define UART_NR	S3C64XX_UART1

#elif defined(CONFIG_SERIAL3)
#define UART_NR	S3C64XX_UART2

#elif defined(CONFIG_SERIAL4)
#define UART_NR	S3C64XX_UART3

#else
#error "Bad: you didn't configure serial ..."
#endif

static OS_EVENT *UartSem;

//static int be_quiet = 0;

static S3C64XX_UART * S3C64XX_GetBase_UART(S3C64XX_UARTS_NR nr)
{
	return (S3C64XX_UART *)(ELFIN_UART_BASE + (nr*0x400));
}


void Uart_putc(const char c)
{
	S3C64XX_UART *const uart = S3C64XX_GetBase_UART(UART_NR);

	INT8U err;

#ifdef CONFIG_MODEM_SUPPORT
	if (be_quiet)
		return;
#endif

	OSSemPend(UartSem, 0, &err);

	/* wait for room in the tx FIFO */
	while (!(uart->UTRSTAT & 0x2));

#ifdef CONFIG_HWFLOW
	/* Wait for CTS up */
	while (hwflow && !(uart->UMSTAT & 0x1));
#endif

	uart->UTXH = c;

	/* If \n, also do \r */
	if (c == '\n')
	{
		OSSemPost(UartSem);
		Uart_putc('\r');
	}
	else
	{
		OSSemPost(UartSem);
	}
}

void Uart_init(void)
{
	S3C64XX_UART *const uart = S3C64XX_GetBase_UART(UART_NR);

    // UART I/O port initialize (RXD0 : GPA0, TXD0: GPA1)
    rGPACON = (rGPACON & ~(0xff<<0)) | (0x22<<0);    // GPA0->RXD0, GPA1->TXD0
    rGPAPUD = (rGPAPUD & ~(0xf<<0)) | (0x1<<0);        // RXD0: Pull-down, TXD0: pull up/down disable

    // Initialize UART Ch0
    uart->ULCON = (0<<6)|(0<<3)|(0<<2)|(3<<0);                    // Normal Mode, No Parity, 1 Stop Bit, 8 Bit Data
    uart->UCON = (0<<10)|(1<<9)|(1<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(1<<2)|(1<<0);    // PCLK divide, Polling Mode
    uart->UFCON = (0<<6)|(0<<4)|(0<<2)|(0<<1)|(0<<0);            // Disable FIFO
    uart->UMCON = (0<<5)|(0<<4)|(0<<0);                        // Disable Auto Flow Control

    uart->UBRDIV = 35;                                    // Baud rate
    uart->UDIVSLOT = 0x80;//aSlotTable[DivSlot];
}

void Uart_SendString(char *s)
{
	while (*s) {
		Uart_putc (*s++);
	}
}
void Uart_SendHex(unsigned int dat)
{
	char str[30];
	sprintf(str,"0x%08x ", dat);
	Uart_SendString(str);

}
void Uart_Printf(const char *fmt,...)
{
    va_list ap;
    char string[512];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    va_end(ap);
    Uart_SendString(string);
}
