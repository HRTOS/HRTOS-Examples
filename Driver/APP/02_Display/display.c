#include "hrtos.h"
#include "hrtos_hal.h"

/*
 * =========================================================
 * 4位数码管动态显示示例
 * ---------------------------------------------------------
 * 功能：
 * 1. 初始化数码管
 * 2. 使用一个任务进行动态扫描
 * 3. 数码管显示：1234
 * =========================================================
 */

void task_seg_display(void)
{
    while(1)
    {
        /* 显示第1位：1 */
        drv_seg_display(0, 6);
        //os_delay(2);

        /* 显示第2位：2 */
        drv_seg_display(1, 6);
        //os_delay(2);

        /* 显示第3位：3 */
        drv_seg_display(2, 6);
        //os_delay(2);

        /* 显示第4位：4 */
        drv_seg_display(3, 6);
        //os_delay(2);
    }
}


/**
 * @brief 数码管示例初始化
 */
void hrtos_main(void)
{
    /* 初始化数码管 */
    drv_seg_init();

    /* 创建数码管显示任务 */
    os_task_create(
        task_seg_display,
        2,
        2,
		4
    );
}