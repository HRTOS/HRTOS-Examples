#include "hrtos_hal.h"



/*------------------------------------------------
 * 雨滴传感器初始化
 *------------------------------------------------*/
void drv_raindrop_init(void)
{
    /* DO作为输入使用 */
    DRV_RAINDROP_DO = 1;

    /*
     * ADC上电
     *
     * 这里不指定具体通道，
     * 在读取AO时再选择通道。
     */
    ADC_CONTR = ADC_POWER | ADC_SPEED;

    ADC_RES  = 0;
    ADC_LOW2 = 0;
}