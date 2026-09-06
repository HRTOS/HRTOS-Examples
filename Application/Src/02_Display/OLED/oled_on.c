#include "hrtos_hal.h"


/*------------------------------------------------
 * OLEDÏÔÊ¾¿ªÆô
 *------------------------------------------------*/
void drv_oled_on(void)
{
    drv_oled_write_command(0x8D);
    drv_oled_write_command(0x14);
    drv_oled_write_command(0xAF);
}