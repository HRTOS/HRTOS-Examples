#include "hrtos.h"

/* =========================
 * LED
 * ========================= */
sbit LED_OK   = P1^0;
sbit LED_FAIL = P1^1;

/* 模拟资源池 */
#define MAX_BLOCK 4

unsigned char pool[MAX_BLOCK];

/* =========================================================
 * 资源分配演示任务
 * ========================================================= */
void task_allocation_demo(void)
{
    unsigned char i;

    while(1)
    {
        /* 模拟资源申请 */
        for(i = 0; i < MAX_BLOCK; i++)
        {
            if(pool[i] == 0)
            {
                pool[i] = 1;

                LED_OK = ~LED_OK;
                break;
            }
        }

        /* 没有资源 */
        if(i == MAX_BLOCK)
        {
            LED_FAIL = ~LED_FAIL;
        }

        os_delay(50);
    }
}

/* =========================================================
 * 入口
 * ========================================================= */
void hrtos_main(void)
{
    unsigned char i;

    for(i = 0; i < MAX_BLOCK; i++)
        pool[i] = 0;

    LED_OK = 0;
    LED_FAIL = 0;

    os_task_create((unsigned int)task_allocation_demo, 1, 3, 5);
}