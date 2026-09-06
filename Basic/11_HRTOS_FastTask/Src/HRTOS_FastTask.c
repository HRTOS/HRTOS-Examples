#include "hrtos.h"

/*==================== 配置 ====================*/
#define TASK_FAST_A   16
#define TASK_FAST_B   17

#define PRIO_FAST     8

#define STACK_FAST_A  5
#define STACK_FAST_B  5

/*==================== LED 定义 ====================*/
sbit LED_FAST_A = P1^0;   /* 高速任务A指示灯 */
sbit LED_FAST_B = P1^1;   /* 高速任务B指示灯 */

/*==================== 全局 ====================*/
static u16 count_a = 0;
static u16 count_b = 0;

/*==================== 高速任务A ====================*/
static void fast_task_a(void)
{
    LED_FAST_A = 1;

    while (1)
    {
        count_a++;

        /* 高速任务A运行 → 翻转LED */
        LED_FAST_A = ~LED_FAST_A;
    }
}

/*==================== 高速任务B ====================*/
static void fast_task_b(void)
{
    LED_FAST_B = 1;

    while (1)
    {
        count_b++;

        /* 高速任务B运行 → 翻转LED */
        LED_FAST_B = ~LED_FAST_B;
    }
}

/*==================== 系统入口 ====================*/
void hrtos_main(void)
{
    if (os_task_create(fast_task_a, TASK_FAST_A, PRIO_FAST, STACK_FAST_A) != 1)
    {
        while (1);
    }

    if (os_task_create(fast_task_b, TASK_FAST_B, PRIO_FAST, STACK_FAST_B) != 1)
    {
        while (1);
    }
}