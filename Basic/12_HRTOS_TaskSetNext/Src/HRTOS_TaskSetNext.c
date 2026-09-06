#include "hrtos.h"

/*==================== 配置 ====================*/
#define TASK_NORMAL   1
#define TASK_FAST     16

#define PRIO_NORMAL   4
#define PRIO_FAST     8

#define STACK_NORMAL  5
#define STACK_FAST    5

#define TICK_NORMAL   50

/*==================== LED 定义 ====================*/
sbit LED_NORMAL = P1^0;   /* 普通任务指示灯 */
sbit LED_FAST   = P1^1;   /* 高速任务指示灯 */

/*==================== 全局 ====================*/
static u16 normal_count = 0;
static u16 fast_count = 0;

/*==================== 普通任务 ====================*/
static void normal_task(void)
{
    LED_NORMAL = 1;

    while (1)
    {
        normal_count++;

        /* 普通任务运行 → 翻转LED */
        LED_NORMAL = ~LED_NORMAL;

        os_delay_ms(TICK_NORMAL);
    }
}

/*==================== 高速任务 ====================*/
static void fast_task(void)
{
    LED_FAST = 1;

    while (1)
    {
        fast_count++;

        /* 高速任务运行 → 翻转LED */
        LED_FAST = ~LED_FAST;

        /*
         * 指定普通任务作为下一个运行任务
         * 终止正常调度，强制切换到普通任务
         */
        os_task_set_next(TASK_NORMAL);
    }
}

/*==================== 系统入口 ====================*/
void hrtos_main(void)
{
    if (os_task_create(normal_task, TASK_NORMAL, PRIO_NORMAL, STACK_NORMAL) != 1)
    {
        while (1);
    }

    if (os_task_create(fast_task, TASK_FAST, PRIO_FAST, STACK_FAST) != 1)
    {
        while (1);
    }
}