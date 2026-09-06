#include "hrtos_hal.h"



/*
 * 函数名：drv_relay_off
 *
 * 功能：
 *      释放继电器。
 */
void drv_relay_off(void)
{
    RELAY_IN = 0;
    RELAY_EN = 1;

    relay_state = 0;
}