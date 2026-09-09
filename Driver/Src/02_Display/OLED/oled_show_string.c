#include "hrtos_hal.h"


/*------------------------------------------------
 * OLEDÏÔÊ¾×Ö·û´®
 *------------------------------------------------*/
void drv_oled_show_string(unsigned char x,
                          unsigned char y,
                          unsigned char *str,
                          unsigned char size)
{
    unsigned char i;

    i = 0;

    while(str[i] != '\0')
    {
        drv_oled_show_char(x, y, str[i], size);

        x += 8;

        if(x > 120)
        {
            x = 0;
            y += 2;
        }

        i++;
    }
}