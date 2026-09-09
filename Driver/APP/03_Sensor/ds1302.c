
#include "hrtos.h"
#include "hrtos_hal.h"

/*
 * =========================================================
 * DS1302 + 4位数码管时钟示例
 * ---------------------------------------------------------
 * 功能：
 * 1. 初始化DS1302
 * 2. 读取DS1302当前时间
 * 3. 使用4位数码管显示时、分
 * 4. 每秒更新一次时间
 *
 * 显示格式：
 *
 *       12:35
 *
 * DS1302时间数据采用BCD格式：
 *
 * time[0]：秒
 * time[1]：分
 * time[2]：时
 * time[3]：日
 * time[4]：月
 * time[5]：星期
 * time[6]：年
 * =========================================================
 */


/*------------------------------------------------
 * DS1302时间数据
 *------------------------------------------------*/
unsigned char rtc_time[7];

//u8 sec;
/*------------------------------------------------
 * BCD转十进制
 *
 * 例如：
 * 0x25 -> 25
 * 0x59 -> 59
 *------------------------------------------------*/
unsigned char bcd_to_dec(unsigned char bcd)
{
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}


/*------------------------------------------------
 * DS1302读取任务
 *
 * 每秒读取一次DS1302时间
 *------------------------------------------------*/
void task_ds1302_read(void)
{
    while(1)
    {
        /* 读取DS1302时间 */
        drv_ds1302_read_time(rtc_time);

        /*
         * DS1302每秒自动走时，
         * 因此这里每隔1秒读取一次即可。
         */
		
        os_delay(10);
    }
}


/*------------------------------------------------
 * 数码管显示任务
 *
 * 显示：
 *
 * HHMM
 *
 * 例如：
 * 12:35 -> 1235
 * 08:06 -> 0806
 *------------------------------------------------*/
void task_seg_display(void)
{
    unsigned char hour;
    unsigned char minute;

    while(1)
    {
        /* BCD转换为十进制 */
        hour = bcd_to_dec(rtc_time[2]);
        minute = bcd_to_dec(rtc_time[1]);

        /* 显示小时 */
        drv_seg_display(0, hour / 10);
		os_delay_ms(1);
        drv_seg_display(1, hour % 10);
		os_delay_ms(1);
        /* 显示分钟 */
        drv_seg_display(2, (u16)minute / 10);
		os_delay_ms(1);
        drv_seg_display(3, minute % 10);
		os_delay_ms(1);
		
		
    }
}


/**
 * @brief DS1302时钟示例初始化
 */
void hrtos_main(void)
{
    /* 初始化DS1302 */
    drv_ds1302_init();

    /* 初始化数码管 */
    drv_seg_init();

    /* 创建DS1302读取任务 */
    os_task_create(
        task_ds1302_read,
        2,
        2,
        4
    );

    /* 创建数码管显示任务 */
    os_task_create(
        task_seg_display,
        3,
        2,
        4
    );
}

