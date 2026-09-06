#include "hrtos_hal.h"


/*
 * 函数名：drv_relay_on
 *
 * 功能：
 *      吸合继电器。
 */
void drv_relay_on(void)
{
    RELAY_EN = 1;
    RELAY_IN = 1;

    relay_state = 1;
}