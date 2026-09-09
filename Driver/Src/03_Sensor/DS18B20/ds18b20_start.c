#include "hrtos_hal.h"


/*------------------------------------------------
 * 启动温度转换
 *
 * 发送：
 * 0xCC - Skip ROM
 * 0x44 - Convert T
 *
 * 注意：
 * 单总线通信部分关闭总中断，
 * 防止时序被中断打断。
 *------------------------------------------------*/
void drv_ds18b20_start(void)
{
    unsigned char ea_state;

    ea_state = EA;
    EA = 0;

    drv_ds18b20_reset();

    drv_ds18b20_write_byte(0xCC);
    drv_ds18b20_write_byte(0x44);

    EA = ea_state;
}