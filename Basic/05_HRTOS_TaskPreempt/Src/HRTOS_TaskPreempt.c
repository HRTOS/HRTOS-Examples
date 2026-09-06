#include "hrtos.h"

/*==================== 配置 ====================*/
#define TASK_LOW      1
#define TASK_HIGH     2
#define TASK_SEND     3

#define PRIO_LOW      3
#define PRIO_HIGH     6
#define PRIO_SEND     4

#define STACK_LOW     5
#define STACK_HIGH    5
#define STACK_SEND    5

#define EVENT_HIGH    1

#define TICK_SEND     100

/*==================== LED 定义 ====================*/
sbit LED_LOW  = P1^0;   /* 低优先级任务指示灯 */
sbit LED_HIGH = P1^1;   /* 高优先级任务指示灯 */
sbit LED_SEND = P1^2;   /* 发送任务指示灯 */

/*==================== 全局 ====================*/
static u8 high_count = 0;

/*==================== 低优先级任务 ====================*/
static void low_task(void)
{
    LED_LOW = 1;

    while (1)
    {
        /*
         * 低优先级任务持续运行。
         * 当高优先级任务变为就绪状态时，
         * 当前任务会立即被高优先级任务抢占。
         */
        LED_LOW = ~LED_LOW;
    }
}

/*==================== 高优先级任务 ====================*/
static void high_task(void)
{
    LED_HIGH = 1;

    while (1)
    {
        /*
         * 等待事件。
         * 收到事件后，高优先级任务立即参与调度。
         */
        if (os_event_wait(EVENT_HIGH, 0) == 1)
        {
            high_count++;

            /* 高优先级任务运行 → 翻转LED */
            LED_HIGH = ~LED_HIGH;

            /*
             * 延时后再次进入等待，
             * 使低优先级任务重新获得CPU。
             */
            os_delay(20);
        }
    }
}

/*==================== 事件发送任务 ====================*/
static void send_task(void)
{
    LED_SEND = 1;

    while (1)
    {
        /*
         * 发送事件，使高优先级任务变为就绪状态。
         */
        os_event_write(EVENT_HIGH);

        LED_SEND = ~LED_SEND;

        os_delay(TICK_SEND);
    }
}

/*==================== 系统入口 ====================*/
void hrtos_main(void)
{
    os_event_init(EVENT_HIGH);

    if (os_task_create(low_task, TASK_LOW, PRIO_LOW, STACK_LOW) != 1)
    {
        while (1) ;
    }

    if (os_task_create(high_task, TASK_HIGH, PRIO_HIGH, STACK_HIGH) != 1)
    {
        while (1) ;
    }

    if (os_task_create(send_task, TASK_SEND, PRIO_SEND, STACK_SEND) != 1)
    {
        while (1) ;
    }
}