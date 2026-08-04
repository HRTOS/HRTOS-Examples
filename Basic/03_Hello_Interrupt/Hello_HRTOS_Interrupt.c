#include "hrtos.h"

/*============================================================
 * Hello HRTOS INT0 (Debounce + Event Model)
 *
 * 架构说明：
 *   INT0 → HRTOS内核接管 → ISR触发 → 防抖 → event → task
 *
 * Model:
 *   INT0 ISR (kernel) → debounce → os_event → wait_task
 *===========================================================*/


/*==================== 配置 ====================*/
#define TASK_WAIT     1
#define TASK_INT0     2

#define PRIO_WAIT     6
#define PRIO_INT0     9

#define STACK_WAIT    5
#define STACK_INT0    5

#define IRQ_EVENT     1

#define WAIT_TICK     0 //0表示不使用时间触发

/* 防抖时间（简单软件延时） */
#define DEBOUNCE_TICK  5


/*==================== LED ====================*/
sbit LED = P1^0;


/*==================== 全局 ====================*/
static u8 irq_count = 0;
static u8 debounce_flag = 0;


/*============================================================
 * 等待任务（业务处理层）
 *
 * 功能：
 *   等待 INT0 事件触发
 *   每次触发 LED 翻转 + 计数
 *===========================================================*/
static void wait_task(void)
{
	
    while (1)
    {
        os_event_wait(IRQ_EVENT, WAIT_TICK);        
        irq_count++;
        LED = ~LED;        		
		os_event_delete(IRQ_EVENT);
    }
}


/*============================================================
 * INT0 任务（内核调度层）
 *
 * 功能：
 *   由 HRTOS 在 INT0 触发时调用
 *   做“轻量防抖 + 事件投递”
 *
 * 注意：
 *   必须调用 os_interrupt_exit() 退出中断上下文
 *===========================================================*/
static void int0_task(void)
{
	
	/* 触发事件 */
	os_event_set_from_isr(IRQ_EVENT);
	
    /* ===== 必须退出中断上下文 ===== */
    os_interrupt_exit();
}


/*============================================================
 * 系统入口
 *
 * 功能：
 *   1. 初始化事件
 *   2. 注册 INT0 内核任务
 *   3. 创建等待任务
 *===========================================================*/
void hrtos_main(void)
{
    os_event_init(IRQ_EVENT);

    /* LED 初始化 */
    LED = 1;
	
	IT0 = 1;   // 1 = 下降沿触发（推荐）
	EX0 = 1;   // 允许 INT0 中断

    /* 注册 INT0 处理任务（由内核触发） */
    if (os_task_create(int0_task, TASK_INT0, PRIO_INT0, STACK_INT0) != 1)
    {
        while (1) os_delay(20);
    }

    /* 创建等待任务 */
    if (os_task_create(wait_task, TASK_WAIT, PRIO_WAIT, STACK_WAIT) != 1)
    {
        while (1) os_delay(20);
    }
}