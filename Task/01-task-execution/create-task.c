#include "hrtos.h"

#define TASK_ID  1

sbit LED = P1^0;

/* 任务函数 */
void task_demo(void)
{
    u16 i;
	while(1)
    {
        LED = ~LED;   // 任务运行心跳灯
        
        os_task_yield();
		
		i=20000;
		
		while(i--);
    }
}

/* 系统初始化 */
void hrtos_main(void)
{

    /* 创建任务 */
    os_task_create((unsigned int)task_demo, TASK_ID, 3, 5);

    
}