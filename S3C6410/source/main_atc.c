#include "..\..\uCOS_II\SOURCE\ucos_ii.h"
#include "..\includes\s3c64x0.h"
#include "..\includes\config.h"
#include "..\..\uCOS_II\API\app_cfg.h"
#include "..\includes\uart.h"
#include "..\includes\gpio.h"
#include "..\..\Printf\Printf.h"
#include "..\includes\mmu.h"
#include "..\includes\malloc.h"

// Global declarations
OS_STK	ATCStk [ATCStkLength];              // Define the ATC stack
OS_STK	WaitStk [WaitStkLength];            // Define the wait aisle stack
OS_STK	TerminalStk [TerminalStkLength];    // Define the Terminal stack
OS_STK  ServiceStk[ServiceStkLength];       // Define the service stack
OS_STK  ServiceAStk [ServiceAStkLength];    // Define the ServiceA stack
OS_STK  ServiceBStk [ServiceBStkLength];    // Define the ServiceB stack
OS_STK  ServiceCStk [ServiceCStkLength];    // Define the ServiceC stack
OS_STK  ServiceDStk [ServiceDStkLength];    // Define the ServiceD stack
OS_STK	DepartStk [DepartStkLength];        // Define the Depart stack

OS_EVENT *max_capacity_sem; //maximum capacity semaphore
OS_EVENT *wait_aisle_sem;   //waiting aisle semaphore
OS_EVENT *mutex1_sem;       //mutex for plane request to land
OS_EVENT *termA_sem;        //terminal A availability semaphore
OS_EVENT *termB_sem;        //terminal B availability semaphore
OS_EVENT *termC_sem;        //terminal C availability semaphore
OS_EVENT *termD_sem;        //terminal D availability semaphore
OS_EVENT *A_sem;            //start service at terminal A semaphor
OS_EVENT *B_sem;            //start service at terminal B semaphor
OS_EVENT *C_sem;            //start service at terminal C semaphor
OS_EVENT *D_sem;            //start service at terminal D semaphor
OS_EVENT *cap_sem;          //capacity semaphore
OS_EVENT *wait_sem;         //fire code reached semaphore
OS_EVENT *fire_sem;         //fire code semaphore
OS_EVENT *mutex2_sem;       //mutex for service team
OS_EVENT *done_sem;         //finish service semaphore
OS_EVENT *departA_sem;      //depart from lane A semaphore
OS_EVENT *departure;        //departure semaphore
OS_EVENT *departB_sem;      //depart from lane B semaphore
OS_EVENT *service_sem;      //service team semaphore
OS_EVENT *waitaisleQ;       //waiting aisle semaphore
OS_EVENT *serviceQ;         //service FIFO queue
OS_EVENT *serviceAQ;        //service terminal A
OS_EVENT *serviceBQ;        //service terminal B
OS_EVENT *serviceCQ;        //service terminal C
OS_EVENT *serviceDQ;        //service terminal D
void *waitaisle_msg[10];
void *service_msg[4];
void *main_msg[20];
INT8U enter = 0;            //plane entering the terminal
INT8U arrive = 0;           //plane arriving at waiting aisle
INT8U term_arrive = 0;      //plane entering the terminal
int planeCount[21];         //plane id at waiting aisle
int lane[21];               //lane id for servicing FIFO
int terminalCount[21];      //plane id at terminal
char *airport_status;
INT8U counts[20];           //for status reporting on plane id in waiting aisle
char *TA;                       //for status to display plane id in terminal A
char *TB                        //for status to display plane id in terminal B
char *TC;                       //for status to display plane id in terminal C
char *TD;                       //for status to display plane id in terminal D
INT8U term = 0;
INT8U terminalLED = 0x00;
INT8U indicatorLED = 0x00;

