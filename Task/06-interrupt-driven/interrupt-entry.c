#include "hrtos.h"

/* =========================================================
 * LED
 * ========================================================= */
sbit LED_INT  = P1^0;   /* 中断触发 */
sbit LED_TASK = P1^1;   /* 任务响应 */

/* =========================================================
 * 配置
 * ========================================================= */
#define TASK_WAIT   1
#define TASK_INT0   2

#define PRIO_WAIT   6
#define PRIO_INT0   9

#define STACK_WAIT  5
#define STACK_INT0  5

#define IRQ_EVENT   1

#define WAIT_TICK   0

/* =========================================================
 * 全局状态
 * ========================================================= */
static u16 irq_count = 0;

/* =========================================================
 * 等待任务（业务层）
 * ========================================================= */
static void wait_task(void)
{
    while(1)
    {
        os_event_wait(IRQ_EVENT, WAIT_TICK);

        irq_count++;
        LED_TASK = ~LED_TASK;

        os_event_delete(IRQ_EVENT);
    }
}

/* =========================================================
 * INT0任务（中断映射任务）
 * ========================================================= */
static void int0_task(void)
{
    /* ISR安全事件投递 */
    os_event_set_from_isr(IRQ_EVENT);

    LED_INT = ~LED_INT;

    /* 退出中断上下文 */
    os_interrupt_exit();
}

/* =========================================================
 * 系统入口
 * ========================================================= */
void hrtos_main(void)
{
    LED_INT  = 0;
    LED_TASK = 0;

    os_event_init(IRQ_EVENT);

    /* INT0配置 */
    IT0 = 1;
    EX0 = 1;

    /* 注册 INT0任务（内核调度层） */
    if (os_task_create(int0_task, TASK_INT0, PRIO_INT0, STACK_INT0) != 1)
    {
        while(1);
    }

    /* 注册等待任务 */
    if (os_task_create(wait_task, TASK_WAIT, PRIO_WAIT, STACK_WAIT) != 1)
    {
        while(1);
    }
}