#include "hrtos_hal.h"



/*------------------------------------------------
 * ADC当前通道
 *
 * 当前示例使用P1.0
 *------------------------------------------------*/
unsigned char drv_adc_channel = 0;


/*------------------------------------------------
 * ADC转换结果
 *------------------------------------------------*/
unsigned int drv_adc_value = 0;


/*------------------------------------------------
 * 功能：ADC初始化
 * 说明：配置P1.0为ADC输入并启动ADC转换
 *------------------------------------------------*/
void drv_adc_init(void)
{
    P1ASF = 0x01;

    ADC_RES = 0;

    ADC_CONTR = DRV_ADC_POWER |
                DRV_ADC_SPEEDLL |
                DRV_ADC_START |
                drv_adc_channel;

    /*
     * ADC上电及启动转换等待
     * 原程序Delay(2)
     */
    os_delay_ms(2);
}