// NOTE: MainTask is only for creating other tasks, please don't use it for other purposes
void MainTask(void *pdata) //Main Task create various tasks
{

#if OS_CRITICAL_METHOD == 3    /* Allocate storage for CPU status register */
   OS_CPU_SR  cpu_sr;
#endif

	INT16U Version = OSVersion();

	OS_ENTER_CRITICAL();
	ISRInit();   // initial interrupt prio or enable or disable
	OS_EXIT_CRITICAL();

	OSStatInit();

	OSPrintfInit();		// use task to print massage to Uart

    OSPrintf("********************************\n");
    OSPrintf("********************************\n");
	OSPrintf("\n");
    OSPrintf("     uC/OS-II Version:V%d.%d\n",Version/100, Version % 100);	// print uC/OS-II version
	OSPrintf("\n");
    OSPrintf("********************************\n");
    OSPrintf("********************************\n");

	/* init user defined global variables here */
	max_capacity_sem = OSSemCreate(20);
	wait_aisle_sem = OSSemCreate(6);
	termA_sem = OSSemCreate(1);
	termB_sem = OSSemCreate(1);
	termC_sem = OSSemCreate(1);
	termD_sem = OSSemCreate(1);
	A_sem = OSSemCreate(0);
	B_sem = OSSemCreate(0);
	C_sem = OSSemCreate(0);
	D_sem = OSSemCreate(0);
	cap_sem = OSSemCreate(0);
	wait_sem = OSSemCreate(0);
	fire_sem = OSSemCreate(11);
	mutex1_sem = OSSemCreate(0);
	mutex2_sem = OSSemCreate(1);
	done_sem = OSSemCreate(0);
	waitaisleQ = OSQCreate(&waitaisle_msg[0], 6);
	serviceQ = OSQCreate(&main_msg[0], 20);
	serviceAQ = OSQCreate(&service_msg[0], 2);
	serviceBQ = OSQCreate(&service_msg[0], 2);
	serviceCQ = OSQCreate(&service_msg[0], 2);
	serviceDQ = OSQCreate(&service_msg[0], 2);
	departA_sem = OSSemCreate(10);
	departB_sem = OSSemCreate(0);
	departure = OSSemCreate(0);
	service_sem = OSSemCreate(1);
	/*******************************************/

	/*** Create user defined task here ***/
	OSTaskCreate (ATC,(void *)0, &ATCStk[ATCStkLength - 1], ATCPrio);
    OSTaskCreate (wait_aisle,(void *)0, &WaitStk[WaitStkLength - 1], WaitPrio);
    OSTaskCreate (Depart,(void *)0, &DepartStk[DepartStkLength - 1], DepartPrio);
    OSTaskCreate (Terminal,(void *)0, &TerminalStk[TerminalStkLength - 1], TerminalPrio);
	OSTaskCreate (Service, (void *)0, &ServiceStk[ServiceStkLength - 1], ServicePrio);
	OSTaskCreate (ServiceA,(void *)0, &ServiceAStk[ServiceAStkLength - 1], ServiceAPrio);
	OSTaskCreate (ServiceB,(void *)0, &ServiceBStk[ServiceBStkLength - 1], ServiceBPrio);
	OSTaskCreate (ServiceC,(void *)0, &ServiceCStk[ServiceCStkLength - 1], ServiceCPrio);
	OSTaskCreate (ServiceD,(void *)0, &ServiceDStk[ServiceDStkLength - 1], ServiceDPrio);
	/*****************************/

	OSPrintf("\nEnter Main Task\n");
	while(1)
	{
		OSTimeDly(OS_TICKS_PER_SEC);
	}

}

