#include "hrtos_hal.h"


/*------------------------------------------------
 * 读取雨滴传感器数字量
 *
 * DO：
 * 0：检测到雨滴
 * 1：未检测到雨滴
 *------------------------------------------------*/
unsigned char drv_raindrop_read_do(void)
{
    return DRV_RAINDROP_DO;
}