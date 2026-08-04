#include "hrtos.h"

#define TASK_A 1
#define TASK_B 2

sbit LED_A = P1^0;
sbit LED_B = P1^1;

/* Task A */
void task_A(void)
{
	u16 i;
	P1=0XFF;
    while(1)
    {
        LED_A = ~LED_A;   // ÇÐ»»AµÆ×´Ì¬
        i=2000;
		while(i--);

    }
}

/* Task B */
void task_B(void)
{
	u16 i;
    while(1)
    {
        LED_B = ~LED_B;   // ÇÐ»»BµÆ×´Ì¬        
        i=2000;
		while(i--);        
		
    }
}

/* ÏµÍ³Èë¿Ú */
void hrtos_main(void)
{

    os_task_create((unsigned int)task_A, TASK_A, 3, 2);
    os_task_create((unsigned int)task_B, TASK_B, 3, 2);
	
}