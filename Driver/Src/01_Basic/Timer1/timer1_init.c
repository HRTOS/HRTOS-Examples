#include "hrtos_hal.h"


/**
 * @brief  定时器1初始化
 * @param  reload_h 定时器高8位初值
 * @param  reload_l 定时器低8位初值
 * @retval 无
 */
void drv_timer1_init(unsigned char reload_h, unsigned char reload_l)
{
    TMOD &= 0x0F;     // 清除定时器1配置
    TMOD |= 0x10;     // 定时器1模式1

    TH1 = reload_h;   // 设置初值
    TL1 = reload_l;

    ET1 = 1;          // 开启定时器1中断
    TR1 = 1;          // 启动定时器1
}