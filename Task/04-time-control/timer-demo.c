#include "hrtos.h"

/* =========================================================
 * LED定义（时间系统可视化）
 * ========================================================= */
sbit LED_DELAY = P1^0;   /* delay任务 */
sbit LED_WAIT  = P1^1;   /* wait状态 */
sbit LED_EVENT = P1^2;   /* event触发 */
sbit LED_TICK  = P1^3;   /* tick心跳（辅助观察） */

/* =========================================================
 * EVENT ID
 * ========================================================= */
#define EVENT_ID 1

/* =========================================================
 * 1. 时间驱动任务（Delay vs Schedule Delay）
 * ========================================================= */
void task_delay(void)
{
    u32 last = 0;

    while(1)
    {
        u32 now = os_uptime_ms();

        /* 500ms节奏闪烁 */
        if (now - last >= 500)
        {
            last = now;
            LED_DELAY = ~LED_DELAY;
        }

        /* 释放CPU（关键对比点） */
        os_delay(os_ms_to_tick(10));
    }
}

/* =========================================================
 * 2. 事件等待任务（Wait）
 * ========================================================= */
void task_wait(void)
{
    u16 timeout;

    while(1)
    {
        LED_WAIT = 0;

        timeout = os_ms_to_tick(100);

        if (os_event_wait(EVENT_ID, timeout))
        {
            /* 被事件唤醒 */
            LED_WAIT = 1;

            /* 可视化脉冲 */
            os_delay(os_ms_to_tick(50));
        }
    }
}

/* =========================================================
 * 3. 事件触发任务（Producer）
 * ========================================================= */
void task_event_trigger(void)
{
    u32 last = 0;

    while(1)
    {
        u32 now = os_uptime_ms();

        /* 每1000ms触发一次事件 */
        if (now - last >= 1000)
        {
            last = now;

            os_event_write(EVENT_ID);

            LED_EVENT = ~LED_EVENT;
        }

        os_delay(os_ms_to_tick(10));
    }
}

/* =========================================================
 * 4. tick可视化辅助任务（增强教学效果）
 * ========================================================= */
void task_tick_view(void)
{
    while(1)
    {
        /* 低频闪烁 = 系统活跃指示 */
        LED_TICK = ~LED_TICK;

        os_delay(os_ms_to_tick(100));
    }
}


/* =========================================================
 * 6. 系统入口（必须）
 * ========================================================= */
void hrtos_main(void)
{
    /* LED初始化 */
    LED_DELAY = 0;
    LED_WAIT  = 0;
    LED_EVENT = 0;
    LED_TICK  = 0;

    /* =========================
     * 初始化事件系统
     * ========================= */
    os_event_init(EVENT_ID);

    /* =========================
     * tick系统配置
     * ========================= */
    os_tick_config(1, 1);  /* 1ms tick */

    /* =========================
     * 普通任务注册
     * ========================= */
    os_task_create((unsigned int)task_delay,        1, 2, 5);
    os_task_create((unsigned int)task_wait,         2, 3, 5);
    os_task_create((unsigned int)task_event_trigger,3, 2, 5);
    os_task_create((unsigned int)task_tick_view,    4, 4, 5);
}