#include "hrtos_hal.h"

/* ADC结果低2位寄存器 */
sfr ADC_RESL = 0xBE;

/*------------------------------------------------
 * 函数名：drv_adc_get_value
 * 功能  ：读取指定ADC通道
 * 输入  ：channel - ADC通道号
 * 返回  ：10位ADC转换结果
 *         范围：0~1023
 *------------------------------------------------*/
u16 drv_adc_get_value(u8 channel)
{
    u16 value;
    u8 i;

    /* ADC通道范围限制 */
    if (channel > ADC_CH7)
    {
        return 0;
    }

    /* 设置P1.0~P1.7为模拟输入 */
    P1ASF = 0xFF;

    /* 清空ADC结果寄存器 */
    ADC_RES  = 0;
    ADC_RESL = 0;

    /* ADC速度及通道配置 */
    ADC_CONTR = 0x80 | 0x08 | 0x20 | channel;

    /* 等待ADC输入稳定 */
    i = 20;

    while (i--)
    {
        ;
    }

    /* 等待转换完成 */
    while (!(ADC_CONTR & 0x10))
    {
        ;
    }

    /* 清除ADC完成标志 */
    ADC_CONTR &= 0xF7;

    /* 获取10位ADC转换结果 */
    value = ADC_RES * 4 + ADC_RESL;

    return value;
}