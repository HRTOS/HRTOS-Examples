
#include "hrtos.h"
#include "hrtos_hal.h"

/*
 * =========================================================
 * 超声波 + 4位数码管测距示例
 * ---------------------------------------------------------
 * 功能：
 * 1. 初始化超声波模块
 * 2. 测量超声波距离
 * 3. 使用4位数码管显示距离
 *
 * 显示单位：
 * mm
 *
 * 例如：
 * 350mm -> 0350
 * 1250mm -> 1250
 *
 * 超声波驱动内部已经完成：
 *     多次采样
 *     数据排序
 *     去除最大值和最小值
 *     平均值计算
 *     距离换算
 * =========================================================
 */


/*------------------------------------------------
 * 超声波距离
 *
 * 单位：mm
 *------------------------------------------------*/
unsigned int ultrasonic_distance = 0;


/*------------------------------------------------
 * 超声波测距任务
 *
 * 周期读取距离
 *------------------------------------------------*/
void task_ultrasonic_read(void)
{
    while(1)
    {
        /* 读取超声波距离 */
        ultrasonic_distance = drv_ultrasonic_measure();//drv_ultrasonic_read();

        /*
         * 稍作延时后再次测量
         */
        os_delay(10);
    }
}


/*------------------------------------------------
 * 数码管显示任务
 *
 * 显示4位距离数据
 *------------------------------------------------*/
void task_seg_display(void)
{
    unsigned int distance;

    while(1)
    {
        /* 获取当前距离 */
        distance = ultrasonic_distance;

        /*
         * 显示距离
         *
         * 例如：
         * 0350mm -> 0350
         * 1250mm -> 1250
         */
        drv_seg_display(0, distance / 1000);
        drv_seg_display(1, distance / 100 % 10);
        drv_seg_display(2, distance / 10 % 10);
        drv_seg_display(3, distance % 10);
    }
}


/**
 * @brief 超声波测距示例初始化
 */
void hrtos_main(void)
{
    /* 初始化超声波模块 */
    drv_ultrasonic_init();

    /* 初始化数码管 */
    drv_seg_init();

    /* 创建超声波测距任务 */
    os_task_create(
        task_ultrasonic_read,
        2,
        2,
        4
    );

    /* 创建数码管显示任务 */
    os_task_create(
        task_seg_display,
        3,
        2,
        4
    );
}

