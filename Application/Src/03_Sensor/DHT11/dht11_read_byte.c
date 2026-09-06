#include "hrtos_hal.h"



/*------------------------------------------------
 * DHT11读取一个字节
 *
 * 注意：
 * 本函数必须在关闭总中断的情况下调用，
 * 保证数据位采样时序不被中断打断。
 *------------------------------------------------*/
void drv_dht11_read_byte(unsigned char *_data)
{
    unsigned char i;
    unsigned char flag;
    unsigned char temp;
    unsigned char value;

    value = 0;

    for(i = 0; i < 8; i++)
    {
        flag = 2;
		
		
		
        /*
         * 等待数据线进入低电平
         */
        while((!DHT11_DATA) && flag++);

        /*
         * 延时后判断数据线状态
         * 高电平持续时间决定数据位为0还是1
         */
        drv_dht11_delay_10us();
        drv_dht11_delay_10us();

        temp = 0;

        if(DHT11_DATA)
        {
            temp = 1;
        }

        flag = 2;

        /*
         * 等待当前数据位结束
         */
        while((DHT11_DATA) && flag++);
		
		if(flag==1)break;

        /*
         * 保存数据位
         */
        value <<= 1;
        value |= temp;
    }

    *_data = value;
}
