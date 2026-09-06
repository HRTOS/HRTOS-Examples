#include "hrtos.h"

/*==================== 配置 ====================*/
#define TASK_A        1
#define TASK_B        2

#define TASK_T1       0

#define PRIO_TASK     4
#define PRIO_T1       9
#define PRIO_T1_NEST  10

#define STACK_TASK_A  5
#define STACK_TASK_B  5
#define STACK_T1      0

#define TICK_TASK     20

/*==================== LED 定义 ====================*/
sbit LED_A = P1^0;   /* 任务A指示灯 */
sbit LED_B = P1^1;   /* 任务B指示灯 */
sbit LED_T1 = P1^2;  /* T1中断指示灯 */

/*==================== 全局 ====================*/
static u16 count_a = 0;
static u16 count_b = 0;
static u16 xdata count_t1 = 0;

/*==================== 普通任务A ====================*/
static void task_a(void)
{
    LED_A = 1;

    while (1)
    {
        count_a++;

        LED_A = ~LED_A;

        os_delay(TICK_TASK);
    }
}

/*==================== 普通任务B ====================*/
static void task_b(void)
{
    LED_B = 1;

    while (1)
    {
        count_b++;

        LED_B = ~LED_B;

        os_delay(TICK_TASK);
    }
}

void Timer1_Init(void)
{
    TMOD &= 0x0F;
    TMOD |= 0x10;     // T1模式1

    TH1 = 0xFC;
    TL1 = 0x18;       // 12MHz 1ms

    TF1 = 0;
    ET1 = 1;
    PT1 = 1;          // T1高优先级
    TR1 = 1;
}

/*==================== T1中断处理 ====================*/
static void t1_isr(void)
{
    count_t1++;
	
	TH1 = 0xFC;
    TL1 = 0x18;       // 12MHz 1ms

    /*
     * T1进入中断
     */
    LED_T1 = ~LED_T1;

    /*
     * 中断处理完成
     */
    os_interrupt_exit();
}

/*==================== 系统入口 ====================*/
void hrtos_main(void)
{

    Timer1_Init();
	
	/*
     * 两个同优先级普通任务
     */
    if (os_task_create(task_a, TASK_A, PRIO_TASK, STACK_TASK_A) != 1)
    {
        while (1);
    }

    if (os_task_create(task_b, TASK_B, PRIO_TASK, STACK_TASK_B) != 1)
    {
        while (1);
    }

    /*
     * T1普通中断任务
     * 优先级9
     */
    os_task_create((unsigned int)t1_isr, TASK_T1, PRIO_T1, STACK_T1);

    /*
     * T1嵌套中断任务
     * 优先级10
     */
    os_task_create((unsigned int)t1_isr, TASK_T1, PRIO_T1_NEST, STACK_T1);

}