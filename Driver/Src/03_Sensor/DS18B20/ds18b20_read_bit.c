#include "hrtos_hal.h"


/*------------------------------------------------
 * 读取一个数据位
 *
 * 返回：
 * 0 - 数据位为0
 * 1 - 数据位为1
 *
 * 注意：
 * DS18B20单总线严格时序。
 *------------------------------------------------*/
unsigned char drv_ds18b20_read_bit(void)
{
    unsigned int i;
    unsigned char _data;

    DS18B20_DATA = 0;

    i++;

    DS18B20_DATA = 1;

    i++;
    i++;

    _data = DS18B20_DATA;

    i = 8;

    while(i > 0)
    {
        i--;
    }

    return _data;
}