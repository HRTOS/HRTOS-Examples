
#include "hrtos.h"
#include "hrtos_hal.h"

/*
 * =========================================================
 * ADC + 4位数码管显示示例
 * ---------------------------------------------------------
 * 功能：
 * 1. 初始化ADC
 * 2. 初始化4位数码管
 * 3. 使用ADC读取P1.0模拟输入
 * 4. 数码管实时显示ADC转换结果
 *
 * ADC转换结果范围：
 * 0 ~ 1023
 * =========================================================
 */


/*------------------------------------------------
 * ADC显示任务
 *------------------------------------------------*/
void task_adc_display(void)
{
    unsigned int adc_value;

    while(1)
    {
        /* 获取ADC转换结果 */
        adc_value = drv_adc_get_value(7);

        /* 显示ADC结果 */
        drv_seg_display(0, adc_value / 1000);
        drv_seg_display(1, adc_value / 100 % 10);
        drv_seg_display(2, adc_value / 10 % 10);
        drv_seg_display(3, adc_value % 10);

        /*
         * 数码管动态扫描
         * 不需要延时，由任务持续刷新
         */
    }
}


/**
 * @brief ADC + 数码管示例初始化
 */
void hrtos_main(void)
{

    /* 初始化数码管 */
    drv_seg_init();

    /* 创建ADC显示任务 */
    os_task_create(
        task_adc_display,
        2,
        2,
        4
    );
    
}

