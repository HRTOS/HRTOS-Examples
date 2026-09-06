#include "hrtos_hal.h"


/**
 * @brief  外部中断1初始化
 * @param  mode
 *         0: 电平触发
 *         1: 下降沿触发
 * @retval 无
 */
void drv_int1_init(unsigned char mode)
{
    EX1 = 1;      // 开启外部中断1

    if(mode)
    {
        IT1 = 1;  // 下降沿触发
    }
    else
    {
        IT1 = 0;  // 电平触发
    }
}