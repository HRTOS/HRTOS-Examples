#include "hrtos_hal.h"

/*------------------------------------------------
 * OLEDÏÔÊ¾ºº×Ö
 *------------------------------------------------*/
void drv_oled_show_chinese(unsigned char x,
                           unsigned char y,
                           unsigned char no)
{
    unsigned char i;

    drv_oled_set_pos(x, y);

    for(i = 0; i < 16; i++)
    {
        drv_oled_write_data(Hzk[2 * no][i]);
    }

    drv_oled_set_pos(x, y + 1);

    for(i = 0; i < 16; i++)
    {
        drv_oled_write_data(Hzk[2 * no + 1][i]);
    }
}