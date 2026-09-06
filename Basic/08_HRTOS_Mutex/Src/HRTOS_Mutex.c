#include "hrtos.h"

/*==================== 配置 ====================*/
#define TASK_LOW       1
#define TASK_HIGH      2

#define PRIO_LOW       3
#define PRIO_HIGH      6

#define STACK_LOW      5
#define STACK_HIGH     5

#define MUTEX_KEY      1

#define TICK_LOW       50
#define TICK_HIGH      100

/*==================== LED 定义 ====================*/
sbit LED_LOW  = P1^0;   /* 低优先级任务指示灯 */
sbit LED_HIGH = P1^1;   /* 高优先级任务指示灯 */

/*==================== 全局 ====================*/
static u8 mutex_count = 0;

/*==================== 低优先级任务 ====================*/
static void low_task(void)
{
    LED_LOW = 1;

    while (1)
    {
        /*
         * 获取互斥锁
         */
        if (os_mutex_lock(MUTEX_KEY) == 1)
        {
            mutex_count++;

            /*
             * 持有互斥锁，模拟访问共享资源
             */
            LED_LOW = ~LED_LOW;

            os_delay(TICK_LOW);

            /*
             * 释放互斥锁
             */
            os_mutex_unlock(MUTEX_KEY);
        }

        os_delay(20);
    }
}

/*==================== 高优先级任务 ====================*/
static void high_task(void)
{
    LED_HIGH = 1;

    while (1)
    {
        /*
         * 等待低优先级任务持有互斥锁
         */
        os_delay(TICK_HIGH);

        /*
         * 尝试获取互斥锁
         *
         * 如果互斥锁已经被低优先级任务占用，
         * 系统执行优先级继承。
         */
        if (os_mutex_lock(MUTEX_KEY) == 1)
        {
            /*
             * 高优先级任务获得互斥锁
             */
            LED_HIGH = ~LED_HIGH;

            /*
             * 模拟访问共享资源
             */
            os_delay(20);

            /*
             * 只有成功获取锁的任务才能释放
             */
            os_mutex_unlock(MUTEX_KEY);
        }
    }
}

/*==================== 系统入口 ====================*/
void hrtos_main(void)
{
    if (os_mutex_init(MUTEX_KEY) != 1)
    {
        while (1);
    }

    if (os_task_create(low_task, TASK_LOW, PRIO_LOW, STACK_LOW) != 1)
    {
        while (1);
    }

    if (os_task_create(high_task, TASK_HIGH, PRIO_HIGH, STACK_HIGH) != 1)
    {
        while (1);
    }
}