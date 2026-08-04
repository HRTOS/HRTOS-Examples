#include "hrtos.h"

sbit LED_HIGH = P1^0;
sbit LED_LOW  = P1^1;

/* 高优先级任务：持续占用CPU */
void task_high(void)
{
    u16 i;
	while(1)
    {
        LED_HIGH = ~LED_HIGH;   // 高频闪烁（CPU主导者）

        os_task_yield();        // 仍然让出，但因优先级高持续被选中
		
		i=20000;
		
		while(i--);
    }
}

/* 低优先级任务：可能长期无法执行 */
void task_low(void)
{
    while(1)
    {
        LED_LOW = 1;            // 试图点亮

        os_delay(10);           // 被延迟放大“饥饿现象”

        LED_LOW = 0;

        os_delay(10);
    }
}

/* 系统入口 */
void hrtos_main(void)
{
    LED_HIGH = 0;
    LED_LOW = 0;

    /* 高优先级任务 */
    os_task_create((unsigned int)task_high, 1, 4, 5);

    /* 低优先级任务 */
    os_task_create((unsigned int)task_low,  2, 1, 5);
}