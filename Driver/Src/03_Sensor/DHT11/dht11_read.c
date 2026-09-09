#include "hrtos_hal.h"



/*------------------------------------------------
 * DHT11读取温湿度
 *
 * 返回：
 * 0 - 读取成功
 * 1 - 读取失败
 *
 * humidity：
 * 湿度整数部分
 *
 * temperature：
 * 温度整数部分
 *------------------------------------------------*/
unsigned char drv_dht11_read(unsigned char *humidity,
                              unsigned char *temperature)
{
    unsigned char flag;
    unsigned char temp;
    unsigned char rh_high;
    unsigned char rh_low;
    unsigned char t_high;
    unsigned char t_low;
    unsigned char checksum;
    unsigned char ea_state;


    /*------------------------------------------------
     * 主机发送启动信号
     *
     * 这一阶段不属于严格的数据采样时序，
     * 因此不关闭总中断。
     *------------------------------------------------*/
    DHT11_DATA = 0;

    drv_dht11_delay(300);

    DHT11_DATA = 1;


    /*------------------------------------------------
     * 进入DHT11严格时序区
     *
     * 保存原EA状态，然后关闭总中断，
     * 防止中断和任务调度影响微秒级时序。
     *------------------------------------------------*/
    ea_state = EA;
    EA = 0;


    /*------------------------------------------------
     * 等待约40us
     *------------------------------------------------*/
	 
    drv_dht11_delay_10us();
    drv_dht11_delay_10us();
    drv_dht11_delay_10us();
    drv_dht11_delay_10us();


    /*------------------------------------------------
     * 等待DHT11响应
     *------------------------------------------------*/
    DHT11_DATA = 1;
	
	if(!DHT11_DATA)
		
	{
		flag = 2;

		while((!DHT11_DATA) && flag++);

		flag = 2;

		while((DHT11_DATA) && flag++);
		
		/*------------------------------------------------
		* 接收40位数据
		*
		* 湿度高8位
		* 湿度低8位
		* 温度高8位
		* 温度低8位
		* 校验和
		*------------------------------------------------*/
		drv_dht11_read_byte(&rh_high);
		drv_dht11_read_byte(&rh_low);
		drv_dht11_read_byte(&t_high);
		drv_dht11_read_byte(&t_low);
		drv_dht11_read_byte(&checksum);
		
		DHT11_DATA = 1;
		
		/*------------------------------------------------
		* 数据校验
		*------------------------------------------------*/
		temp = rh_high +
			rh_low +
			t_high +
			t_low;

		if(temp == checksum)
		{
			/*------------------------------------------------
			* 保存有效数据
			*------------------------------------------------*/
			dht11_humidity_high    = rh_high;
			dht11_humidity_low     = rh_low;
			dht11_temperature_high = t_high;
			dht11_temperature_low  = t_low;
			dht11_checksum         = checksum;
		}
	
	}

    
	/*------------------------------------------------
     * 返回整数温湿度
     *------------------------------------------------*/
    *humidity = rh_high;
    *temperature = t_high;

    


    


    /*------------------------------------------------
     * 恢复进入时序区之前的中断状态
     *------------------------------------------------*/
    EA = ea_state;

    

    return 0;
}