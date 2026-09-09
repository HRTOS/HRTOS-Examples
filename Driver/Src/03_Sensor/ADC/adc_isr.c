#include "hrtos_hal.h"



/*------------------------------------------------
 * 功能：ADC中断服务程序
 *------------------------------------------------*/
void drv_adc_isr(void)// interrupt 5 using 1
{
    /* 清除ADC中断标志 */
    ADC_CONTR &= ~DRV_ADC_FLAG;

    /* 读取10位ADC转换结果 */
    drv_adc_value = (ADC_RES << 2) + ADC_LOW2;

    /* 启动下一次ADC转换 */
    ADC_CONTR = DRV_ADC_POWER |
                DRV_ADC_SPEEDLL |
                DRV_ADC_START |
                drv_adc_channel;
}