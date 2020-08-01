/*
 * (C) Copyright 2003
 * David Müller ELSOFT AG Switzerland. d.mueller@elsoft.ch
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/************************************************
 * NAME	    : S3C64XX.h
 * Version  : 31.3.2003
 *
 * common stuff for SAMSUNG S3C64XX SoC
 ************************************************/

#ifndef __S3C64XX_H__
#define __S3C64XX_H__
#include "typedef1.h"

typedef vu_char		S3C64XX_REG8;
typedef vu_short	S3C64XX_REG16;
typedef vu_long		S3C64XX_REG32;


//@0x33FF_Fe20
#define pISR_EINT0		(*(unsigned *)(_ISR_STARTADDRESS+0x20))
#define pISR_EINT1		(*(unsigned *)(_ISR_STARTADDRESS+0x24))
#define pISR_RTC_TIC	(*(unsigned *)(_ISR_STARTADDRESS+0x28))
#define pISR_CAMIF_C	(*(unsigned *)(_ISR_STARTADDRESS+0x2c))
#define pISR_CAMIF_P	(*(unsigned *)(_ISR_STARTADDRESS+0x30))
#define pISR_I2C1		(*(unsigned *)(_ISR_STARTADDRESS+0x34))
#define pISR_I2S		(*(unsigned *)(_ISR_STARTADDRESS+0x38))
#define pISR_Reserved1	(*(unsigned *)(_ISR_STARTADDRESS+0x3c))
#define pISR_3D			(*(unsigned *)(_ISR_STARTADDRESS+0x40))
#define pISR_Post0		(*(unsigned *)(_ISR_STARTADDRESS+0x44))
#define pISR_ROTATOR	(*(unsigned *)(_ISR_STARTADDRESS+0x48))
#define pISR_2D			(*(unsigned *)(_ISR_STARTADDRESS+0x4c))
#define pISR_TVENC		(*(unsigned *)(_ISR_STARTADDRESS+0x50))
#define pISR_SCALER		(*(unsigned *)(_ISR_STARTADDRESS+0x54))
#define pISR_BATF		(*(unsigned *)(_ISR_STARTADDRESS+0x58))
#define pISR_JPEG		(*(unsigned *)(_ISR_STARTADDRESS+0x5c))


//@0x33FF_Fe60

#define pISR_MFC		(*(unsigned *)(_ISR_STARTADDRESS+0x60))
#define pISR_SDMA0		(*(unsigned *)(_ISR_STARTADDRESS+0x64))
#define pISR_SDMA1		(*(unsigned *)(_ISR_STARTADDRESS+0x68))
#define pISR_ARM_DMAERR	(*(unsigned *)(_ISR_STARTADDRESS+0x6c))
#define pISR_ARM_DMA	(*(unsigned *)(_ISR_STARTADDRESS+0x70))
#define pISR_ARM_DMAS	(*(unsigned *)(_ISR_STARTADDRESS+0x74))
#define pISR_KEYPAD		(*(unsigned *)(_ISR_STARTADDRESS+0x78))
#define pISR_TIMER0		(*(unsigned *)(_ISR_STARTADDRESS+0x7c))
#define pISR_TIMER1		(*(unsigned *)(_ISR_STARTADDRESS+0x80))
#define pISR_TIMER2		(*(unsigned *)(_ISR_STARTADDRESS+0x84))
#define pISR_WDT		(*(unsigned *)(_ISR_STARTADDRESS+0x88))
#define pISR_TIMER3		(*(unsigned *)(_ISR_STARTADDRESS+0x8c))
#define pISR_TIMER4		(*(unsigned *)(_ISR_STARTADDRESS+0x90))
#define pISR_LCD0		(*(unsigned *)(_ISR_STARTADDRESS+0x94))
#define pISR_LCD1		(*(unsigned *)(_ISR_STARTADDRESS+0x98))
#define pISR_LCD2		(*(unsigned *)(_ISR_STARTADDRESS+0x9c))

//@0x33FF_Fea0

