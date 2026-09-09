#include "hrtos_hal.h"


/*------------------------------------------------
 * –¥OLED√¸¡Ó
 *------------------------------------------------*/
void drv_oled_write_command(unsigned char cmd)
{
    drv_oled_iic_start();

    drv_oled_iic_write_byte(0x78);
    drv_oled_iic_wait_ack();

    drv_oled_iic_write_byte(0x00);
    drv_oled_iic_wait_ack();

    drv_oled_iic_write_byte(cmd);
    drv_oled_iic_wait_ack();

    drv_oled_iic_stop();
}