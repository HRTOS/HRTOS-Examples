#include "hrtos_hal.h"


/**
 * @brief  µ„¡¡÷∏∂®LED
 * @param  led 0-7
 */
void drv_led_on(unsigned char led)
{
    
    LED_PORT &= ~(1 << led);
}