// ATC checks if airport has available space for planes
void ATC (void *pdata)
{
    OS_TCB taskinfo;
	int input_dat[KEYS_NUM] = {INPUT_K1, INPUT_K2, INPUT_K3, INPUT_K4, INPUT_K5, INPUT_K6, INPUT_K7, INPUT_K8};
	char input_state[KEYS_NUM];
	char input_prevstate[KEYS_NUM];
	int i;
	INT8U err_plane;
	LedM_Display(0);    //Port M LED
    LedE_Display(0);    //Port E LED
    LedQ_Display(0);    //Port Q LED

	for (i=0; i<KEYS_NUM;i++){
	input_state[i] = Get_Key_Input(input_dat[i]);
    input_prevstate[i] = input_state[i];
	}

    OSPrintf("\n*******AIRPORT OPEN********");
    OSTimeDly(OS_TICKS_PER_SEC/5);

	while(1){
    for (i=0;i<KEYS_NUM;i++){
        input_state[i] = Get_Key_Input(input_dat[i]);
        if(input_state[i] && (input_state[i] ^ input_prevstate[i]))
        {
           if (i==KEY_5){
              INT16U airport_capacity = OSSemAccept(max_capacity_sem);  //check if maximum capacity reached
				if (airport_capacity > 0){
                    OSSemPost(mutex1_sem);
					INT16U cap = OSSemAccept(fire_sem);                //check if fire code reached
					if (cap > 0){
						OSSemPost(cap_sem);
						airport_status = " AIRPORT OPEN ";
					}
					else{
                        OSSemPost(wait_sem);    //post wait_sem when fire code reached
					}
				}
				else{
					if (enter == 20){
                        OSPrintf("\n*******AIRPORT CLOSED********");
                        buzzer(0x01);
                        OSTimeDly(OS_TICKS_PER_SEC);
                        buzzer(0x00);
                    }
				}
				 OSTimeDly(OS_TICKS_PER_SEC/10);
             }

             if (i==KEY_4){
                status();
             }
         }
       input_prevstate[i]= input_state[i];
    }
    OSTimeDly(OS_TICKS_PER_SEC/5);
    }
}


// wait aisle task controls the plane arrival
void wait_aisle	(void *pdata)
{
        INT8U err_mutex;
        INT8U err;
		INT8U parrive = 0;          //plane arriving
		INT8U plane = 0;
		INT8U plane_landed = 0;
		INT16U aisle_capacity = 6;
		INT8U err_wait;
		OS_SEM_DATA sem_data;

    while(1){
        INT16U mux = OSSemAccept(mutex1_sem);
			if (mux > 0){
				INT8U fire_code = OSSemAccept(cap_sem);            //to check fire code
				if (fire_code > 0){
					plane++;
					aisle_capacity = OSSemAccept(wait_aisle_sem);   //to check the availability of aisle
					if (aisle_capacity > 0){
						parrive++;
						planeCount[parrive] = parrive;
						if (parrive == 20){
                            airport_status = "AIRPORT CLOSED";
                            indicatorLED = indicatorLED|LED1;
                            LedQ_Display(indicatorLED);
						}
						err_wait = OSQPost(waitaisleQ, &planeCount[parrive]);   //post airplane to waiting aisle
						if (err_wait == OS_NO_ERR){
							plane_landed++;
							OSPrintf("\n*********Plane %d requests to land*********", plane_landed);
							OSTimeDly(OS_TICKS_PER_SEC / 10);
						}
					}
					else{
						OSSemPost(max_capacity_sem);
						OSTimeDly(OS_TICKS_PER_SEC / 10);
					}
				}
				else{
					INT8U overcap = OSSemAccept(wait_sem);          //to check if fire code reached
					if (overcap > 0){
						if (plane <= 20){
                            OSPrintf("\n************ Fire code reached !!! plane moves to other airport ***********");
                            OSTimeDly(OS_TICKS_PER_SEC / 10);
						}
                    }
                    OSSemPost(max_capacity_sem);
					OSTimeDly(OS_TICKS_PER_SEC / 10);
					}
				}
			OSTimeDly(OS_TICKS_PER_SEC / 10);

			OSSemQuery(wait_aisle_sem,&sem_data);
            LedM_Display(64-(1<<sem_data.OSCnt));           //light up waiting aisle LED

            if(sem_data.OSCnt > 0){
                indicatorLED = (indicatorLED & (~LED2));    //turn off fire code LED when waiting aisle > 0
                LedQ_Display(indicatorLED);
            }
    }
}

