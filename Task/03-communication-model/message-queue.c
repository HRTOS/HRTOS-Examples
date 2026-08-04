#include "hrtos.h"

/* 消息队列对象 + 缓冲区 */
static os_msgq_t g_msgq;
static u8 g_msgq_buf[16];

sbit LED_P = P1^0;   // producer
sbit LED_C = P1^1;   // consumer
sbit LED_Q = P1^2;   // queue activity（关键亮点）

/* 生产者 */
void producer(void)
{
    u8 data1 = 0x11;

    while(1)
    {
        LED_P = ~LED_P;   // 生产节奏

        os_msgq_send(&g_msgq, 0, data1, 0);

        data1++;

        /* 队列“活动提示” */
        LED_Q = 1;

        os_delay(3);
    }
}

/* 消费者 */
void consumer(void)
{
    u8 data1;

    while(1)
    {
        /* 阻塞接收 */
        if(os_msgq_recv(&g_msgq, 0, &data1, 0))
        {
            LED_C = ~LED_C;   // 消费节奏

            /* 用数据低2位显示队列内容变化 */
            P1 = (P1 & 0xF0) | (data1 & 0x0F);

            LED_Q = 0;        // 消费后清理“队列活动标志”
        }

        os_delay(2);
    }
}

/* 系统入口 */
void hrtos_main(void)
{
    LED_P = 0;
    LED_C = 0;
    LED_Q = 0;

    os_msgq_init(&g_msgq, g_msgq_buf, sizeof(g_msgq_buf));

    os_task_create((unsigned int)producer, 1, 3, 5);
    os_task_create((unsigned int)consumer, 2, 2, 5);
}