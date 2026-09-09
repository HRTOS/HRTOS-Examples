#include "HRTOS.h"
#include "HRTOS_HAL.h"


/**
 * @brief  矩阵键盘任务
 *
 * 周期性扫描矩阵键盘，并获取当前按键值。
 */
void task_matrixkey(void)
{
    unsigned char key;

    while(1)
	{
	
	key = drv_matrixkey_scan();

    if(key != MATRIXKEY_NONE)
    {
        switch(key)
        {
            case 0:
                /* 按键0 */
                break;

            case 1:
                /* 按键1 */
                break;

            case 2:
                /* 按键2 */
                break;

            case 3:
                /* 按键3 */
                break;

            case 4:
                /* 按键4 */
                break;

            case 5:
                /* 按键5 */
                break;

            case 6:
                /* 按键6 */
                break;

            case 7:
                /* 按键7 */
                break;

            case 8:
                /* 按键8 */
                break;

            case 9:
                /* 按键9 */
                break;

            case 10:
                /* 按键A */
                break;

            case 11:
                /* 按键B */
                break;

            case 12:
                /* 按键C */
                break;

            case 13:
                /* 按键D */
                break;

            case 14:
                /* 按键E */
                break;

            case 15:
                /* 按键F */
                break;
        }
    }

    os_delay(10);
}
}


/**
 * @brief  应用初始化
 */
void app_init(void)
{
    drv_matrixkey_init();

    os_task_create(task_matrixkey,2,2,3);
}


/**
 * @brief  程序入口
 */
void hrtos_main(void)
{
    app_init();
}