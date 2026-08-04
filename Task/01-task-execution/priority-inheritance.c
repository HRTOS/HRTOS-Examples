#include "hrtos.h"

#define RES_LOCK_ID  1

sbit LED_LOW  = P1^0;
sbit LED_HIGH = P1^1;

/* 低优先级任务：持锁 + 长时间执行 */
void task_low(void)
{
    os_mutex_lock(RES_LOCK_ID);

    LED_LOW = 1;   // 进入临界区标记

    /* 模拟临界区占用资源 */
    os_delay(30);

    LED_LOW = 0;

    os_mutex_unlock(RES_LOCK_ID);

    while(1)
    {
        os_task_yield();
    }
}

/* 高优先级任务：尝试频繁获取锁 */
void task_high(void)
{
    while(1)
    {
        if(os_mutex_lock(RES_LOCK_ID) == 0)
        {
            LED_HIGH = 1;   // 成功获取锁（说明低任务已释放）
            os_delay(5);
            LED_HIGH = 0;

            os_mutex_unlock(RES_LOCK_ID);
        }
        else
        {
            /* 未获得锁：等待（优先级继承期间会被影响调度） */
            LED_HIGH = ~LED_HIGH;
            os_delay(2);
        }

        os_task_yield();
    }
}

/* 系统入口 */
void hrtos_main(void)
{
    LED_LOW = 0;
    LED_HIGH = 0;

    os_mutex_init(RES_LOCK_ID);

    os_task_create((unsigned int)task_low,  1, 1, 5);  // 低优先级
    os_task_create((unsigned int)task_high, 2, 3, 5);  // 高优先级
}