#include "hrtos_hal.h"


/**
 * @brief  πÿ±’÷∏∂®LED
 * @param  led 0-7
 */
void drv_led_off(unsigned char led)
{
    
    LED_PORT |= (1 << led);
}
