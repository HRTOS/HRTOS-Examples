#include "hrtos_hal.h"


/*******************************************************************************
 * 函 数 名 : eeprom_i2c_read_byte
 * 功能描述 : I2C 读取一个字节
 * 输    入 : 无
 * 输    出 : 读取到的数据
 ******************************************************************************/
unsigned char eeprom_i2c_read_byte(void)
{
    unsigned char i;
    unsigned char dat;

    dat = 0;

    EEPROM_SDA = 1;

    for (i = 0; i < 8; i++)
    {
        EEPROM_SCL = 1;
        eeprom_delay();

        dat <<= 1;
        dat |= EEPROM_SDA;

        EEPROM_SCL = 0;
        eeprom_delay();
    }

    return dat;
}

