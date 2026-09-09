#include "hrtos_hal.h"


/*------------------------------------------------
 * OLED显示BMP
 *
 * x0/y0：起始位置
 * x1/y1：结束位置
 *------------------------------------------------*/
void drv_oled_draw_bmp(unsigned char x0,
                       unsigned char y0,
                       unsigned char x1,
                       unsigned char y1,
                       unsigned char *bmp)
{
    unsigned int j;
    unsigned char x;
    unsigned char y;

    j = 0;

    for(y = y0; y < y1; y++)
    {
        drv_oled_set_pos(x0, y);

        for(x = x0; x < x1; x++)
        {
            drv_oled_write_data(bmp[j++]);
        }
    }
}
