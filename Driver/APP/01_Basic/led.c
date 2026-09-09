#include "HRTOS.h"
#include "HRTOS_HAL.h"


/**
 * @brief  LED任务
 *
 * 依次点亮8个LED，实现流水灯效果。
 */
void task_led(void)
{
    static unsigned char led = 0;

	while(1)
	{
	drv_led_write(0xff);
    drv_led_on(led);

    led++;

    if(led >= 8)
    {
        led = 0;
    }

    os_delay(20);
	
	}
    
}


/**
 * @brief  应用初始化
 */
void app_init(void)
{
    drv_led_init();

    os_task_create(task_led,2,2,3);
}


/**
 * @brief  程序入口
 */
void hrtos_main(void)
{
    app_init();
}