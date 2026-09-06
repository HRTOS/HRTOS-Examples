#include "hrtos_hal.h"


/*******************************************************************************
 * 函 数 名 : eeprom_i2c_write_byte
 * 功能描述 : I2C 发送一个字节
 * 输    入 : dat - 待发送数据
 * 输    出 : 1 - 收到应答
 *           0 - 未收到应答
 ******************************************************************************/
unsigned char eeprom_i2c_write_byte(unsigned char dat)
{
    unsigned char i;
    unsigned char timeout;

    for (i = 0; i < 8; i++)
    {
        EEPROM_SDA = dat & 0x80;
        dat <<= 1;

        eeprom_delay();

        EEPROM_SCL = 1;
        eeprom_delay();

        EEPROM_SCL = 0;
        eeprom_delay();
    }

    /* 释放 SDA，等待从机应答 */
    EEPROM_SDA = 1;
    eeprom_delay();

    EEPROM_SCL = 1;
    eeprom_delay();

    timeout = 0;

    while (EEPROM_SDA)
    {
        timeout++;

        if (timeout > 200)
        {
            EEPROM_SCL = 0;
            eeprom_delay();

            return 0;
        }
    }

    EEPROM_SCL = 0;
    eeprom_delay();

    return 1;
}



