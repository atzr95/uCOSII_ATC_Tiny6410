#include "..\includes\gpio.h"

void Port_Init(void)
{
	// configure GPIO Port E p1, p2, p3, p4 as Output
	rGPECON = (rGPECON & ~(0xffffU<<4))|(0x1111U<<4);
	// disable GPIO Port E p1, p2, p3, p4 pull-up/down
	rGPEPUD = (rGPEPUD & ~(0xffffU<<2))|(0x00<<2);

	// configure GPIO Port K p4, p5, p6, p7 as OUTPUT
    rGPKCON0 = (rGPKCON0 & ~(0xffffU<<16))|(0x1111U<<16);
    // disable GPIO Port K p4, p5, p6, p7 pull-up/down
    rGPKPUD  = (rGPKPUD  & ~(0xffU << 8))|(0x00U<<8);

	// configure GPIO Port L p11, p12 as INPUT
    rGPLCON1 = (rGPLCON1 & ~(0xffU<<12))|(0x00U<<12);
    // enable GPIO Port L p11, p12 pull-up/down
    rGPLPUD  = (rGPLPUD  & ~(0xfU<<22))|(0x5U<<22);

	// configure GPIO Port M p0, p1, p2, p3, p4. p5 as OUTPUT
    rGPMCON = (rGPMCON & ~0xffffffU)|(0x111111U);
    // disable GPIO Port M p0, p1, p2, p3, p4. p5 pull-up/down
    rGPMPUD  = (rGPMPUD  & ~0xfffU)|(0x000U);

	// configure GPIO Port N p0, p1, p2, p3, p4. p5 as INPUT
    rGPNCON = (rGPNCON & ~0xfffU)|(0x000U);
    // enable GPIO Port N p0, p1, p2, p3, p4. p5 pull-up/down
    rGPNPUD  = (rGPNPUD  & ~0xfffU)|(0x555U);

	// configure GPIO Port Q p1, p2, p3, p4, p5. p6 as OUTPUT
    rGPQCON = (rGPQCON & ~(0xfffU<<2))|(0x555U<<2);
    // disable GPIO Port Q p1, p2, p3, p4, p5. p6 pull-up/down
    rGPQPUD  = (rGPQPUD  & ~(0xfffU<<2))|(0x000U<<2);

    // configure GPIO Port F p14 as OUTPUT
    rGPFCON = (rGPFCON & ~(0b11U<<28))|(0b01U<<28);
    // disable GPIO Port F p14 pull-up/down
    rGPFPUD  = (rGPFPUD  & ~(0x11U<<28))|(0x00U<<28);

}

void Led_Display(int data)
{
    rGPKDAT = (rGPKDAT & ~(0xf<<4)) | (((~data) & 0xf)<<4);
}

void LedE_Display(int data)
{
    rGPEDAT = (rGPEDAT & ~(0xf<<1)) | (((data) & 0xf)<<1);
}

void LedM_Display(int data)
{
    rGPMDAT = (rGPMDAT & ~(0x3f)) | (((data) & 0x3f));
}

void LedQ_Display(int data)
{
     rGPQDAT = (rGPQDAT & ~(0x3f<<1)) | (((data) & 0x3f)<<1);
}

void buzzer(int data)
{
    rGPFDAT = (rGPFDAT & ~(0b1<<14)) | (((data) & 0b1)<<14);
}

char Get_Key_Input(int data)
{
	if(!data)
		return 0;

	if(data < INPUT_K7)
	{
		return !(rGPNDAT & data);
	}

	return !(rGPLDAT & ((data&0x03)<<11));

}
