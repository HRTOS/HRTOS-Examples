#include "hrtos_hal.h"


/**
 * @brief  ·­×ªÖ¸¶¨LED
 * @param  led 0-7
 */
void drv_led_toggle(unsigned char led)
{
    
    LED_PORT ^= (1 << led);
}