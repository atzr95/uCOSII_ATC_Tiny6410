#ifndef _GPIO_H_
#define _GPIO_H_

#define LED1 0x1
#define LED2 0x2
#define LED3 0x4
#define LED4 0x8
#define LED5 0x10
#define LED6 0x20

#define KEY_OFF 0
#define KEY_ON 1


#define KEY_1 0
#define KEY_2 1
#define KEY_3 2
#define KEY_4 3
#define KEY_5 4
#define KEY_6 5
#define KEY_7 6
#define KEY_8 7
#define KEYS_NUM (KEY_8+1)

#define INPUT_K1 0x01
#define INPUT_K2 0x02
#define INPUT_K3 0x04
#define INPUT_K4 0x08
#define INPUT_K5 0x10
#define INPUT_K6 0x20
#define INPUT_K7 0x101
#define INPUT_K8 0x102

#include "s3c64x0.h"

void Port_Init(void);
void Led_Display(int data);
void LedE_Display(int data);
void LedM_Display(int data);
void LedQ_Display(int data);
char Get_Key_Input(int data);

#endif

