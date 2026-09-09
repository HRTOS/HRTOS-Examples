#include "hrtos_hal.h"

/*------------------------------------------------
 * 获取超声波距离
 *
 * 返回：
 *     距离，单位mm
 *
 * 处理流程：
 *     1. 多次采样
 *     2. 数据排序
 *     3. 去除最大值和最小值
 *     4. 求平均值
 *     5. 根据计数值换算距离
 *------------------------------------------------*/
unsigned int drv_ultrasonic_read(void)
{
    unsigned char i;

    unsigned long sum;

    unsigned int average;
    unsigned int distance;


    /*--------------------------------------------
     * 多次采样
     *--------------------------------------------*/
	HRTOS_DRV_LOOP_0001:
    for(i = 0;
        i < DRV_ULTRASONIC_SAMPLE_MAX;
        )
    {
        ultrasonic_buffer[i] =
            drv_ultrasonic_measure();
		if(ultrasonic_buffer[i]!=0)
			i++;
		os_delay(1);
    }


    /*--------------------------------------------
     * 数据排序
     *--------------------------------------------*/
    drv_ultrasonic_sort();


    /*--------------------------------------------
     * 去除最大值和最小值
     *--------------------------------------------*/
    sum = 0;

    for(i = DRV_ULTRASONIC_FILTER_COUNT;
        i < DRV_ULTRASONIC_SAMPLE_MAX -
            DRV_ULTRASONIC_FILTER_COUNT;
        i++)
    {
        sum += ultrasonic_buffer[i];
    }


    /*--------------------------------------------
     * 求平均值
     *--------------------------------------------*/
    average =
        (unsigned int)(
            sum /
            (DRV_ULTRASONIC_SAMPLE_MAX -
             2 * DRV_ULTRASONIC_FILTER_COUNT)
        );


    /*--------------------------------------------
     * 根据Timer1计数值换算距离
     *
     * 11.0592MHz晶振：
     *
     * 传统12T 51：
     *     机器周期约为1.085us
     *
     * 超声波距离：
     *
     *     距离 = 时间 × 声速 / 2
     *
     * 这里按照原例程的实际换算系数，
     * 将Timer计数值转换为mm。
     *--------------------------------------------*/
    distance =
        (unsigned int)(average * 0.1953925);

	if(distance!=0)
    return distance;
	else 
	{
		goto HRTOS_DRV_LOOP_0001;
	
	}
}
                