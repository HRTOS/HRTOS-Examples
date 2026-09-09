#include "hrtos_hal.h"


/*******************************************************************************
 * 函 数 名 : eeprom_24c02_write
 * 功能描述 : 向 24C02 指定地址写入一个字节
 * 输    入 : addr - EEPROM 地址
 *           dat  - 写入数据
 * 输    出 : 无
 ******************************************************************************/
void eeprom_24c02_write(unsigned char addr, unsigned char dat)
{
    eeprom_i2c_start();

    /* 24C02 写地址 */
    eeprom_i2c_write_byte(0xA0);

    /* EEPROM 内部地址 */
    eeprom_i2c_write_byte(addr);

    /* 写入数据 */
    eeprom_i2c_write_byte(dat);

    eeprom_i2c_stop();

    /*
     * 等待 EEPROM 内部写周期完成
     */
    eeprom_delay();
    eeprom_delay();
    eeprom_delay();
    eeprom_delay();
}