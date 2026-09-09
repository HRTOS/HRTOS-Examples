#include "hrtos_hal.h"


/*
 * 函数名：drv_relay_get
 *
 * 功能：
 *      获取当前继电器状态。
 */
u8 drv_relay_get(void)
{
    return relay_state;
}