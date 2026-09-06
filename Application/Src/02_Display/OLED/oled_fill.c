#include "hrtos_hal.h"


/*------------------------------------------------
 * OLEDÈ«ÆÁÌî³ä
 *
 * _data = 0x00£ºÈ«ºÚ
 * _data = 0xFF£ºÈ«ÁÁ
 *------------------------------------------------*/
void drv_oled_fill(unsigned char _data)
{
    unsigned char page;
    unsigned char column;

    for(page = 0; page < 8; page++)
    {
        drv_oled_write_command(0xB0 + page);
        drv_oled_write_command(0x02);
        drv_oled_write_command(0x10);

        for(column = 0; column < 128; column++)
        {
            drv_oled_write_data(_data);
        }
    }
}