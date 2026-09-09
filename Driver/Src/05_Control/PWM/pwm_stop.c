#include "hrtos_hal.h"


/*
 * 函数名：drv_pwm_stop
 *
 * 功能：
 *      停止PCA PWM。
 */
void drv_pwm_stop(void)
{
    CR = 0;
}
