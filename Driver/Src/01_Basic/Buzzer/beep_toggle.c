#include "hrtos_hal.h"



/**
 * @brief  ·äÃùÆ÷·­×ª
 */
void drv_beep_toggle(void)
{
    beep = ~beep;
}