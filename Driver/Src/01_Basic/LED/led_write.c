#include "hrtos_hal.h"


/**
 * @brief  Ö±½ÓÊä³öLED×´Ì¬
 * @param  value
 */
void drv_led_write(unsigned char value)
{
    
    LED_PORT = value;
}