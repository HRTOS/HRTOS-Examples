#include "HRTOS.h"
#include "HRTOS_HAL.h"


/**
 * @brief  蜂鸣器任务
 *
 * 蜂鸣器每500ms翻转一次，实现周期性鸣叫。
 */
void task_beep(void)
{
    drv_beep_toggle();

    os_delay(50);
}


/**
 * @brief  应用初始化
 */
void app_init(void)
{
    drv_beep_init();

    os_task_create(task_beep,2,2,3);
}


/**
 * @brief  程序入口
 */
void hrtos_main(void)
{
    app_init();

}