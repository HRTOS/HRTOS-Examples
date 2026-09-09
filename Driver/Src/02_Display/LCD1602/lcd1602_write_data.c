#include "hrtos_hal.h"


/**
 * @brief LCDÐ´Êý¾Ý
 */
void drv_lcd1602_write_data(unsigned char dat)
{
    LCD_RS = 1;
    LCD_RW = 0;

    LCD_DATA = dat;

    LCD_EN = 1;
    delay_us();
    LCD_EN = 0;

    delay_ms(1);
}