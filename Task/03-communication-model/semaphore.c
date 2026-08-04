#include "hrtos.h"

#define SEM_ID 0

sbit LED_PRODUCER = P1^0;
sbit LED_CONSUMER = P1^1;
sbit LED_SEM      = P1^2;   // semaphore state indicator

/* 生产者任务 */
void task_A(void)
{
    while(1)
    {
        os_delay(2);

        os_sem_post(SEM_ID);

        LED_PRODUCER = ~LED_PRODUCER;  // 生产节奏闪烁

        LED_SEM = 1;   // 表示资源“被释放/可用”
    }
}

/* 消费者任务 */
void task_B(void)
{
    while(1)
    {
        /* 等待资源 */
        if(os_sem_wait(SEM_ID))
        {
            /* 获取资源成功 */
            LED_CONSUMER = ~LED_CONSUMER;  // 消费节奏

            LED_SEM = 0;   // 表示资源被消耗

            /* 模拟处理 */
            os_delay(1);
        }

        os_delay(1);
    }
}

/* 系统入口 */
void hrtos_main(void)
{
    LED_PRODUCER = 0;
    LED_CONSUMER = 0;
    LED_SEM = 0;

    os_sem_init(SEM_ID, 0);

    os_task_create((unsigned int)task_A, 1, 3, 5);
    os_task_create((unsigned int)task_B, 2, 2, 5);
}