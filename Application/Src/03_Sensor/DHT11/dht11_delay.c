#include "hrtos_hal.h"

/*------------------------------------------------
 * DHT11长延时
 *
 * 用于DHT11启动信号等非严格时序部分
 *------------------------------------------------*/
void drv_dht11_delay(unsigned int time)
{
    unsigned char i;

    while(time--)
    {
        for(i = 0; i < 27; i++);
    }
}