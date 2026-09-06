#include "hrtos_hal.h"


/*------------------------------------------------
 * OLED显示整数
 *
 * num：0~4294967295
 * len：显示位数
 * size：字体大小
 *------------------------------------------------*/
void drv_oled_show_num(unsigned char x,
                       unsigned char y,
                       unsigned long num,
                       unsigned char len,
                       unsigned char size)
{
    unsigned char t;
    unsigned char temp;
    unsigned char enshow;

    enshow = 0;

    for(t = 0; t < len; t++)
    {
        temp = (num / drv_oled_pow(10, len - t - 1)) % 10;

        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                drv_oled_show_char(
                    x + (size / 2) * t,
                    y,
                    ' ',
                    size
                );

                continue;
            }
            else
            {
                enshow = 1;
            }
        }

        drv_oled_show_char(
            x + (size / 2) * t,
            y,
            temp + '0',
            size
        );
    }
}