#define pISR_EINT2		(*(unsigned *)(_ISR_STARTADDRESS+0xa0))
#define pISR_EINT3		(*(unsigned *)(_ISR_STARTADDRESS+0xa4))
#define pISR_PCM0		(*(unsigned *)(_ISR_STARTADDRESS+0xa8))
#define pISR_PCM1		(*(unsigned *)(_ISR_STARTADDRESS+0xac))
#define pISR_AC97		(*(unsigned *)(_ISR_STARTADDRESS+0xb0))
#define pISR_UART0		(*(unsigned *)(_ISR_STARTADDRESS+0xb4))
#define pISR_UART1		(*(unsigned *)(_ISR_STARTADDRESS+0xb8))
#define pISR_UART2		(*(unsigned *)(_ISR_STARTADDRESS+0xbc))
#define pISR_UART3		(*(unsigned *)(_ISR_STARTADDRESS+0xc0))
#define pISR_DMA0		(*(unsigned *)(_ISR_STARTADDRESS+0xc4))
#define pISR_DMA1		(*(unsigned *)(_ISR_STARTADDRESS+0xc8))
#define pISR_ONENAND0	(*(unsigned *)(_ISR_STARTADDRESS+0xcc))
#define pISR_ONENAND1	(*(unsigned *)(_ISR_STARTADDRESS+0xd0))
#define pISR_NFC		(*(unsigned *)(_ISR_STARTADDRESS+0xd4))
#define pISR_CFC		(*(unsigned *)(_ISR_STARTADDRESS+0xd8))
#define pISR_UHOST		(*(unsigned *)(_ISR_STARTADDRESS+0xdc))

//@0x33FF_Fee0

#define pISR_SPI0		(*(unsigned *)(_ISR_STARTADDRESS+0xe0))
#define pISR_SPI1_HSMMC2	(*(unsigned *)(_ISR_STARTADDRESS+0xe4))
#define pISR_I2C0		(*(unsigned *)(_ISR_STARTADDRESS+0xe8))
#define pISR_HSItx		(*(unsigned *)(_ISR_STARTADDRESS+0xec))
#define pISR_HSIrx		(*(unsigned *)(_ISR_STARTADDRESS+0xf0))
#define pISR_EINT4		(*(unsigned *)(_ISR_STARTADDRESS+0xf4))
#define pISR_MSM		(*(unsigned *)(_ISR_STARTADDRESS+0xf8))
#define pISR_HOSTIF		(*(unsigned *)(_ISR_STARTADDRESS+0xfc))
#define pISR_HSMMC0		(*(unsigned *)(_ISR_STARTADDRESS+0x100))
#define pISR_HSMMC1		(*(unsigned *)(_ISR_STARTADDRESS+0x104))
#define pISR_OTG		(*(unsigned *)(_ISR_STARTADDRESS+0x108))
#define pISR_IrDA		(*(unsigned *)(_ISR_STARTADDRESS+0x10c))
#define pISR_RTC_ALARM	(*(unsigned *)(_ISR_STARTADDRESS+0x110))
#define pISR_SEC		(*(unsigned *)(_ISR_STARTADDRESS+0x114))
#define pISR_PENDNUP	(*(unsigned *)(_ISR_STARTADDRESS+0x118))
#define pISR_ADC		(*(unsigned *)(_ISR_STARTADDRESS+0x11c))


// GPIO
#define GPIO_BASE    (0x7F008000)

#define rGPACON        (*(volatile unsigned *)(GPIO_BASE+0x00))
#define rGPADAT        (*(volatile unsigned *)(GPIO_BASE+0x04))
#define rGPAPUD        (*(volatile unsigned *)(GPIO_BASE+0x08))

#define rGPBCON        (*(volatile unsigned *)(GPIO_BASE+0x20))
#define rGPBDAT        (*(volatile unsigned *)(GPIO_BASE+0x24))
#define rGPBPUD        (*(volatile unsigned *)(GPIO_BASE+0x28))

#define rGPECON         (*(volatile unsigned *)(GPIO_BASE+0x80)) 
#define rGPEDAT         (*(volatile unsigned *)(GPIO_BASE+0x84)) 
#define rGPEPUD         (*(volatile unsigned *)(GPIO_BASE+0x88))
#define rGPECONSLP		(*(volatile unsigned *)(GPIO_BASE+0x8C))
#define rGPEPUDSLP		(*(volatile unsigned *)(GPIO_BASE+0x90)) 

#define rGPFCON         (*(volatile unsigned *)(GPIO_BASE+0xA0))
#define rGPFDAT         (*(volatile unsigned *)(GPIO_BASE+0xA4))
#define rGPFPUD         (*(volatile unsigned *)(GPIO_BASE+0xA8))
#define rGPFCONSLP     (*(volatile unsigned *)(GPIO_BASE+0xAC))
#define rGPFPUDSLP      (*(volatile unsigned *)(GPIO_BASE+0xB0))

#define rGPKCON0        (*(volatile unsigned *)(GPIO_BASE+0x800))
#define rGPKCON1        (*(volatile unsigned *)(GPIO_BASE+0x804))
#define rGPKDAT         (*(volatile unsigned *)(GPIO_BASE+0x808))
#define rGPKPUD         (*(volatile unsigned *)(GPIO_BASE+0x80C))

#define rGPLCON0        (*(volatile unsigned *)(GPIO_BASE+0x810))
#define rGPLCON1        (*(volatile unsigned *)(GPIO_BASE+0x814))
#define rGPLDAT         (*(volatile unsigned *)(GPIO_BASE+0x818))
#define rGPLPUD         (*(volatile unsigned *)(GPIO_BASE+0x81C))

