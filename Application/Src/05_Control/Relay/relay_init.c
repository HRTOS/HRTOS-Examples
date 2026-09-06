#include "hrtos_hal.h"

/*
 * 继电器当前状态
 *
 * 0：关闭
 * 1：打开
 */
u8 relay_state;


/*
 * 函数名：drv_relay_init
 *
 * 功能：
 *      初始化继电器。
 *
 * 说明：
 *      初始化后默认关闭继电器，
 *      防止单片机启动时继电器误吸合。
 */
void drv_relay_init(void)
{
    relay_state = 0;

    /*
     * 关闭继电器
     */
    RELAY_IN = 0;

    /*
     * 使能脚保持有效
     *
     * 根据原实验程序：
     * RELAY_EN = 1
     * 为正常工作状态。
     */
    RELAY_EN = 1;
}