#include "hrtos.h"

/*==================== 配置 ====================*/
#define TASK_WAIT     1
#define TASK_SEND     2

#define PRIO_WAIT     5
#define PRIO_SEND     4

#define STACK_WAIT    5
#define STACK_SEND    5

#define EVENT_KEY     1

#define TICK_WAIT     50
#define TICK_SEND     20

/*==================== LED 定义 ====================*/
sbit LED_WAIT = P1^0;   /* 等待任务指示灯 */
sbit LED_SEND = P1^1;   /* 发送任务指示灯 */

/*==================== 全局 ====================*/
static u8 event_count = 0;

/*==================== 等待任务 ====================*/
static void wait_task(void)
{
    LED_WAIT = 1;   /* 初始灭灯（8051低电平亮可自行改） */

    while (1)
    {
		
		if (os_event_wait(EVENT_KEY, TICK_WAIT) == 1)
        {
            event_count++;

            LED_WAIT = ~LED_WAIT;   /* 收到事件 → 翻转LED */
        }
		
		os_event_delete(EVENT_KEY);/* 删除事件 */
    }
}

/*==================== 发送任务 ====================*/
static void send_task(void)
{
    LED_SEND = 1;

    while (1)
    {
        os_event_write(EVENT_KEY);

        LED_SEND = ~LED_SEND;   /* 每次发送 → 翻转LED */

        os_delay(TICK_SEND);
    }
}

/*==================== 系统入口 ====================*/
void hrtos_main(void)
{
    os_event_init(EVENT_KEY);

    if (os_task_create(wait_task, TASK_WAIT, PRIO_WAIT, STACK_WAIT) != 1)
    {
        while (1) os_delay(20);
    }

    if (os_task_create(send_task, TASK_SEND, PRIO_SEND, STACK_SEND) != 1)
    {
        while (1) os_delay(20);
    }
}