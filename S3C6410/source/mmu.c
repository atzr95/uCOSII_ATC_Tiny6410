/************************************************
  NAME    : MMU.C
  DESC	  :
  Revision: 2002.2.28 ver 0.0
 ************************************************/

#include "..\includes\uart.h"

#include "..\includes\typedef1.h"
#include "..\includes\mmu.h"
#include "..\includes\config.h"

void MMU_DisableDCache(void);
void MMU_DisableICache(void);
void MMU_CleanInvalidateDCacheIndex(U32 index);
void MMU_InvalidateICache(void);
void MMU_DisableMMU(void);
void MMU_InvalidateTLB(void);
void MMU_SetTTBase(int base);
void MMU_SetDomain(int domain);
void MMU_SetProcessId(U32 pid);
void MMU_EnableAlignFault(void);
void MMU_EnableMMU(void);
void MMU_EnableICache(void);
void MMU_EnableDCache(void);


// 1) Only the section table is used.
// 2) The cachable/non-cachable area can be changed by MMT_DEFAULT value.
//    The section size is 1MB.


extern char __ENTRY[];

void MMU_Init(void)
{
    int i,j;
    // unsigned long * pTT;
    //========================== IMPORTANT NOTE =========================
    //The current stack and code area can't be re-mapped in this routine.
    //If you want memory map mapped freely, your own sophiscated MMU
    //initialization code is needed.
    //===================================================================

    MMU_DisableDCache();
    MMU_DisableICache();

    //If write-back is used,the DCache should be cleared.
    for(i=0;i<64;i++)
    	for(j=0;j<8;j++)
    	    MMU_CleanInvalidateDCacheIndex((i<<26)|(j<<5));
    MMU_InvalidateICache();

    #if 0
    //To complete MMU_Init() fast, Icache may be turned on here.
    MMU_EnableICache();
    #endif

    MMU_DisableMMU();
    MMU_InvalidateTLB();

    // MMU_SetMTT(0x00000000,0xffffffff,0,RW_FAULT);  //bank0


    MMU_SetMTT(0x00000000,0x08000000,(int)__ENTRY,RW_NCNB);  //bank0
    MMU_SetMTT(0x50000000,0x58000000,0x50000000,RW_NCNB);  //bank0
//    MMU_SetMTT(0x60000000,0x68000000,0x60000000,RW_NCNB);  // heap memory starting address


    MMU_SetMTT(0x70000000,0x70400000,0x70000000,RW_NCNB);  //bank0
    MMU_SetMTT(0x71000000,0x71400000,0x71000000,RW_NCNB);  //bank0

    MMU_SetMTT(0x72000000,0x73200000,0x72000000,RW_NCNB);  //bank0
    MMU_SetMTT(0x74000000,0x74500000,0x74000000,RW_NCNB);  //bank0
    MMU_SetMTT(0x75000000,0x75400000,0x75000000,RW_NCNB);  //bank0
    MMU_SetMTT(0x76000000,0x76400000,0x76000000,RW_NCNB);  //bank0
    MMU_SetMTT(0x77000000,0x77300000,0x77000000,RW_NCNB);  //bank0
    MMU_SetMTT(0x78000000,0x78c00000,0x78000000,RW_NCNB);  //bank0
    MMU_SetMTT(0x7c000000,0x7c500000,0x7c000000,RW_NCNB);  //bank0
    MMU_SetMTT(0x7d000000,0x7e100000,0x7d000000,RW_NCNB);  //bank0

    MMU_SetMTT(0x7f000000,0x7f100000,0x7f000000,RW_NCNB);  //bank0


    MMU_SetMTT(0xc0000000,0xc8000000,0x50000000,RW_NCNB);

    MMU_SetTTBase(_MMUTT_STARTADDRESS);
    MMU_SetDomain(0x55555550|DOMAIN1_ATTR|DOMAIN0_ATTR);
    	//DOMAIN1: no_access, DOMAIN0,2~15=client(AP is checked)
   	MMU_SetProcessId(0x0);
    MMU_EnableAlignFault();

    MMU_EnableMMU();

    MMU_EnableICache();
    MMU_EnableDCache(); //DCache should be turned on after MMU is turned on.
}


// attr=RW_CB,RW_CNB,RW_NCNB,RW_FAULT
void ChangeRomCacheStatus(int attr)
{
    int i,j;
    MMU_DisableDCache();
    MMU_DisableICache();
    //If write-back is used,the DCache should be cleared.
    for(i=0;i<64;i++)
    	for(j=0;j<8;j++)
    	    MMU_CleanInvalidateDCacheIndex((i<<26)|(j<<5));
    MMU_InvalidateICache();
    MMU_DisableMMU();
    MMU_InvalidateTLB();
    MMU_SetMTT(0x00000000,0x07f00000,0x00000000,attr);	//bank0
    MMU_SetMTT(0x08000000,0x0ff00000,0x08000000,attr);	//bank1
    MMU_EnableMMU();
    MMU_EnableICache();
    MMU_EnableDCache();
}


void MMU_SetMTT(unsigned int vaddrStart,unsigned int vaddrEnd,unsigned int paddrStart,unsigned int attr)
{
    volatile U32 *pTT;
    volatile int i,nSec;
    pTT=(volatile U32 *)_MMUTT_STARTADDRESS+(vaddrStart>>20);
    nSec=(vaddrEnd>>20)-(vaddrStart>>20);

    for(i=0;i<=nSec;i++)*pTT++=attr |(((paddrStart>>20)+i)<<20);
}






