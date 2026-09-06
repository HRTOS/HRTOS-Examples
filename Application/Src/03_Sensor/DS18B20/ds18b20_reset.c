#include "hrtos_hal.h"

/*------------------------------------------------
 * DS18B20复位
 *
 * 功能：
 * 主机发送复位脉冲，并等待DS18B20响应。
 *
 * 注意：
 * 本函数属于严格时序操作，
 * 调用时需要关闭总中断。
 *------------------------------------------------*/
void drv_ds18b20_reset(void)
{
    unsigned int i;

    DS18B20_DATA = 0;

    i = 70;

    while(i > 0)
    {
        i--;
    }

    DS18B20_DATA = 1;

    i = 5;

    while(DS18B20_DATA)
    {
        i--;
		os_delay_ms(1);
		if(!i)break;
    }
}