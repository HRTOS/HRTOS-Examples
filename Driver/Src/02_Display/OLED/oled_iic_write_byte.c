#include "hrtos_hal.h"


/*------------------------------------------------
 * 软件IIC写一个字节
 *------------------------------------------------*/
void drv_oled_iic_write_byte(unsigned char _data)
{
    unsigned char i;

    OLED_SCLK_CLR();

    for(i = 0; i < 8; i++)
    {
        if(_data & 0x80)
        {
            OLED_SDIN_SET();
        }
        else
        {
            OLED_SDIN_CLR();
        }

        _data <<= 1;

        OLED_SCLK_SET();
        OLED_SCLK_CLR();
    }
}