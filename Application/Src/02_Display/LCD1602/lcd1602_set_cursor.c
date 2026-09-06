#include "hrtos_hal.h"


/**
 * @brief 设置光标位置
 * @param x 0-15
 * @param y 0-1
 */
void drv_lcd1602_set_cursor(unsigned char x,unsigned char y)
{
    if(y==0)
    {
        drv_lcd1602_write_cmd(0x80+x);
    }
    else
    {
        drv_lcd1602_write_cmd(0xC0+x);
    }
}