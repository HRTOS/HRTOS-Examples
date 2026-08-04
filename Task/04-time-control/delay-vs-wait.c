#include "hrtos.h"

#define EVENT_ID 1

sbit LED_DELAY  = P1^0;
sbit LED_WAIT   = P1^1;
sbit LED_EVENT  = P1^2;

/* 时间驱动任务 */
void task_delay(void)
{
    while(1)
    {
        LED_DELAY = ~LED_DELAY;   // 时间流动节奏

        os_delay(10);
    }
}

/* 事件等待任务 */
void task_wait(void)
{
    while(1)
    {
        u16 i;
        /* 等待事件 */
        if(os_event_wait(EVENT_ID, 0))
        {
            /* 事件触发成功 */
            LED_WAIT = ~LED_WAIT;   // 短暂亮起表示被唤醒
			
			os_event_delete(EVENT_ID);
			
			i=10000;
			
			while(i--);
        }
    }
}

/* 事件触发任务（模拟ISR/生产者） */
void task_event_trigger(void)
{
    while(1)
    {
        os_delay(20);

        os_event_write(EVENT_ID);

        /* 事件触发瞬间提示 */
        LED_EVENT = ~LED_EVENT;
    }
}

/* 系统入口 */
void hrtos_main(void)
{
    LED_DELAY = 0;
    LED_WAIT = 0;
    LED_EVENT = 0;

    os_event_init(EVENT_ID);

    os_task_create((unsigned int)task_delay,        1, 2, 5);
    os_task_create((unsigned int)task_wait,         2, 3, 5);
    os_task_create((unsigned int)task_event_trigger,3, 2, 5);
}