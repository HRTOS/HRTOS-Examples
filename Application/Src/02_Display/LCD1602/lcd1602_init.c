#include "hrtos_hal.h"


/**
 * @brief LCD初始化
 */
void drv_lcd1602_init(void)
{
    LCD_RW = 0;

    drv_lcd1602_write_cmd(0x38);   //8位数据，两行显示
    drv_lcd1602_write_cmd(0x0c);   //显示开，光标关闭
    drv_lcd1602_write_cmd(0x06);   //地址自动加1
    drv_lcd1602_write_cmd(0x01);   //清屏
	
	os_delay_ms(2);
}