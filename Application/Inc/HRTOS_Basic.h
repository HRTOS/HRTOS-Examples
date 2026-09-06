#ifndef __HRTOS_BASIC_H__
#define __HRTOS_BASIC_H__

/*================================================
 * 基础输入输出模块
 *================================================*/

/*------------------------------------------------
 * 蜂鸣器
 *------------------------------------------------*/
#ifndef __DRV_BEEP_H__
#define __DRV_BEEP_H__

sbit beep = P3^6;

#define BEEP_ON     0
#define BEEP_OFF    1

void drv_beep_init(void);
void drv_beep_on(void);
void drv_beep_off(void);
void drv_beep_toggle(void);

#endif


/*------------------------------------------------
 * 独立按键
 *------------------------------------------------*/
#ifndef __DRV_KEY_H__
#define __DRV_KEY_H__

#define KEY_NONE    0

#define KEY_1       1
#define KEY_2       2
#define KEY_3       3
#define KEY_4       4

sbit key1 = P3^2;
sbit key2 = P3^3;
sbit key3 = P3^4;
sbit key4 = P3^5;

#define GPIO_KEY    P1

void drv_key_init(void);
unsigned char drv_key_scan(void);

#endif


/*------------------------------------------------
 * 矩阵按键
 *------------------------------------------------*/
#ifndef __DRV_MATRIXKEY_H__
#define __DRV_MATRIXKEY_H__

#define MATRIXKEY_NONE  0xff

void drv_matrixkey_init(void);
unsigned char drv_matrixkey_scan(void);

#endif


/*------------------------------------------------
 * LED
 *------------------------------------------------*/
#ifndef __DRV_LED_H__
#define __DRV_LED_H__

#define LED_ON      0
#define LED_OFF     1

#define LED_PORT    P1

void drv_led_init(void);
void drv_led_on(unsigned char led);
void drv_led_off(unsigned char led);
void drv_led_toggle(unsigned char led);
void drv_led_write(unsigned char value);

#endif


/*------------------------------------------------
 * 定时器1
 *------------------------------------------------*/
#ifndef __DRV_TIMER1_H__
#define __DRV_TIMER1_H__

void drv_timer1_init(unsigned char reload_h, unsigned char reload_l);

#endif


#endif /* __HRTOS_BASIC_H__ */