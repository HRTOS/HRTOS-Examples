#include "hrtos_hal.h"


/*------------------------------------------------
 * 等待IIC应答
 *
 * 当前OLED模块采用固定时序，
 * 不读取SDA，因此仅产生一个时钟。
 *------------------------------------------------*/
void drv_oled_iic_wait_ack(void)
{
    OLED_SCLK_SET();
    OLED_SCLK_CLR();
}