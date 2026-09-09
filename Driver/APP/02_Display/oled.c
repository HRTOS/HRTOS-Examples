
#include "hrtos_hal.h"

/* LED6 */
sbit LED6 = P3^6;


void oled_demo()
{

	unsigned char t;

    drv_oled_init();
    drv_oled_clear();

    t = 0;

    drv_oled_show_string(0, 0, "HRTOS OLED TEST", 16);
    drv_oled_show_string(0, 2, "Status: RUN", 16);
    drv_oled_show_string(0, 4, "CODE:", 16);

    /* drv_oled_show_char(48, 6, t, 16); */

    while(1)
    {
        t++;

        if(t > 100)
        {
            t = 0;
        }

        LED6 = 0;

        drv_oled_show_num(103, 6, t, 3, 16);

        os_delay_ms(20);

        LED6 = 1;

        os_delay_ms(20);
    }
}

void hrtos_main(void)
{
    os_task_create(oled_demo,2,2,5);
}