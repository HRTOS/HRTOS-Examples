#include "hrtos.h"

/* =========================
 * LED
 * ========================= */
sbit LED_FRAG = P1^0;

/* 简单模拟内存块 */
#define MEM_SIZE 8

unsigned char mem[MEM_SIZE];

/* =========================================================
 * 碎片模拟
 * ========================================================= */
void task_fragment_demo(void)
{
    unsigned char i, j;

    while(1)
    {
        /* 随机释放一半 */
        for(i = 0; i < MEM_SIZE; i++)
        {
            if(i % 2)
                mem[i] = 0;
            else
                mem[i] = 1;
        }

        /* 再随机占用 */
        for(j = 0; j < MEM_SIZE; j++)
        {
            if(mem[j] == 0)
                mem[j] = 1;
        }

        
        LED_FRAG = ~LED_FRAG;

        os_delay(1);
    }
}

/* =========================================================
 * 入口
 * ========================================================= */
void hrtos_main(void)
{
    unsigned char i;

    for(i = 0; i < MEM_SIZE; i++)
        mem[i] = 0;

    LED_FRAG = 0;

    os_task_create((unsigned int)task_fragment_demo, 1, 4, 5);
}