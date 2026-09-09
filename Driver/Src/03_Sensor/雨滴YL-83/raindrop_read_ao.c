#include "hrtos_hal.h"


/*------------------------------------------------
 * 读取STC12C5A60S2内部ADC
 *
 * channel：
 * 0~7，对应P1.0~P1.7
 *
 * 返回：
 * 10位ADC转换结果
 * 0~1023
 *------------------------------------------------*/
unsigned int drv_raindrop_read_ao(unsigned char channel)
{
    unsigned int adc_value;

    /* 限制ADC通道范围 */
    channel &= 0x07;

    /*
     * 将对应P1引脚设置为ADC输入
     *
     * P1ASF：
     * bit0 -> P1.0
     * bit1 -> P1.1
     * ...
     * bit7 -> P1.7
     */
    P1ASF |= (1 << channel);

    /*
     * 启动ADC转换
     */
    ADC_CONTR =
        ADC_POWER |
        ADC_SPEED |
        ADC_START |
        channel;

    /*
     * 等待ADC转换完成
     */
    while(!(ADC_CONTR & ADC_FLAG))
    {
        ;
    }

    /*
     * 读取10位ADC结果
     *
     * ADC_RES：
     * 高8位
     *
     * ADC_LOW2：
     * 低2位
     */
    adc_value = ((unsigned int)ADC_RES << 2);
    adc_value |= (ADC_LOW2 & 0x03);

    /*
     * 清除ADC完成标志
     */
    ADC_CONTR &= ~ADC_FLAG;

    return adc_value;
}