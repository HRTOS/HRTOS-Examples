#include "hrtos.h"

/*============================================================
 * Example: Dual LED Task Demo
 *------------------------------------------------------------
 * Fast Task : 2 ticks toggle LED0 (high priority)
 * Slow Task : 25 ticks toggle LED1 (low priority)
 *===========================================================*/


/*============================================================
 * Task Configuration
 *===========================================================*/

/* Fast LED Task */
#define FAST_TASK_ID     1
#define FAST_PRIORITY    7
#define FAST_STACK       5
#define FAST_DELAY       2

/* Slow LED Task */
#define SLOW_TASK_ID     2
#define SLOW_PRIORITY    3
#define SLOW_STACK       5
#define SLOW_DELAY       25


/*============================================================
 * Hardware Definition
 *===========================================================*/
sbit LED_FAST = P1^0;
sbit LED_SLOW = P1^1;


/*============================================================
 * Task: Fast LED
 *===========================================================*/
static void fast_led_task(void)
{
    while (1)
    {
        LED_FAST = !LED_FAST;
        os_delay(FAST_DELAY);
    }
}


/*============================================================
 * Task: Slow LED
 *===========================================================*/
static void slow_led_task(void)
{
    while (1)
    {
        LED_SLOW = !LED_SLOW;
        os_delay(SLOW_DELAY);
    }
}


/*============================================================
 * Application Entry
 *===========================================================*/
void hrtos_main(void)
{
    /* Create Fast Task */
    os_task_create(
        (unsigned int)fast_led_task,
        FAST_TASK_ID,
        FAST_PRIORITY,
        FAST_STACK
    );

    /* Create Slow Task */
    os_task_create(
        (unsigned int)slow_led_task,
        SLOW_TASK_ID,
        SLOW_PRIORITY,
        SLOW_STACK
    );
}