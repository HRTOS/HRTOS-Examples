#include "hrtos_hal.h"


/**
 * @brief LCD1602显示示例
 *
 * 功能：
 *  第1行显示：HRTOS 4.0
 *  第2行显示：LCD1602 Demo
 */
void app_lcd1602_example(void)
{
    
	/* LCD1602初始化 */
    drv_lcd1602_init();

    /* 第1行 */
    drv_lcd1602_set_cursor(0, 0);
    drv_lcd1602_write_string("HRTOS 4.0");

    /* 第2行 */
    drv_lcd1602_set_cursor(0, 1);
    drv_lcd1602_write_string("LCD1602 Demo");
	
	while(1);
}

void hrtos_main()
{

	os_task_create(app_lcd1602_example,2,2,5);
}