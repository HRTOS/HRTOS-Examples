#include "hrtos.h"

#define MAILBOX_ID 0

sbit LED_SEND = P1^0;
sbit LED_RECV = P1^1;

/* 发送任务 */
void sender(void)
{
    u8 msg = 0x55;

    while(1)
    {
        LED_SEND = ~LED_SEND;   // 发送节奏指示

        os_mailbox_send(MAILBOX_ID, msg);

        msg++;                  // 数据变化（非常关键，体现通信流动）

        os_delay(5);
    }
}

/* 接收任务 */
void receiver(void)
{
    int msg;

    while(1)
    {
        /* 阻塞接收：这里是重点 */
        msg = os_mailbox_receive(MAILBOX_ID);

        if(msg >= 0)
        {
            /* 收到数据：LED直接显示数据 */
            P1 = (P1 & 0xFC) | (msg & 0x03); 
            // 只用P1.0/P1.1做简单映射，避免破坏其他位
        }

        LED_RECV = ~LED_RECV;   // 每次接收触发闪烁反馈

        os_delay(1);
    }
}

/* 系统入口 */
void hrtos_main(void)
{
    LED_SEND = 0;
    LED_RECV = 0;

    os_mailbox_init(MAILBOX_ID);

    os_task_create((unsigned int)sender,   1, 3, 5);
    os_task_create((unsigned int)receiver, 2, 2, 5);
}