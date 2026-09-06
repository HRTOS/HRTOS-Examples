#include "hrtos.h"

/*==================== 配置 ====================*/
#define TASK_TARGET   1
#define TASK_TEST     2

#define PRIO_TARGET   5
#define PRIO_TEST     4

#define STACK_TARGET  5
#define STACK_TEST    5

#define TICK_TARGET   20
#define TICK_TEST     50

/*==================== LED 定义 ====================*/
sbit LED_TARGET = P1^0;   /* 目标任务运行指示灯 */
sbit LED_TEST   = P1^1;   /* 删除结果指示灯 */

/*==================== 全局 ====================*/
static u8 delete_result = 0;

/*==================== 目标任务 ====================*/
static void target_task(void)
{
    LED_TARGET = 1;   /* 初始灭灯 */

    while (1)
    {
        LED_TARGET = ~LED_TARGET;

        os_delay(TICK_TARGET);
    }
}

/*==================== 删除测试任务 ====================*/
static void test_task(void)
{
    LED_TEST = 1;   /* 初始灭灯 */

    while (1)
    {
        /*
         * 删除目标任务
         * HRTOS采用标记删除机制，
         * 最终由IDLE任务完成任务清理。
         */
        if (os_task_delete(TASK_TARGET) == 1)
        {
            /*
             * 主动延时，让出CPU，使IDLE获得运行机会，
             * 完成目标任务的最终删除和资源释放。
             */
            os_delay(TICK_TEST);

            /*
             * 删除完成后重新查询任务状态。
             * 0 = 已删除
             */
            if (os_task_get_state(TASK_TARGET) == 0)
            {
                delete_result = 1;

                /* 删除成功 → 翻转LED */
                LED_TEST = ~LED_TEST;
            }
            else
            {
                delete_result = 2;
            }

            /*
             * 删除测试只执行一次。
             * 后续保持结果状态。
             */
            while (1)
            {
                os_delay(TICK_TEST);
            }
        }

        os_delay(TICK_TEST);
    }
}

/*==================== 系统入口 ====================*/
void hrtos_main(void)
{
    if (os_task_create(target_task, TASK_TARGET, PRIO_TARGET, STACK_TARGET) != 1)
    {
        while (1) ;
    }

    if (os_task_create(test_task, TASK_TEST, PRIO_TEST, STACK_TEST) != 1)
    {
        while (1) ;
    }
}