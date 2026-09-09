#include "hrtos_hal.h"

/**
 * @brief  外部中断0初始化
 * @param  mode
 *         0: 电平触发
 *         1: 下降沿触发
 * @retval 无
 */
void drv_int0_init(unsigned char mode)
{
    EX0 = 1;      // 开启外部中断0

    if(mode)
    {
        IT0 = 1;  // 下降沿触发
    }
    else
    {
        IT0 = 0;  // 电平触发
    }
}