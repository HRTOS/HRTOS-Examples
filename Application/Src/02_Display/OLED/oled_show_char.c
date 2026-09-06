#include "hrtos_hal.h"


/*------------------------------------------------
 * OLEDÏÔÊ¾×Ö·û
 *
 * size = 16£º8x16
 * size = 8 £º6x8
 *------------------------------------------------*/
void drv_oled_show_char(unsigned char x,
                        unsigned char y,
                        unsigned char chr,
                        unsigned char size)
{
    unsigned char c;
    unsigned char i;

    c = chr - ' ';

    if(x > DRV_OLED_WIDTH - 1)
    {
        x = 0;
        y += 2;
    }

    if(size == 16)
    {
        drv_oled_set_pos(x, y);

        for(i = 0; i < 8; i++)
        {
            drv_oled_write_data(F8X16[c * 16 + i]);
        }

        drv_oled_set_pos(x, y + 1);

        for(i = 0; i < 8; i++)
        {
            drv_oled_write_data(F8X16[c * 16 + i + 8]);
        }
    }
    else
    {
        drv_oled_set_pos(x, y);

        for(i = 0; i < 6; i++)
        {
            drv_oled_write_data(F6x8[c][i]);
        }
    }
}