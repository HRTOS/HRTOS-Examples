#include "hrtos_hal.h"


/*******************************************************************************
 * 函 数 名 : eeprom_i2c_start
 * 功能描述 : 产生 I2C 起始信号
 * 输    入 : 无
 * 输    出 : 无
 ******************************************************************************/
void eeprom_i2c_start(void)
{
    EEPROM_SDA = 1;
    eeprom_delay();

    EEPROM_SCL = 1;
    eeprom_delay();

    EEPROM_SDA = 0;
    eeprom_delay();

    EEPROM_SCL = 0;
    eeprom_delay();
}