#define rGPMCON          (*(volatile unsigned *)(GPIO_BASE+0x820))
#define rGPMDAT          (*(volatile unsigned *)(GPIO_BASE+0x824))
#define rGPMPUD          (*(volatile unsigned *)(GPIO_BASE+0x828))

#define rGPNCON         (*(volatile unsigned *)(GPIO_BASE+0x830))
#define rGPNDAT         (*(volatile unsigned *)(GPIO_BASE+0x834))
#define rGPNPUD         (*(volatile unsigned *)(GPIO_BASE+0x838))

#define rGPQCON         (*(volatile unsigned *)(GPIO_BASE+0x180))
#define rGPQDAT         (*(volatile unsigned *)(GPIO_BASE+0x184))
#define rGPQPUD         (*(volatile unsigned *)(GPIO_BASE+0x188))
#define rGPQCONSLP		(*(volatile unsigned *)(GPIO_BASE+0x18C))
#define rGPQPUDSLP		(*(volatile unsigned *)(GPIO_BASE+0x190))

#define ELFIN_UART_BASE		0x7F005000

typedef enum {
	S3C64XX_UART0,
	S3C64XX_UART1,
	S3C64XX_UART2,
	S3C64XX_UART3
} S3C64XX_UARTS_NR;


/* UART (see manual chapter 11) */
typedef struct {
	S3C64XX_REG32	ULCON;
	S3C64XX_REG32	UCON;
	S3C64XX_REG32	UFCON;
	S3C64XX_REG32	UMCON;
	S3C64XX_REG32	UTRSTAT;
	S3C64XX_REG32	UERSTAT;
	S3C64XX_REG32	UFSTAT;
	S3C64XX_REG32	UMSTAT;
#ifdef __BIG_ENDIAN
	S3C64XX_REG8	res1[3];
	S3C64XX_REG8	UTXH;
	S3C64XX_REG8	res2[3];
	S3C64XX_REG8	URXH;
#else /* Little Endian */
	S3C64XX_REG8	UTXH;
	S3C64XX_REG8	res1[3];
	S3C64XX_REG8	URXH;
	S3C64XX_REG8	res2[3];
#endif
	S3C64XX_REG32	UBRDIV;
	S3C64XX_REG32	UDIVSLOT;
	S3C64XX_REG32	UINTP;
	S3C64XX_REG32	UINTSP;
	S3C64XX_REG32	UINTM;
} /*__attribute__((__packed__))*/ S3C64XX_UART;


#define ELFIN_VIC0_BASE			0x71200000
#define ELFIN_VIC1_BASE			0x71300000

#define ELFIN_CLOCK_POWER_BASE	0x7e00f000
#define ELFIN_DMA_BASE		0x75000000
#define ELFIN_LCD_BASE		0x77100000
#define ELFIN_USB_HOST_BASE	0x74300000
#define ELFIN_I2C_BASE		0x7f004000
#define ELFIN_I2S_BASE		0x7f002000
#define ELFIN_ADC_BASE		0x7e00b000
#define ELFIN_SPI_BASE		0x7f00b000
#define ELFIN_HSMMC_0_BASE	0x7c200000
#define ELFIN_HSMMC_1_BASE	0x7c300000
#define ELFIN_HSMMC_2_BASE	0x7c400000

#define ELFIN_CLOCK_POWER_BASE	0x7e00f000

/* Clock & Power Controller for mDirac3*/
#define APLL_LOCK_OFFSET	0x00
#define MPLL_LOCK_OFFSET	0x04
#define EPLL_LOCK_OFFSET	0x08
#define APLL_CON_OFFSET		0x0C
#define MPLL_CON_OFFSET		0x10
#define EPLL_CON0_OFFSET	0x14
#define EPLL_CON1_OFFSET	0x18
#define CLK_SRC_OFFSET		0x1C
#define CLK_DIV0_OFFSET		0x20
#define CLK_DIV1_OFFSET		0x24
#define CLK_DIV2_OFFSET		0x28
#define CLK_OUT_OFFSET		0x2C
#define HCLK_GATE_OFFSET	0x30
#define PCLK_GATE_OFFSET	0x34
#define SCLK_GATE_OFFSET	0x38
#define AHB_CON0_OFFSET		0x100
#define AHB_CON1_OFFSET		0x104
#define AHB_CON2_OFFSET		0x108
#define SELECT_DMA_OFFSET	0x110
#define SW_RST_OFFSET		0x114
#define SYS_ID_OFFSET		0x118
#define MEM_SYS_CFG_OFFSET	0x120
#define QOS_OVERRIDE0_OFFSET	0x124
#define QOS_OVERRIDE1_OFFSET	0x128
#define MEM_CFG_STAT_OFFSET	0x12C
#define PWR_CFG_OFFSET		0x804
#define EINT_MASK_OFFSET	0x808
#define NOR_CFG_OFFSET		0x810
#define STOP_CFG_OFFSET		0x814
#define SLEEP_CFG_OFFSET	0x818
#define OSC_FREQ_OFFSET		0x820
#define OSC_STABLE_OFFSET	0x824
#define PWR_STABLE_OFFSET	0x828
#define FPC_STABLE_OFFSET	0x82C
#define MTC_STABLE_OFFSET	0x830
#define OTHERS_OFFSET		0x900
#define RST_STAT_OFFSET		0x904
#define WAKEUP_STAT_OFFSET	0x908
#define BLK_PWR_STAT_OFFSET	0x90C
#define INF_REG0_OFFSET		0xA00
#define INF_REG1_OFFSET		0xA04
#define INF_REG2_OFFSET		0xA08
#define INF_REG3_OFFSET		0xA0C
#define INF_REG4_OFFSET		0xA10
#define INF_REG5_OFFSET		0xA14
#define INF_REG6_OFFSET		0xA18
#define INF_REG7_OFFSET		0xA1C

