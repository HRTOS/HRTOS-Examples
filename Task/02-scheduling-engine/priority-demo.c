#include "hrtos.h"

sbit LED_HIGH = P1^0;
sbit LED_LOW  = P1^1;

void task_low(void)
{
    while(1)
    {
        LED_LOW = ~LED_LOW;   // 慢闪
        os_delay(20);
    }
}

void task_high(void)
{
    while(1)
    {
        LED_HIGH = ~LED_HIGH; // 快闪（体现抢占更频繁）
        os_delay(5);
    }
}

void hrtos_main(void)
{
    LED_LOW = 0;
    LED_HIGH = 0;

    os_task_create((unsigned int)task_low,  1, 1, 5);
    os_task_create((unsigned int)task_high, 2, 3, 5);
}