#include "hrtos_hal.h"



/*------------------------------------------------
 * DHT11数据缓存
 *------------------------------------------------*/
unsigned char dht11_humidity_high;
unsigned char dht11_humidity_low;
unsigned char dht11_temperature_high;
unsigned char dht11_temperature_low;
unsigned char dht11_checksum;


/*------------------------------------------------
 * DHT11初始化
 *------------------------------------------------*/
void drv_dht11_init(void)
{
    DHT11_DATA = 1;
	dht11_humidity_high = 0;
	dht11_humidity_low = 0;
	dht11_temperature_high = 0;
	dht11_temperature_low = 0;
	dht11_checksum = 0;
}