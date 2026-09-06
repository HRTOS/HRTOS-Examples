#ifndef __HRTOS_STORAGE_H__
#define __HRTOS_STORAGE_H__


/*================================================
 * 24C02 EEPROM
 *================================================*/
#ifndef __EEPROM_24C02_H__
#define __EEPROM_24C02_H__


/*------------------------------------------------
 * 24C02 I2C接口
 *------------------------------------------------*/
sbit EEPROM_SCL = P1^1;
sbit EEPROM_SDA = P1^0;


/*------------------------------------------------
 * I2C基础操作
 *------------------------------------------------*/
void eeprom_i2c_start(void);
void eeprom_i2c_stop(void);

unsigned char eeprom_i2c_write_byte(unsigned char dat);
unsigned char eeprom_i2c_read_byte(void);


/*------------------------------------------------
 * EEPROM操作
 *------------------------------------------------*/
void eeprom_24c02_write(unsigned char addr,
                        unsigned char dat);

unsigned char eeprom_24c02_read(unsigned char addr);


/*------------------------------------------------
 * EEPROM延时
 *------------------------------------------------*/
void eeprom_delay(void);


#endif /* __EEPROM_24C02_H__ */


#endif /* __HRTOS_STORAGE_H__ */