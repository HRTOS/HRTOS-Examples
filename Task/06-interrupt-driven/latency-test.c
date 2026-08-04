#include "hrtos.h"

/* =========================================================
 * LED
 * ========================================================= */
sbit LED_ISR  = P1^0;
sbit LED_MEAS = P1^1;
sbit LED_LOAD = P1^2;

/* =========================================================
 * 延迟统计
 * ========================================================= */
static u16 isr_trigger_count = 0;
static u16 task_response_count = 0;

/* =========================================================
 * ISR任务
 * ========================================================= */
static void int0_task(void)
{
    isr_trigger_count++;

    LED_ISR = ~LED_ISR;

    /* ISR → event */
    os_event_set_from_isr(1);
	
	IE0 = 0;

    os_interrupt_exit();
}

/* =========================================================
 * 响应任务
 * ========================================================= */
static void response_task(void)
{
    while(1)
    {
        os_event_wait(1, 0);

        task_response_count++;

        LED_MEAS = ~LED_MEAS;
		
		os_event_delete(1);
    }
}

/* =========================================================
 * 负载任务（模拟系统压力）
 * ========================================================= */
static void load_task(void)
{
    volatile u16 i;

    while(1)
    {
        LED_LOAD = ~LED_LOAD;
		
		for(i = 0; i < 20000; i++);

    }
}

/* =========================================================
 * 入口
 * ========================================================= */
void hrtos_main(void)
{

    os_event_init(1);

    /* INT0 */
    IT0 = 1;
    EX0 = 1;
	IE0 = 0;

    os_task_create(int0_task, 1, 9, 5);
    os_task_create(response_task, 2, 5, 5);
    os_task_create(load_task, 3, 3, 5);
}