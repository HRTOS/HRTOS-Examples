
#include "hrtos.h"
#include "hrtos_hal.h"

/*
 * =========================================================
 * DHT11 + 4位数码管显示示例
 * ---------------------------------------------------------
 * 功能：
 * 1. 初始化DHT11
 * 2. 初始化4位数码管
 * 3. 读取DHT11温度和湿度
 * 4. 使用数码管显示温度和湿度
 *
 * 显示格式：
 * 温度：0025
 * 湿度：0060
 *
 * 数码管每隔一段时间在温度和湿度之间切换
 * =========================================================
 */


/*------------------------------------------------
 * DHT11数据
 *------------------------------------------------*/
unsigned char dht11_humidity;
unsigned char dht11_temperature;


/*------------------------------------------------
 * 显示数据
 *
 * 0：显示温度
 * 1：显示湿度
 *------------------------------------------------*/
unsigned char sensor_display_mode = 0;


/*------------------------------------------------
 * 传感器读取任务
 *
 * 周期读取DHT11温湿度数据
 *------------------------------------------------*/
void task_dht11_read(void)
{
    while(1)
    {
        /*
         * 读取DHT11
         *
         * 返回：
         * 0 - 成功
         * 1 - 失败
         */
        drv_dht11_read(
            &dht11_humidity,
            &dht11_temperature
        );

        /*
         * DHT11需要一定的读取间隔
         */
        os_delay(100);
    }
}


/*------------------------------------------------
 * 数码管显示任务
 *
 * 持续进行动态扫描
 *------------------------------------------------*/
void task_seg_display(void)
{
    unsigned char value;

    while(1)
    {
        /* 根据当前显示模式选择数据 */
        if(sensor_display_mode == 0)
        {
            /* 显示温度 */
            value = dht11_temperature;
        }
        else
        {
            /* 显示湿度 */
            value = dht11_humidity;
        }

        /*
         * 显示4位数据
         *
         * 例如：
         * 温度25 -> 0025
         * 湿度60 -> 0060
         */
        drv_seg_display(0, value / 1000);
        drv_seg_display(1, value / 100 % 10);
        drv_seg_display(2, value / 10 % 10);
        drv_seg_display(3, value % 10);
    }
}


/*------------------------------------------------
 * 显示切换任务
 *
 * 温度、湿度交替显示
 *------------------------------------------------*/
void task_display_switch(void)
{
    while(1)
    {
        os_delay(300);

        /* 切换显示内容 */
        if(sensor_display_mode == 0)
        {
            sensor_display_mode = 1;
        }
        else
        {
            sensor_display_mode = 0;
        }
    }
}


/**
 * @brief DHT11传感器示例初始化
 */
void hrtos_main(void)
{
    /* 初始化DHT11 */
    drv_dht11_init();

    /* 初始化数码管 */
    drv_seg_init();

    /* 创建DHT11读取任务 */
    os_task_create(
        task_dht11_read,
        2,
        3,
        4
    );

    /* 创建数码管显示任务 */
    os_task_create(
        task_seg_display,
        3,
        2,
        4
    );

    /* 创建显示切换任务 */
    os_task_create(
        task_display_switch,
        4,
        2,
        4
    );
}