//controls assignment of plane to terminals
void Terminal (void *pdata)
{
    void *plane_msg;
	INT8U err;
	INT8U err_A;
	INT8U err_B;
	INT8U err_C;
	INT8U err_D;
	INT8U servicelane = 0;
    OS_SEM_DATA cap_data;

  while(1)
  {
        plane_msg = OSQPend(waitaisleQ, 10, &err);
		if (err == OS_NO_ERR){
			terminalCount[enter] = (*(int *)plane_msg);
			enter++;
			counts[enter] = enter;                  //enter plane id into array
			OSPrintf("\n*********Plane %d lands at waiting aisle*********", enter);
            OSTimeDly(OS_TICKS_PER_SEC / 10);
            buzzer(0x01);
            OSTimeDly(OS_TICKS_PER_SEC/10);
            buzzer(0x00);
		}

        OSSemQuery(fire_sem,&cap_data);       //turn on fire code LED when fire code reached
        if(cap_data.OSCnt == 0){
            indicatorLED = indicatorLED|LED2;
            LedQ_Display(indicatorLED);
        }

		if (arrive < enter){
			OSSemPend(termA_sem, 10, &err_A);     //check availability of terminal A
			if (err_A == OS_NO_ERR){
				OSTimeDly(OS_TICKS_PER_SEC);
				servicelane = 1;
				lane[arrive] = servicelane;
				TA = terminalCount[arrive];
				term_arrive++;
				OSPrintf("\nPlane %d moves to terminal A", terminalCount[arrive]);
				term = 1;
                LED_status();
				OSSemPost(wait_aisle_sem);
				OSTimeDly(OS_TICKS_PER_SEC/5);
				OSQPost(serviceAQ, &terminalCount[arrive]);
				OSQPost(serviceQ, &lane[arrive]);
				arrive++;
			}
			else{
				OSSemPend(termB_sem, 10, &err_B);    //check availability of terminal B
				if (err_B == OS_NO_ERR){
					OSTimeDly(OS_TICKS_PER_SEC);
					servicelane = 2;
					lane[arrive] = servicelane;
					TB = terminalCount[arrive];
					term_arrive++;
                    OSPrintf("\nPlane %d moves to terminal B", terminalCount[arrive]);
					term = 2;
					LED_status();
					OSSemPost(wait_aisle_sem);
					OSTimeDly(OS_TICKS_PER_SEC/5);
					OSQPost(serviceBQ, &terminalCount[arrive]);
					OSQPost(serviceQ, &lane[arrive]);
					arrive++;
				}
				else{
					OSSemPend(termC_sem, 10, &err_C);   //check availability of terminal C
					if (err_C == OS_NO_ERR){
						OSTimeDly(OS_TICKS_PER_SEC);
						servicelane = 3;
						lane[arrive] = servicelane;
						TC = terminalCount[arrive];
						term_arrive++;
						OSPrintf("\nPlane %d moves to terminal C", terminalCount[arrive]);
						term = 3;
						LED_status();
						OSSemPost(wait_aisle_sem);
						OSTimeDly(OS_TICKS_PER_SEC/5);
						OSQPost(serviceCQ, &terminalCount[arrive]);
						OSQPost(serviceQ, &lane[arrive]);
						arrive++;
					}
					else{
						OSSemPend(termD_sem, 10, &err_D);   //check availability of terminal D
						if (err_D == OS_NO_ERR){
							OSTimeDly(OS_TICKS_PER_SEC);
							servicelane = 4;
							lane[arrive] = servicelane;
							TD = terminalCount[arrive];
							term_arrive++;
							OSPrintf("\nPlane %d moves to terminal D", terminalCount[arrive]);
							term = 4;
							LED_status();
							OSSemPost(wait_aisle_sem);
							OSTimeDly(OS_TICKS_PER_SEC/5);
							OSQPost(serviceDQ, &terminalCount[arrive]);
							OSQPost(serviceQ, &lane[arrive]);
							arrive++;
						}
					}
				}
			}
		}
    OSTimeDly(OS_TICKS_PER_SEC/5);
    }
}

