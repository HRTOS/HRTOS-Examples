#include "hrtos_hal.h"

/*******************************************************************************
 * 函 数 名 : eeprom_24c02_read
 * 功能描述 : 从 24C02 指定地址读取一个字节
 * 输    入 : addr - EEPROM 地址
 * 输    出 : 读取到的数据
 ******************************************************************************/
unsigned char eeprom_24c02_read(unsigned char addr)
{
    unsigned char dat;

    /* 写入待读取的 EEPROM 地址 */
    eeprom_i2c_start();

    eeprom_i2c_write_byte(0xA0);
    eeprom_i2c_write_byte(addr);

    /* 重启总线，切换到读操作 */
    eeprom_i2c_start();

    eeprom_i2c_write_byte(0xA1);

    dat = eeprom_i2c_read_byte();

    /* 主机发送 NACK，表示读取结束 */
    EEPROM_SDA = 1;
    eeprom_delay();

    EEPROM_SCL = 1;
    eeprom_delay();

    EEPROM_SCL = 0;
    eeprom_delay();

    eeprom_i2c_stop();

    return dat;
}