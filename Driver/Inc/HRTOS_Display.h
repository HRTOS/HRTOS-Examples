#ifndef __HRTOS_DISPLAY_H__
#define __HRTOS_DISPLAY_H__


/*================================================
 * 数码管
 *================================================*/
#ifndef __DRV_SEG_H__
#define __DRV_SEG_H__


/*------------------------------------------------
 * 数码管IO定义
 *------------------------------------------------*/
#define SEG_PORT    P0

sbit SEG1 = P2^0;
sbit SEG2 = P2^1;
sbit SEG3 = P2^2;
sbit SEG4 = P2^3;


/*------------------------------------------------
 * 数码管字模
 *------------------------------------------------*/
extern unsigned char code seg_table[];


/*------------------------------------------------
 * 数码管初始化
 *------------------------------------------------*/
void drv_seg_init(void);


/*------------------------------------------------
 * 数码管显示
 *
 * 参数：
 *      pos：显示位置
 *      num：显示数字
 *------------------------------------------------*/
void drv_seg_display(unsigned char pos, unsigned char num);


#endif /* __DRV_SEG_H__ */


/*================================================
 * LCD1602
 *================================================*/
#ifndef __DRV_LCD1602_H__
#define __DRV_LCD1602_H__


/*------------------------------------------------
 * LCD1602 IO定义
 *------------------------------------------------*/
#define LCD_DATA    P2

sbit LCD_RS = P0^7;
sbit LCD_RW = P0^6;
sbit LCD_EN = P0^5;


/*------------------------------------------------
 * LCD1602初始化
 *------------------------------------------------*/
void drv_lcd1602_init(void);


/*------------------------------------------------
 * LCD1602写命令
 *------------------------------------------------*/
void drv_lcd1602_write_cmd(unsigned char cmd);


/*------------------------------------------------
 * LCD1602写数据
 *------------------------------------------------*/
void drv_lcd1602_write_data(unsigned char dat);


/*------------------------------------------------
 * LCD1602写字符串
 *------------------------------------------------*/
void drv_lcd1602_write_string(unsigned char *str);


/*------------------------------------------------
 * LCD1602设置光标
 *
 * 参数：
 *      x：横坐标
 *      y：纵坐标
 *------------------------------------------------*/
void drv_lcd1602_set_cursor(unsigned char x, unsigned char y);


/*------------------------------------------------
 * LCD1602延时
 *------------------------------------------------*/
void delay_ms(unsigned int ms);
void delay_us(void);


#endif /* __DRV_LCD1602_H__ */


/*================================================
 * OLED
 *================================================*/
#ifndef __DRV_OLED_H__
#define __DRV_OLED_H__


/*------------------------------------------------
 * OLED显示参数
 *------------------------------------------------*/
#define DRV_OLED_WIDTH     128
#define DRV_OLED_HEIGHT    64


/*------------------------------------------------
 * OLED初始化
 *------------------------------------------------*/
void drv_oled_init(void);


/*------------------------------------------------
 * OLED显示控制
 *------------------------------------------------*/
void drv_oled_on(void);
void drv_oled_off(void);


/*------------------------------------------------
 * OLED显示操作
 *------------------------------------------------*/
void drv_oled_clear(void);
void drv_oled_fill(unsigned char _data);
void drv_oled_set_pos(unsigned char x, unsigned char y);


/*------------------------------------------------
 * OLED字符显示
 *------------------------------------------------*/
void drv_oled_show_char(unsigned char x,
                        unsigned char y,
                        unsigned char chr,
                        unsigned char _size);

void drv_oled_show_string(unsigned char x,
                          unsigned char y,
                          unsigned char *str,
                          unsigned char _size);

void drv_oled_show_num(unsigned char x,  
                       unsigned char y,
                       unsigned long num,
                       unsigned char len,
                       unsigned char _size);


/*------------------------------------------------
 * OLED汉字显示
 *------------------------------------------------*/
void drv_oled_show_chinese(unsigned char x,
                           unsigned char y,
                           unsigned char no);


/*------------------------------------------------
 * OLED图片显示
 *------------------------------------------------*/
void drv_oled_draw_bmp(unsigned char x0,
                       unsigned char y0,
                       unsigned char x1,
                       unsigned char y1,
                       unsigned char *bmp);


/*------------------------------------------------
 * OLED IO定义
 *------------------------------------------------*/
sbit OLED_SCL  = P1^0;
sbit OLED_SDIN = P1^1;


/*------------------------------------------------
 * OLED时钟控制
 *------------------------------------------------*/
#define OLED_SCLK_CLR()    OLED_SCL = 0
#define OLED_SCLK_SET()    OLED_SCL = 1


/*------------------------------------------------
 * OLED数据控制
 *------------------------------------------------*/
#define OLED_SDIN_CLR()    OLED_SDIN = 0
#define OLED_SDIN_SET()    OLED_SDIN = 1


/*------------------------------------------------
 * OLED底层I2C操作
 *------------------------------------------------*/
void drv_oled_iic_write_byte(unsigned char _data);

unsigned long drv_oled_pow(unsigned char m,
                           unsigned char n);

void drv_oled_write_data(unsigned char _data);
void drv_oled_write_command(unsigned char cmd);

void drv_oled_iic_wait_ack(void);
void drv_oled_iic_start(void);
void drv_oled_iic_stop(void);


#endif /* __DRV_OLED_H__ */


/*================================================
 * OLED字库
 *================================================*/
#ifndef __OLED_FONT_H__
#define __OLED_FONT_H__


/*------------------------------------------------
 * 常用ASCII表
 *
 * 偏移量：32
 * ASCII字符集
 *
 * 大小：12*6
 *------------------------------------------------*/
extern unsigned char code Hzk[][32];

extern const unsigned char code F8X16[];

extern const unsigned char code F6x8[][6];


#endif /* __OLED_FONT_H__ */


/*================================================
 * OLED图片数据
 *================================================*/

/*------------------------------------------------
 * OLED模块接线
 *
 * GND    电源地
 * VCC    接5V或3.3V电源
 * D0     P1^0（SCL）
 * D1     P1^1（SDA）
 * RES    接P1^2
 * DC     接P1^3
 * CS     接P1^4
 *------------------------------------------------*/

#ifndef __BMP_H__
#define __BMP_H__


extern unsigned char code BMP1[];
extern unsigned char code BMP2[];


#endif /* __BMP_H__ */


#endif /* __HRTOS_DISPLAY_H__ */