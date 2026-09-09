#include "hrtos_hal.h"



/*
 * 函数名：drv_pwm_start
 *
 * 功能：
 *      启动PCA PWM。
 */
void drv_pwm_start(void)
{
    CR = 1;
}