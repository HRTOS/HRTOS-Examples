#include "hrtos_hal.h"


/*------------------------------------------------
 * 写入一个字节
 *
 * DS18B20采用低位优先方式发送数据。
 *------------------------------------------------*/
void drv_ds18b20_write_byte(unsigned char _data)
{
    unsigned char i;
    unsigned char bit_data;
    unsigned int delay;

    for(i = 0; i < 8; i++)
    {
        bit_data = _data & 0x01;

        _data >>= 1;

        if(bit_data)
        {
            DS18B20_DATA = 0;

            delay++;
            delay++;

            DS18B20_DATA = 1;

            delay = 8;

            while(delay > 0)
            {
                delay--;
            }
        }
        else
        {
            DS18B20_DATA = 0;

            delay = 8;

            while(delay > 0)
            {
                delay--;
            }

            DS18B20_DATA = 1;

            delay++;
            delay++;
        }
    }
}