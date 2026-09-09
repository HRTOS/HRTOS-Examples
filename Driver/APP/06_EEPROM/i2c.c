
#include "hrtos.h"
#include "hrtos_hal.h"



/*
 * =========================================================
 * 24C02 EEPROM + 4位数码管显示示例
 * ---------------------------------------------------------
 * 功能：
 * 1. 从24C02读取保存的数据
 * 2. 将数据加1后重新写入EEPROM
 * 3. 使用4位数码管显示当前保存的数据
 *
 * EEPROM地址：
 *     0x00
 *
 * 显示范围：
 *     0000 ~ 0255
 *
 * 实验现象：
 *     第一次运行：
 *         读取EEPROM中的数据
 *         数据 +1
 *         写回EEPROM
 *
 *     重新上电：
 *         EEPROM中的数据不会丢失
 *         数码管显示的数据继续增加
 * =========================================================
 */


/*------------------------------------------------
 * EEPROM数据
 *------------------------------------------------*/
unsigned char eeprom_value = 0;


/*------------------------------------------------
 * EEPROM读写任务
 *------------------------------------------------*/
void task_eeprom(void)
{
    while(1)
    {
        /*
         * 读取EEPROM保存的数据
         */
        
		eeprom_value = eeprom_24c02_read(0x00);
		os_delay(100);

        /*
         * 数据加1
         */
        eeprom_value++;

        /*
         * 保存到EEPROM
         */
		eeprom_24c02_write(0,eeprom_value);
        
		
        /*
         * 等待一段时间
         *
         * 防止连续写入EEPROM
         */
        os_delay(100);
    }
}


/*------------------------------------------------
 * 数码管显示任务
 *
 * 显示EEPROM中的数据
 *------------------------------------------------*/
void task_seg_display(void)
{
    unsigned int value;

    while(1)
    {
        value = eeprom_value;

        /*
         * 显示0000~0255
         */
        drv_seg_display(0, value / 1000);
        drv_seg_display(1, value / 100 % 10);
        drv_seg_display(2, value / 10 % 10);
        drv_seg_display(3, value % 10);
    }
}


/**
 * @brief 24C02 EEPROM示例初始化
 */
void hrtos_main(void)
{
    /* 读取EEPROM初始数据 */
    eeprom_value = eeprom_24c02_read(0x00);

    /* 初始化数码管 */
    drv_seg_init();

    /* 创建EEPROM读写任务 */
    os_task_create(
        task_eeprom,
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

