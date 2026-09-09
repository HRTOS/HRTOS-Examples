
#include "hrtos.h"
#include "hrtos_hal.h"

/*
 * =========================================================
 * 土壤湿度传感器综合示例
 * ---------------------------------------------------------
 * 功能：
 * 1. 初始化土壤湿度传感器
 * 2. 读取AO模拟量
 * 3. 读取DO数字量
 * 4. 使用4位数码管显示AO模拟量
 *
 * AO：
 *     P1.0
 *     ADC范围：0~1023
 *
 * DO：
 *     P1.1
 *     0：检测到湿度达到模块阈值
 *     1：未达到模块阈值
 *
 * 数码管显示：
 *     显示AO模拟量
 *
 * 例如：
 *     0256
 *     0512
 *     1023
 * =========================================================
 */


/*------------------------------------------------
 * 土壤湿度数据
 *------------------------------------------------*/
unsigned int soil_ao_value = 0;

unsigned char soil_do_value = 1;


/*------------------------------------------------
 * 土壤湿度读取任务
 *------------------------------------------------*/
void task_soil_read(void)
{
    while(1)
    {
        /* 读取AO模拟量 */
        soil_ao_value = drv_soil_read_ao();

        /* 读取DO数字量 */
        soil_do_value = drv_soil_read_do();

        /*
         * 周期读取
         */
        os_delay(100);
    }
}


/*------------------------------------------------
 * 数码管显示任务
 *
 * 显示AO模拟量：
 *
 * 0000 ~ 1023
 *------------------------------------------------*/
void task_seg_display(void)
{
    unsigned int value;

    while(1)
    {
        /* 获取当前土壤湿度模拟量 */
        //value = soil_ao_value;
		
		value = soil_do_value;

        /* 显示4位ADC数据 */
        drv_seg_display(0, value / 1000);
        drv_seg_display(1, value / 100 % 10);
        drv_seg_display(2, value / 10 % 10);
        drv_seg_display(3, value % 10);
    }
}


/**
 * @brief 土壤湿度传感器综合示例初始化
 */
void hrtos_main(void)
{
    /* 初始化土壤湿度传感器 */
    drv_soil_init();

    /* 初始化数码管 */
    drv_seg_init();

    /* 创建土壤湿度读取任务 */
    os_task_create(
        task_soil_read,
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

