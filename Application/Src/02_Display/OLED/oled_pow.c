#include "hrtos_hal.h"

/*------------------------------------------------
 * 功能：计算整数幂
 * 参数：m - 底数
 *       n - 指数
 * 返回：m的n次方
 *------------------------------------------------*/
unsigned long drv_oled_pow(unsigned char m, unsigned char n)
{
    unsigned long result;

    result = 1;

    while(n--)
    {
        result *= m;
    }

    return result;
}