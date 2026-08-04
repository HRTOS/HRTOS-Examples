#include "hrtos.h"

sbit LED_A = P1^0;
sbit LED_B = P1^1;
sbit LED_C = P1^2;

void task_a(void)
{
    while(1)
    {
        LED_A = ~LED_A;
        os_task_yield();
    }
}

void task_b(void)
{
    while(1)
    {
        LED_B = ~LED_B;
        os_task_yield();
    }
}

void task_c(void)
{
    while(1)
    {
        LED_C = ~LED_C;
        os_task_yield();
    }
}

void hrtos_main(void)
{
    LED_A = LED_B = LED_C = 0;

    os_task_create((unsigned int)task_a, 1, 1, 5);
    os_task_create((unsigned int)task_b, 2, 1, 5);
    os_task_create((unsigned int)task_c, 3, 1, 5);
}