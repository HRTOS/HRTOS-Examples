#include "hrtos_hal.h"

/*
 * 函数名：drv_pwm_set_duty
 *
 * 功能：
 *      设置PWM占空比。
 *
 * 参数：
 *      duty：0~100
 *
 * 说明：
 *      PCA硬件PWM寄存器的方向与
 *      常规“数值越大占空比越大”的
 *      直觉相反。
 *
 *      因此这里将：
 *
 *          0   -> 0%
 *          100 -> 100%
 *
 *      转换成PCA对应的寄存器值。
 */
void drv_pwm_set_duty(u8 duty)
{
    unsigned int pwm_value;

    /*
     * 限制参数范围
     */
    if(duty > 100)
    {
        duty = 100;
    }

    pwm_duty = duty;

    /*
     * 将0~100%的占空比
     * 转换为0~255。
     */
    pwm_value = (unsigned int)duty * 255;
    pwm_value /= 100;

    /*
     * PCA PWM寄存器：
     *
     * 0x00 -> 接近100%
     * 0xFF -> 接近0%
     *
     * 因此需要反向转换。
     */
    pwm_value = 255 - pwm_value;

    CCAP0H = (u8)pwm_value;
    CCAP0L = (u8)pwm_value;
}
