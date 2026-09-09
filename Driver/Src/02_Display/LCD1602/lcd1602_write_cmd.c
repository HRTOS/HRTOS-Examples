#include "hrtos_hal.h"


/**
 * @brief LCD–¥√¸¡Ó
 */
void drv_lcd1602_write_cmd(unsigned char cmd)
{
    LCD_RS = 0;
    LCD_RW = 0;

    LCD_DATA = cmd;

    LCD_EN = 1;
    delay_us();
    LCD_EN = 0;

    delay_ms(1);
}