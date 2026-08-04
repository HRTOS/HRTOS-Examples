#include "hrtos.h"

sbit LED_RUN = P1^0;

/* 任务：状态演示 */
void task_demo(void)
{
    while(1)
    {
        /* 运行态：进入CPU */
        LED_RUN = 1;

        os_delay(3);   // 进入延时 → 阻塞态

        /* 从延时恢复 */
        LED_RUN = 0;

        os_delay(3);
    }
}

/* 系统入口 */
void hrtos_main(void)
{
    LED_RUN = 0;

    os_task_create((unsigned int)task_demo, 1, 2, 5);
}