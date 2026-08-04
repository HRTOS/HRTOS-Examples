#include "hrtos.h"

sbit LED_RT = P1^0;

void task_rt(void)
{
    while(1)
    {
        LED_RT = 1;      // 运行窗口
        os_delay(10);     // 模拟实时执行窗口

        LED_RT = 0;      // 等待周期
        os_delay(40);
    }
}

void hrtos_main(void)
{
    LED_RT = 0;

    os_task_create((unsigned int)task_rt, 1, 2, 5);
}