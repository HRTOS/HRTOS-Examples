#include "hrtos_hal.h"


/*------------------------------------------------
 * 读取DS1302时间
 *
 * time[0]：秒
 * time[1]：分
 * time[2]：时
 * time[3]：日
 * time[4]：月
 * time[5]：星期
 * time[6]：年
 *
 * 返回数据采用BCD格式。
 *------------------------------------------------*/
void drv_ds1302_read_time(unsigned char *time)
{
    unsigned char i;
    unsigned char ea_state;


    /* 保存当前中断状态 */
    ea_state = EA;

    /* 保护连续读取时序 */
    EA = 0;


    /*------------------------------------------------
     * 读取7个时间寄存器
     *
     * 秒、分、时、日、月、星期、年
     *------------------------------------------------*/
    for(i = 0; i < 7; i++)
    {
        time[i] = drv_ds1302_read_byte(
            drv_ds1302_read_addr[i]
        );
    }


    /* 恢复原来的中断状态 */
    EA = ea_state;
}