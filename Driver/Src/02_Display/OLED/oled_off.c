#include "hrtos_hal.h"


/*------------------------------------------------
 * OLEDœ‘ æπÿ±’
 *------------------------------------------------*/
void drv_oled_off(void)
{
    drv_oled_write_command(0x8D);
    drv_oled_write_command(0x10);
    drv_oled_write_command(0xAE);
}