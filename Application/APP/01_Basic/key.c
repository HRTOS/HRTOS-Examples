#include "HRTOS.h"
#include "HRTOS_HAL.h"


/**
 * @brief  按键任务
 *
 * 扫描按键，并根据按键执行对应操作。
 */
void task_key(void)
{
    unsigned char key;

    key = drv_key_scan();
	
	//P1=key;while(P1!=0X00);while(1);

    switch(key)
    {
        case KEY_1:
            drv_beep_on();
            break;

        case KEY_2:
            drv_beep_off();
            break;

        case KEY_3:
            drv_beep_toggle();
            break;

        case KEY_4:
            break;

        default:
            break;
    }

    os_delay(10);
}


/**
 * @brief  应用初始化
 */
void app_init(void)
{
    drv_key_init();
    drv_beep_init();

    os_task_create(task_key,2,2,3);
}


/**
 * @brief  程序入口
 */
void hrtos_main(void)
{
    app_init();
}