#define OSC_CNT_VAL_OFFSET	0x824
#define PWR_CNT_VAL_OFFSET	0x828
#define FPC_CNT_VAL_OFFSET	0x82C
#define MTC_CNT_VAL_OFFSET	0x830

#define CLK_DIV0_REG		__REG(ELFIN_CLOCK_POWER_BASE+CLK_DIV0_OFFSET)
#define OTHERS_REG		__REG(ELFIN_CLOCK_POWER_BASE+OTHERS_OFFSET)

#define ELFIN_TIMER_BASE	0x7F006000

#define TCON_4_AUTO		(1 << 22)       /* auto reload on/off for Timer 4 */
#define TCON_4_UPDATE		(1 << 21)       /* manual Update TCNTB4 */
#define TCON_4_ONOFF		(1 << 20)       /* 0: Stop, 1: start Timer 4 */
#define COUNT_4_ON		(TCON_4_ONOFF*1)

/* PWM TIMER (see manual chapter 10) */
typedef struct {
	S3C64XX_REG32	TCNTB;
	S3C64XX_REG32	TCMPB;
	S3C64XX_REG32	TCNTO;
} /*__attribute__((__packed__))*/ S3C64XX_TIMER;

typedef struct {
	S3C64XX_REG32	TCFG0;
	S3C64XX_REG32	TCFG1;
	S3C64XX_REG32	TCON;
	S3C64XX_TIMER	ch[4];		
	S3C64XX_REG32	TCNTB4;
	S3C64XX_REG32	TCNTO4;
	S3C64XX_REG32	CSTAT;
} /*__attribute__((__packed__))*/ S3C64XX_TIMERS;


#define APLL_CON_REG		__REG(ELFIN_CLOCK_POWER_BASE+APLL_CON_OFFSET)
#define MPLL_CON_REG		__REG(ELFIN_CLOCK_POWER_BASE+MPLL_CON_OFFSET)
#define EPLL_CON0_REG		__REG(ELFIN_CLOCK_POWER_BASE+EPLL_CON0_OFFSET)
#define EPLL_CON1_REG		__REG(ELFIN_CLOCK_POWER_BASE+EPLL_CON1_OFFSET)

/* INTERRUPT (see manual chapter 14) */
typedef struct {
	S3C64XX_REG32	IRQSTATUS;
	S3C64XX_REG32	FIQSTATUS;
	S3C64XX_REG32	RAWINTR;
	S3C64XX_REG32	INTSELECT;
	S3C64XX_REG32	INTENABLE;
	S3C64XX_REG32	INTENCLEAR;
	S3C64XX_REG32	INTSOFTINT;
	S3C64XX_REG32	INTSOFTINTCLEAR;
	S3C64XX_REG32	PROTECTION;
	S3C64XX_REG32	SWPRIORITYMASK;
	S3C64XX_REG32	PRIORITYDAISY;
} /*__attribute__((__packed__))*/ S3C64XX_INTERRUPT;

typedef struct {
	S3C64XX_REG32	ADCCON;
	S3C64XX_REG32	ADCTSC;
	S3C64XX_REG32	ADCDLY;
	S3C64XX_REG32	ADCDAT0;
	S3C64XX_REG32	ADCDAT1;
	S3C64XX_REG32	ADCUPDN;
	S3C64XX_REG32	ADCCLRINT;
	S3C64XX_REG32	RESERVED;
	S3C64XX_REG32	ADCCLRINTPNDNUP;
} /*__attribute__((__packed__))*/ S3C64XX_ADC;

#endif /*__S3C64XX_H__*/
