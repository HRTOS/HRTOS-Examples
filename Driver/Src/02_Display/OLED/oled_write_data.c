#include "hrtos_hal.h"


/*------------------------------------------------
 * Ð´OLEDÊý¾Ý
 *------------------------------------------------*/
void drv_oled_write_data(unsigned char _data)
{
    drv_oled_iic_start();

    drv_oled_iic_write_byte(0x78);
    drv_oled_iic_wait_ack();

    drv_oled_iic_write_byte(0x40);
    drv_oled_iic_wait_ack();

    drv_oled_iic_write_byte(_data);
    drv_oled_iic_wait_ack();

    drv_oled_iic_stop();
}
