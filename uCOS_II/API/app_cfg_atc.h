#ifndef APP_CFG_ATC_H
#define APP_CFG_ATC_H

//Task Stk Length
#define	MainTaskStkLength	1024*2   	// Define the MainTask stack length
#define	ATCStkLength	    1024*2     	// Define the ATC stack length
#define	WaitStkLength	    1024*2     	// Define the Wait stack length
#define	TerminalStkLength	1024*2     	// Define the Terminal stack length
#define ServiceStkLength    1024*2      // Define the Service stack length
#define ServiceAStkLength   1024*2      // Define the Service A stack length
#define ServiceBStkLength   1024*2      // Define the Service B stack length
#define ServiceCStkLength   1024*2      // Define the Service C stack length
#define ServiceDStkLength   1024*2      // Define the Service D stack length
#define	DepartStkLength		1024*2     	// Define the Depart stack length

//Task function
void    MainTask(void *pdata);   		// MainTask task
void 	ATC(void *pdata);			    // ATC task
void 	wait_aisle(void *pdata);	    // wait aisle task
void 	Terminal(void *pdata);			// Terminal task
void    Service(void *pdata);		    // Service task
void    ServiceA(void *pdata);          // ServiceA task
void    ServiceB(void *pdata);          // ServiceB task
void    ServiceC(void *pdata);          // ServiceC task
void    ServiceD(void *pdata);          // ServiceD task
void 	Depart(void *pdata);			// Depart task
void    status();
void    LED_status();

//Task Prio
#define NormalTaskPrio       5
#define MainTaskPrio 	NormalTaskPrio
#define ATCPrio 		NormalTaskPrio+1
#define WaitPrio 		NormalTaskPrio+2
#define TerminalPrio 	NormalTaskPrio+3
#define ServicePrio 	NormalTaskPrio+4
#define ServiceAPrio     NormalTaskPrio+5
#define ServiceBPrio     NormalTaskPrio+6
#define ServiceCPrio     NormalTaskPrio+7
#define ServiceDPrio     NormalTaskPrio+8
#define DepartPrio 		NormalTaskPrio+9
#define TaskLowestPrio  NormalTaskPrio+10
#endif
