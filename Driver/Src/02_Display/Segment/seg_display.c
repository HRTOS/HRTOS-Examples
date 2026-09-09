#include "hrtos_hal.h"


/**
 * @brief 显示单个数码管
 * @param pos 位选择 0-3
 * @param num 显示数据 0-F
 */
void drv_seg_display(unsigned char pos, unsigned char num)
{

    SEG1 = 1;
    SEG2 = 1;
    SEG3 = 1;
    SEG4 = 1;


    SEG_PORT = seg_table[num];


    switch(pos)
    {
        case 0:
            SEG1 = 0;
            break;

        case 1:
            SEG2 = 0;
            break;

        case 2:
            SEG3 = 0;
            break;

        case 3:
            SEG4 = 0;
            break;
    }
}