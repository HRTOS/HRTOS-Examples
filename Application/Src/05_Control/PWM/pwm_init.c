#include "hrtos_hal.h"


/*
 * PWM当前占空比
 *
 * 使用0~100表示，方便应用层使用。
 */
u8 pwm_duty;


/*
 * 函数名：drv_pwm_init
 *
 * 功能：
 *      初始化PCA PWM模块。
 *
 * 说明：
 *      PCA工作在8位PWM模式。
 *      不使用PCA中断。
 */
void drv_pwm_init(void)
{
    pwm_duty = 0;

    /*
     * PCA0/PWM0输出：
     *
     * STC12C5A60S2默认：
     * PCA0/PWM0 -> P1.3
     *
     * PDIP-40：
     * P1.3 = 第4脚
     */

    P1M1 &= ~0x08;
    P1M0 |=  0x08;       /* P1.3 推挽输出 */

    /* PCA使用P1口，不切换到P4 */
    AUXR1 &= ~0x40;

    /* 停止PCA */
    CCON = 0x00;

    /* PCA计数器清零 */
    CL = 0x00;
    CH = 0x00;

    /*
     * PCA时钟：
     * FOSC / 2
     */
    CMOD = 0x02;

    /*
     * PCA模块0：
     * PWM模式
     */
    CCAPM0 = 0x42;

    /*
     * 初始0%
     */
    CCAP0H = 0xFF;
    CCAP0L = 0xFF;

    /* 启动PCA */
    CR = 1;
}