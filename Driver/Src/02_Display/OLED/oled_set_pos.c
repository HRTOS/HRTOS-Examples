#include "hrtos_hal.h"

/*------------------------------------------------
 * 设置显示位置
 *
 * x：0~127
 * y：0~7，OLED页地址
 *------------------------------------------------*/
void drv_oled_set_pos(unsigned char x, unsigned char y)
{
    drv_oled_write_command(0xB0 + y);
    drv_oled_write_command(((x + 2) & 0xF0) >> 4 | 0x10);
    drv_oled_write_command((x + 2) & 0x0F);
}