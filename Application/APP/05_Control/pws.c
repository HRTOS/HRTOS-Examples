
#include "hrtos.h"
#include "hrtos_hal.h"

/*
 * =========================================================
 * PWM + 4位数码管显示示例
 * ---------------------------------------------------------
 * 功能：
 * 1. 初始化PWM
 * 2. 设置PWM占空比
 * 3. PWM占空比从0%逐渐增加到100%
 * 4. 再从100%逐渐降低到0%
 * 5. 使用4位数码管显示当前占空比
 *
 * 数码管显示：
 *     0000 -> 0%
 *     0050 -> 50%
 *     0100 -> 100%
 *
 * PWM输出：
 *     占空比随时间逐渐变化
 * =========================================================
 */


/*------------------------------------------------
 * 当前PWM占空比
 *------------------------------------------------*/
unsigned char pwm_display_duty = 0;


/*------------------------------------------------
 * PWM控制任务
 *------------------------------------------------*/
void task_pwm_control(void)
{
    unsigned char duty;

    while(1)
    {
        /*
         * 占空比从0%增加到100%
         */
        for(duty = 0; duty <= 100; duty++)
        {
            drv_pwm_set_duty(duty);

            pwm_display_duty = duty;

            os_delay(1);
        }

        /*
         * 占空比从100%降低到0%
         */
        for(duty = 100; duty > 0; duty--)
        {
            drv_pwm_set_duty(duty);

            pwm_display_duty = duty;

            os_delay(1);
        }

        /*
         * 最后设置为0%
         */
        drv_pwm_set_duty(0);

        pwm_display_duty = 0;
    }
}


/*------------------------------------------------
 * 数码管显示任务
 *
 * 显示当前PWM占空比
 *------------------------------------------------*/
void task_seg_display(void)
{
    unsigned int value;

    while(1)
    {
        /*
         * 获取当前占空比
         */
        value = drv_pwm_get_duty();

        /*
         * 显示0~100
         */
        drv_seg_display(0, value / 1000);
        drv_seg_display(1, value / 100 % 10);
        drv_seg_display(2, value / 10 % 10);
        drv_seg_display(3, value % 10);
    }
}


/**
 * @brief PWM示例初始化
 */
void hrtos_main(void)
{
    /* 初始化PWM */
    drv_pwm_init();

    /* 初始化数码管 */
    drv_seg_init();

    /* 设置初始占空比为0% */
    drv_pwm_set_duty(0);

    /* 创建PWM控制任务 */
    os_task_create(
        task_pwm_control,
        2,
        2,
        4
    );

    /* 创建数码管显示任务 */
    os_task_create(
        task_seg_display,
        3,
        2,
        4
    );
}

