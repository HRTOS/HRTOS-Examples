#include "hrtos.h"

/*==================== 配置 ====================*/
#define TASK_RECV     1
#define TASK_SEND     2

#define PRIO_RECV     5
#define PRIO_SEND     4

#define STACK_RECV    5
#define STACK_SEND    5

#define MSGQ_SIZE     8

#define TICK_RECV     20
#define TICK_SEND     50

/*==================== LED 定义 ====================*/
sbit LED_RECV = P1^0;   /* 接收任务指示灯 */
sbit LED_SEND = P1^1;   /* 发送任务指示灯 */

/*==================== 全局 ====================*/
static os_msgq_t msgq;
static u8 msgq_buffer[MSGQ_SIZE];

static u8 send_count = 0;
static u8 recv_count = 0;

/*==================== 接收任务 ====================*/
static void recv_task(void)
{
    u8 _data;

    LED_RECV = 1;

    while (1)
    {
        /*
         * 从消息队列接收数据
         * 非阻塞模式
         */
        if (os_msgq_recv(&msgq, 0, &_data, 1) == 1)
        {
            recv_count++;

            /* 接收成功 → 翻转LED */
            LED_RECV = ~LED_RECV;
        }

        os_delay(TICK_RECV);
    }
}

/*==================== 发送任务 ====================*/
static void send_task(void)
{
    LED_SEND = 1;

    while (1)
    {
        send_count++;

        /*
         * 向消息队列发送1字节数据
         */
        if (os_msgq_send(&msgq, 0, send_count, 1) == 1)
        {
            /* 发送成功 → 翻转LED */
            LED_SEND = ~LED_SEND;
        }

        os_delay(TICK_SEND);
    }
}

/*==================== 系统入口 ====================*/
void hrtos_main(void)
{
    /*
     * 初始化消息队列
     * 将消息队列对象与缓冲区绑定
     */
    os_msgq_init(&msgq, msgq_buffer, MSGQ_SIZE);

    if (os_task_create(recv_task, TASK_RECV, PRIO_RECV, STACK_RECV) != 1)
    {
        while (1);
    }

    if (os_task_create(send_task, TASK_SEND, PRIO_SEND, STACK_SEND) != 1)
    {
        while (1);
    }
}