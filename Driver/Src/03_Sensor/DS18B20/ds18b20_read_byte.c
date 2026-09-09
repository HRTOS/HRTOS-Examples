#include "hrtos_hal.h"


/*------------------------------------------------
 * 读取一个字节
 *
 * DS18B20采用低位优先方式发送数据。
 *------------------------------------------------*/
unsigned char drv_ds18b20_read_byte(void)
{
    unsigned char i;
    unsigned char _data;
    unsigned char bit_data;

    _data = 0;

    for(i = 0; i < 8; i++)
    {
        bit_data = drv_ds18b20_read_bit();

        _data = (bit_data << 7) | (_data >> 1);
    }

    return _data;
}