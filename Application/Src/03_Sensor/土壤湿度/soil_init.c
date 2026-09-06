#include "hrtos_hal.h"


/*================================================
 * 函数：drv_soil_init
 *
 * 功能：
 *      初始化土壤湿度传感器
 *
 * 说明：
 *      AO连接P1.0
 *      DO连接P1.1
 *================================================*/
void drv_soil_init(void)
{
    /* P1.0设置为ADC功能 */
    P1ASF |= 0x01;

    /* 清除ADC结果 */
    ADC_RES = 0;

    /* 开启ADC */
    ADC_CONTR = ADC_POWER |
                ADC_SPEEDLL |
                ADC_START;
}