//arranged to order to service plane in FIFO
void Service (void *pdata)
{
	INT8U err;
	INT8U service=0;
	INT8U err_done;
	void *service_msg;
	INT8U done=0;
	INT8U ser[20];

	while (1)
	{
        service_msg = OSQPend(serviceQ, 10, &err);
		if (err == OS_NO_ERR){
			ser[service] = (*(int *)service_msg);
			service++;
		}

		if (done < service){
			OSSemPend(mutex2_sem, 100, &err);
			{
				if (err == OS_NO_ERR){
					switch (ser[done])    //check which terminal to service next
					{
					case 1:
						OSSemPost(A_sem);                      //service terminal A
						OSSemPend(done_sem, 10, &err_done);    //wait until servicing done
						done++;
						break;

					case 2:
						OSSemPost(B_sem);                     //service terminal B
						OSSemPend(done_sem, 10, &err_done);   //wait until servicing done
						done++;
						break;

					case 3:
						OSSemPost(C_sem);                     //service terminal C
						OSSemPend(done_sem, 10, &err_done);   //wait until servicing done
						done++;
						break;

					case 4:
						OSSemPost(D_sem);                    //service terminal D
						OSSemPend(done_sem, 10, &err_done);  //wait until servicing done
						done++;
						break;
					}
                }
            }
        }
		OSTimeDly(OS_TICKS_PER_SEC);
	}
}

//service plane at terminal A
void ServiceA (void *pdata)
{
    INT8U servicenumber=0;
	INT8U err;
	INT8U err_ser;
	INT8U err_A;
	INT8U servicelist[20];
	void *service;
	INT8U number=0;

  while(1)
  {
       service = OSQPend(serviceAQ, 100, &err);
		if (err == OS_NO_ERR){
			servicelist[servicenumber] = *((int *)service);
			servicenumber++;
		}

		if (number < servicenumber){
			OSSemPend(A_sem, 0, &err_A);
			if (err_A == OS_NO_ERR)
			{
                OSSemPend(service_sem, 0, &err_ser);   //wait for service crew
                if(err_ser == OS_NO_ERR){
                    OSTimeDly(OS_TICKS_PER_SEC);
                    OSPrintf("\nService & Engineering crew start servicing plane %d at Terminal A", servicelist[number]);
                    OSTimeDly(OS_TICKS_PER_SEC * 5);
                    OSPrintf("\nService & Engineering crew finish servicing plane %d at Terminal A",servicelist[number]);
                    OSPrintf("\n*********Plane %d requests to depart*********",servicelist[number]);
                    OSSemPost(service_sem);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(termA_sem);
                    TA = 0;
					terminalLED = (terminalLED^LED1);
                    LedE_Display(terminalLED);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(done_sem);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(mutex2_sem);
                    number++;
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(departure);                  //indicate plane ready for departure
                }
                 OSTimeDly(OS_TICKS_PER_SEC/10);
            }
		}
        OSTimeDly(OS_TICKS_PER_SEC);
    }
}

