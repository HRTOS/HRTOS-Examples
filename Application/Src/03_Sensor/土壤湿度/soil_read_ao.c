#include "hrtos_hal.h"


/*================================================
 * 函数：drv_soil_read_ao
 *
 * 功能：
 *      读取土壤湿度模拟量
 *
 * 返回：
 *      10位ADC转换结果
 *
 * 范围：
 *      0~1023
 *
 * 说明：
 *      AO连接P1.0，
 *      因此使用ADC通道0。
 *================================================*/
unsigned int drv_soil_read_ao(void)
{
    unsigned int value;

    /* 启动ADC转换 */
    ADC_CONTR = ADC_POWER |
                ADC_SPEEDLL |
                ADC_START |
                0;

    /* 等待ADC转换完成 */
    while(!(ADC_CONTR & ADC_FLAG))
    {
        ;
    }

    /* 清除ADC完成标志 */
    ADC_CONTR &= ~ADC_FLAG;

    /* 获取10位ADC结果 */
    value = ((unsigned int)ADC_RES << 2);
    value |= (ADC_LOW2 & 0x03);

    return value;
}