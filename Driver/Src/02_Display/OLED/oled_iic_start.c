#include "hrtos_hal.h"

/*------------------------------------------------
 * Èí¼þIIC¿ªÊ¼
 *------------------------------------------------*/
void drv_oled_iic_start(void)
{
    OLED_SCLK_SET();
    OLED_SDIN_SET();
    OLED_SDIN_CLR();
    OLED_SCLK_CLR();
}