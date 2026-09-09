#ifndef __DRV_OLED_INTERNAL_H
#define __DRV_OLED_INTERNAL_H

#include "drv_oled.h"

/* OLED软件IIC */
void drv_oled_iic_start(void);
void drv_oled_iic_stop(void);
void drv_oled_iic_wait_ack(void);
void drv_oled_iic_write_byte(unsigned char data);

/* OLED底层数据传输 */
void drv_oled_write_command(unsigned char cmd);
void drv_oled_write_data(unsigned char data);

/* OLED内部计算 */
unsigned long drv_oled_pow(unsigned char m, unsigned char n);

#endif