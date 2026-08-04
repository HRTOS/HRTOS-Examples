#include "hrtos.h"

#define MUTEX_ID 0

sbit LED_T1 = P1^0;
sbit LED_T2 = P1^1;
sbit LED_CS = P1^2;   // critical section indicator（核心）

/* 任务1 */
void task1(void)
{
    while(1)
    {
        if(os_mutex_lock(MUTEX_ID))
        {
            /* ===== critical section ===== */
            LED_T1 = 1;
            LED_CS = 1;   // 表示资源被占用

            os_delay(3);  // 模拟临界区执行

            LED_T1 = 0;
            LED_CS = 0;

            os_mutex_unlock(MUTEX_ID);
        }
        else
        {
            /* 没拿到锁：等待状态 */
            LED_T1 = ~LED_T1;  // 闪烁表示竞争失败
        }

        os_delay(4);
    }
}

/* 任务2 */
void task2(void)
{
    while(1)
    {
        if(os_mutex_lock(MUTEX_ID))
        {
            /* ===== critical section ===== */
            LED_T2 = 1;
            LED_CS = 1;

            os_delay(3);

            LED_T2 = 0;
            LED_CS = 0;

            os_mutex_unlock(MUTEX_ID);
        }
        else
        {
            /* 等待锁 */
            LED_T2 = ~LED_T2;
        }

        os_delay(4);
    }
}

/* 系统入口 */
void hrtos_main(void)
{
    LED_T1 = 0;
    LED_T2 = 0;
    LED_CS = 0;

    os_mutex_init(MUTEX_ID);

    os_task_create((unsigned int)task1, 1, 3, 5);
    os_task_create((unsigned int)task2, 2, 2, 5);
}