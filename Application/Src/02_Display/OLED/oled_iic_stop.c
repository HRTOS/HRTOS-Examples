#include "hrtos_hal.h"


/*------------------------------------------------
 * Èí¼þIICÍ£Ö¹
 *------------------------------------------------*/
void drv_oled_iic_stop(void)
{
    OLED_SCLK_SET();
    OLED_SDIN_CLR();
    OLED_SDIN_SET();
}