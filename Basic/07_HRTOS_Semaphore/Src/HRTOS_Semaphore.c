#include "hrtos.h"

/*==================== 配置 ====================*/
#define TASK_A       1
#define TASK_B       2

#define PRIO_A       3
#define PRIO_B       4

#define STACK_A      5
#define STACK_B      5

#define SEM_KEY      1

#define TICK_A       50
#define TICK_B       50

/*==================== LED 定义 ====================*/
sbit LED_A = P1^0;   /* 任务A指示灯 */
sbit LED_B = P1^1;   /* 任务B指示灯 */

/*==================== 全局 ====================*/
static u8 sem_count = 0;

/*==================== 任务A ====================*/
static void task_a(void)
{
    LED_A = 1;

    while (1)
    {
        /*
         * 获取信号量
         */
		//
        if (os_sem_wait(SEM_KEY) == 2)
        {
            sem_count++;

            /* 获取成功 → 翻转LED */
            LED_A = ~LED_A;

            /*
             * 模拟占用资源
             */
            os_delay(TICK_A);

            /*
             * 释放信号量
             */
            os_sem_post(SEM_KEY);
        }

        os_delay(10);
    }
}

/*==================== 任务B ====================*/
static void task_b(void)
{
    LED_B = 1;

    while (1)
    {
        /*
         * 获取信号量
         */
        if (os_sem_wait(SEM_KEY) == 2)
        {
            sem_count++;

            /* 获取成功 → 翻转LED */
            LED_B = ~LED_B;

            /*
             * 模拟占用资源
             */
            os_delay(TICK_B);

            /*
             * 释放信号量
             */
            os_sem_post(SEM_KEY);
        }

        os_delay(10);
    }
}

/*==================== 系统入口 ====================*/
void hrtos_main(void)
{
    if (os_sem_init(SEM_KEY, 1) != 1)
    {
        while (1);
    }

    if (os_task_create(task_a, TASK_A, PRIO_A, STACK_A) != 1)
    {
        while (1);
    }

    if (os_task_create(task_b, TASK_B, PRIO_B, STACK_B) != 1)
    {
        while (1);
    }
	os_sem_post(SEM_KEY);
	//P1=0;
}