#include "hrtos.h"

/*==================== 配置 ====================*/
#define TASK_A       1
#define TASK_B       2

#define PRIO_A       4
#define PRIO_B       4

#define STACK_A      5
#define STACK_B      5

/*==================== LED 定义 ====================*/
sbit LED_A = P1^0;   /* 任务A指示灯 */
sbit LED_B = P1^1;   /* 任务B指示灯 */

/*==================== 全局 ====================*/
static u16 count_a = 0;
static u16 count_b = 0;

/*==================== 任务A ====================*/
static void task_a(void)
{
    LED_A = 1;

    while (1)
    {
        count_a++;

        LED_A = ~LED_A;
    }
}

/*==================== 任务B ====================*/
static void task_b(void)
{
    LED_B = 1;

    while (1)
    {
        count_b++;

        LED_B = ~LED_B;
    }
}

/*==================== 系统入口 ====================*/
void hrtos_main(void)
{
    if (os_task_create(task_a, TASK_A, PRIO_A, STACK_A) != 1)
    {
        while (1);
    }

    if (os_task_create(task_b, TASK_B, PRIO_B, STACK_B) != 1)
    {
        while (1);
    }
}