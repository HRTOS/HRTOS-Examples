#include "hrtos_hal.h"



/*
 * 函数名：drv_relay_set
 *
 * 功能：
 *      根据指定状态控制继电器。
 *
 * 参数：
 *      state
 *          0：关闭
 *          1：打开
 */
void drv_relay_set(u8 state)
{
    if(state)
    {
        drv_relay_on();
    }
    else
    {
        drv_relay_off();
    }
}