//service plane at terminal B
void ServiceB (void *pdata)
{
  INT8U servicenumber = 0;
	INT8U err;
	INT8U err_ser;
	INT8U err_B;
	void *service;
	INT8U servicelist[20];
	INT8U number = 0;

  while(1)
  {
        service = OSQPend(serviceBQ, 100, &err);
		if (err == OS_NO_ERR){
			servicelist[servicenumber] = *((int *)service);
			servicenumber++;
		}

		if (number < servicenumber){
			OSSemPend(B_sem, 0, &err_B);
			if (err_B == OS_NO_ERR)
			{
                OSSemPend(service_sem, 0, &err_ser);         //wait for service crew
                if(err_ser == OS_NO_ERR){
                    OSTimeDly(OS_TICKS_PER_SEC);
                    OSPrintf("\nService & Engineering crew start servicing plane %d at Terminal B", servicelist[number]);
                    OSTimeDly(OS_TICKS_PER_SEC * 5);
                    OSPrintf("\nService & Engineering crew finish servicing plane %d at Terminal B",servicelist[number]);
                    OSPrintf("\n*********Plane %d requests to depart*********",servicelist[number]);
                    OSSemPost(service_sem);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(termB_sem);
                    TB = 0;
					terminalLED = (terminalLED^LED2);
                    LedE_Display(terminalLED);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(done_sem);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(mutex2_sem);
                    number++;
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(departure);                 //indicate plane ready for departure
                }
                 OSTimeDly(OS_TICKS_PER_SEC/10);
            }
        }
        OSTimeDly(OS_TICKS_PER_SEC);

   }
}

//service plane at terminal C
void ServiceC (void *pdata)
{
    INT8U servicenumber = 0;
	INT8U err;
	INT8U err_ser;
	INT8U err_C;
	void *service;
	INT8U servicelist[20];
	INT8U number = 0;

  while(1)
  {
      service = OSQPend(serviceCQ, 100, &err);
		if (err == OS_NO_ERR){
			servicelist[servicenumber] = *((int *)service);
			servicenumber++;
		}

		if (number < servicenumber){
			OSSemPend(C_sem, 0, &err_C);
			if (err_C == OS_NO_ERR)
			{
                OSSemPend(service_sem, 0, &err_ser);      //wait for service crew
                if(err_ser == OS_NO_ERR){
                    OSTimeDly(OS_TICKS_PER_SEC);
                    OSPrintf("\nService & Engineering crew start servicing plane %d at Terminal C", servicelist[number]);
                    OSTimeDly(OS_TICKS_PER_SEC * 5);
                    OSPrintf("\nService & Engineering crew finish servicing plane %d at Terminal C",servicelist[number]);
                    OSPrintf("\n*********Plane %d requests to depart*********",servicelist[number]);
                    OSSemPost(service_sem);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(termC_sem);
                    TC = 0;
					terminalLED = (terminalLED^LED3);
                    LedE_Display(terminalLED);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(done_sem);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(mutex2_sem);
                    number++;
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(departure);               //indicate plane ready for departure
                }
                 OSTimeDly(OS_TICKS_PER_SEC/10);
            }
        }
        OSTimeDly(OS_TICKS_PER_SEC);
    }
}

//service plane at terminal D
void ServiceD (void *pdata)
{
	INT8U servicenumber = 0;
	INT8U err;
	INT8U err_ser;
	INT8U err_D;
	void *service;
	INT8U servicelist[20];
	INT8U number = 0;

  while(1)
  {
      service = OSQPend(serviceDQ, 100, &err);
		if (err == OS_NO_ERR){
			servicelist[servicenumber] = *((int *)service);
			servicenumber++;
		}

		if (number < servicenumber){
			OSSemPend(D_sem, 0, &err_D);
			if (err_D == OS_NO_ERR)
			{
                OSSemPend(service_sem, 0, &err_ser);       //wait for service crew
                if(err_ser == OS_NO_ERR){
                    OSTimeDly(OS_TICKS_PER_SEC);
                    OSPrintf("\nService & Engineering crew start servicing plane %d at Terminal D", servicelist[number]);
                    OSTimeDly(OS_TICKS_PER_SEC * 5);
                    OSPrintf("\nService & Engineering crew finish servicing plane %d at Terminal D",servicelist[number]);
                    OSPrintf("\n*********Plane %d requests to depart*********",servicelist[number]);
                    OSSemPost(service_sem);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(termD_sem);
                    TD = 0;
					terminalLED = (terminalLED^LED4);
                    LedE_Display(terminalLED);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(done_sem);
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(mutex2_sem);
                    number++;
                    OSTimeDly(OS_TICKS_PER_SEC / 5);
                    OSSemPost(departure);            //indicate plane ready for departure
                }
                 OSTimeDly(OS_TICKS_PER_SEC/10);
            }
        }
        OSTimeDly(OS_TICKS_PER_SEC);
    }
}

