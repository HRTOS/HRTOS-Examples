#include "hrtos_hal.h"


/**
 * @brief Ð´×Ö·û´®
 */
void drv_lcd1602_write_string(unsigned char *str)
{
    while(*str)
    {
        drv_lcd1602_write_data(*str++);
    }
}