#include "hrtos_hal.h"


/*------------------------------------------------
 * 数据排序
 *
 * 功能：
 *     将采样数据按照从小到大排列
 *------------------------------------------------*/
void drv_ultrasonic_sort(void)
{
    unsigned char i;
    unsigned char j;

    unsigned int temp;


    for(i = 0;
        i < DRV_ULTRASONIC_SAMPLE_MAX - 1;
        i++)
    {
        for(j = 0;
            j < DRV_ULTRASONIC_SAMPLE_MAX - i - 1;
            j++)
        {
            if(ultrasonic_buffer[j] >
               ultrasonic_buffer[j + 1])
            {
                temp = ultrasonic_buffer[j];

                ultrasonic_buffer[j] =
                    ultrasonic_buffer[j + 1];

                ultrasonic_buffer[j + 1] =
                    temp;
            }
        }
    }
}