//control departure of plane from lane A and B
void Depart	(void *pdata)
{
    void *depart_msg;
	INT8U depart[20];
	INT8U departnumber = 0;
	INT8U number = 0;
	INT8U err;
	INT8U err_departA;
    INT8U err_departB;

    while(1)
    {
        OSSemPend(departure,0,&err);
        if (err == OS_NO_ERR)
        {
            number++;
			OSSemPend(departB_sem, 10, &err_departB);
			if (err_departB == OS_NO_ERR){
				OSTimeDly(OS_TICKS_PER_SEC);
				OSPrintf("\nPlane %d has departed from lane B", number);
				OSSemPost(fire_sem);
				indicatorLED = indicatorLED|LED4;
                LedQ_Display(indicatorLED);
                OSTimeDly(OS_TICKS_PER_SEC*2);
                indicatorLED = indicatorLED^LED4;
                LedQ_Display(indicatorLED);
				OSTimeDly(OS_TICKS_PER_SEC / 5);
			}
			else{
				OSSemPend(departA_sem, 10, &err_departA);
				OSTimeDly(OS_TICKS_PER_SEC);
				OSPrintf("\nPlane %d has departed from lane A", number);
				OSSemPost(fire_sem);
				indicatorLED = indicatorLED|LED3;
                LedQ_Display(indicatorLED);
                OSTimeDly(OS_TICKS_PER_SEC*2);
                indicatorLED = indicatorLED^LED3;
                LedQ_Display(indicatorLED);
				OSTimeDly(OS_TICKS_PER_SEC / 5);
				OSSemPost(departB_sem);
			}
            OSTimeDly(OS_TICKS_PER_SEC / 5);
		}
		OSTimeDly(OS_TICKS_PER_SEC / 5);
    }
}

//airport status
void status()
{
	int d = 0;
	int i = 0;
	OSPrintf("\n\n****************************** ANTHONY'S AIRPORT ******************************");
	OSPrintf("\n******************************* %s ********************************", airport_status);
	OSPrintf("\n Waiting Aisle: ");
	for (i = 0; i < (enter - term_arrive); i++){
		if (term_arrive == 20){
			d = 20;
		}
		else{
            d = term_arrive + i + 1;
		}
		OSPrintf("<Plane %d>", counts[d]);
	}

	OSPrintf("\n Terminal A: ");
	if (TA != 0){
		OSPrintf("<Plane %d> ", TA);
	}
	OSPrintf("\n Terminal B: ");
	if (TB != 0){
		OSPrintf("<Plane %d> ", TB);
	}
	OSPrintf("\n Terminal C: ");
	if (TC != 0){
		OSPrintf("<Plane %d> ", TC);
	}
	OSPrintf("\n Terminal D: ");
	if (TD != 0){
		OSPrintf("<Plane %d> ", TD);
	}
	OSPrintf("\n*******************************************************************************\n");
}

//terminal LED
void LED_status()
{
    if (term == 1){
        terminalLED = (terminalLED|LED1);
    }
    else if (term == 2){
        terminalLED = (terminalLED|LED2);
    }
    else if (term == 3){
        terminalLED = (terminalLED|LED3);
    }
    else if (term == 4){
        terminalLED = (terminalLED|LED4);
    }
    else{
        terminalLED = 0;
    }
    LedE_Display(terminalLED);
}

