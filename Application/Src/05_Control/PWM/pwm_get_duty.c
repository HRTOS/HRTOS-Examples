#include "hrtos_hal.h"


/*
 * 函数名：drv_pwm_get_duty
 *
 * 功能：
 *      获取当前PWM占空比。
 */
u8 drv_pwm_get_duty(void)
{
    return pwm_duty;
}
